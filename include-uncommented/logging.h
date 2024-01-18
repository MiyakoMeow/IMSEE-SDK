#ifndef CERCES_INTERNAL_MINIGLOG_GLOG_LOGGING_H_
#define CERCES_INTERNAL_MINIGLOG_GLOG_LOGGING_H_

#ifdef ANDROID
#include <android/log.h>
#elif WIN32

#else
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#define CERES_EXPORT
const int FATAL = -3;
const int ERROR = -2;
const int WARNING = -1;
const int INFO = 0;
namespace google
{
typedef int LogSeverity;
const int INFO = ::INFO;
const int WARNING = ::WARNING;
const int ERROR = ::ERROR;
const int FATAL = ::FATAL;
class CERES_EXPORT LogSink
{
  public:
    virtual ~LogSink()
    {
    }
    virtual void send(LogSeverity severity, const char *full_filename, const char *base_filename, int line,
                      const struct tm *tm_time, const char *message, size_t message_len) = 0;
    virtual void WaitTillSent() = 0;
};
extern CERES_EXPORT std::set<LogSink *> log_sinks_global;

inline void InitGoogleLogging(char *argv)
{
}
inline void AddLogSink(LogSink *sink)
{
    log_sinks_global.insert(sink);
}
inline void RemoveLogSink(LogSink *sink)
{
    log_sinks_global.erase(sink);
}

};
class CERES_EXPORT MessageLogger
{
  public:
    MessageLogger(const char *file, int line, const char *tag, int severity)
        : file_(file), line_(line), tag_(tag), severity_(severity)
    {
        StripBasename(std::string(file), &filename_only_);
        stream_ << filename_only_ << ":" << line << " ";
    }
    ~MessageLogger()
    {
        stream_ << "\n";

#ifdef ANDROID
        static const int android_log_levels[] = {
            ANDROID_LOG_FATAL,
            ANDROID_LOG_ERROR,
            ANDROID_LOG_WARN,
            ANDROID_LOG_INFO,
            ANDROID_LOG_DEBUG,
            ANDROID_LOG_VERBOSE,
        };
        const int kMaxVerboseLevel = 2;
        int android_level_index = std::min(std::max(FATAL, severity_), kMaxVerboseLevel) - FATAL;
        int android_log_level = android_log_levels[android_level_index];
        __android_log_write(android_log_level, tag_.c_str(), stream_.str().c_str());
        if (severity_ == FATAL)
        {
            __android_log_write(ANDROID_LOG_FATAL, tag_.c_str(), "terminating.\n");
        }
#elif WIN32

#else
        timeval curTime;
        gettimeofday(&curTime, NULL);
        int milli = curTime.tv_usec / 1000;
        char buffer[20];
        strftime(buffer, 80, "%m-%d %H:%M:%S", localtime(&curTime.tv_sec));
        char time_cstr[24] = "";
        sprintf(time_cstr, "%s:%d ", buffer, milli);
        char tid_cstr[24] = "";
        pid_t pid = getpid();
        pthread_t tid = pthread_self();
        sprintf(tid_cstr, "%d/%u ", pid, tid);
        if (severity_ == FATAL)
        {
            std::cerr << "\033[1;35m" << tid_cstr << time_cstr << SeverityLabelStr() << stream_.str() << "\033[0m";
        }
        else if (severity_ == ERROR)
        {
            std::cerr << "\033[1;31m" << tid_cstr << time_cstr << SeverityLabelStr() << stream_.str() << "\033[0m";
        }
        else if (severity_ == WARNING)
        {
            std::cerr << "\033[1;33m" << tid_cstr << time_cstr << SeverityLabelStr() << stream_.str() << "\033[0m";
        }
        else
        {
            std::cerr << tid_cstr << time_cstr << SeverityLabelStr() << stream_.str();
        }
#endif

        LogToSinks(severity_);
        WaitForSinks();
        if (severity_ == FATAL)
        {
            abort();
        }
    }
    std::stringstream &stream()
    {
        return stream_;
    }

  private:
    void LogToSinks(int severity)
    {
        time_t rawtime;
        time(&rawtime);

        struct tm timeinfo;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        localtime_s(&timeinfo, &rawtime);
#else
        localtime_r(&rawtime, &timeinfo);
#endif

        std::set<google::LogSink *>::iterator iter;
        for (iter = google::log_sinks_global.begin(); iter != google::log_sinks_global.end(); ++iter)
        {
            (*iter)->send(severity, file_.c_str(), filename_only_.c_str(), line_, &timeinfo, stream_.str().c_str(),
                          stream_.str().size());
        }
    }

    void WaitForSinks()
    {
        std::set<google::LogSink *>::iterator iter;
        for (iter = google::log_sinks_global.begin(); iter != google::log_sinks_global.end(); ++iter)
        {
            (*iter)->WaitTillSent();
        }
    }

    void StripBasename(const std::string &full_path, std::string *filename)
    {
        const char kSeparator = '/';
        size_t pos = full_path.rfind(kSeparator);
        if (pos != std::string::npos)
        {
            *filename = full_path.substr(pos + 1, std::string::npos);
        }
        else
        {
            *filename = full_path;
        }
    }

    char SeverityLabel()
    {
        switch (severity_)
        {
        case FATAL:
            return 'F';
        case ERROR:
            return 'E';
        case WARNING:
            return 'W';
        case INFO:
            return 'I';
        default:
            return 'V';
        }
    }

    std::string SeverityLabelStr()
    {
        switch (severity_)
        {
        case FATAL:
            return "FATAL   ";
        case ERROR:
            return "ERROR   ";
        case WARNING:
            return "WARNING ";
        case INFO:
            return "INFO    ";
        default:
            return "VERBOSE ";
        }
    }

    std::string file_;
    std::string filename_only_;
    int line_;
    std::string tag_;
    std::stringstream stream_;
    int severity_;
};
class CERES_EXPORT LoggerVoidify
{
  public:
    LoggerVoidify()
    {
    }
    void operator&(const std::ostream &s)
    {
    }
};
#define LOG_IF(severity, condition)                                                                                    \
    !(condition) ? (void)0 : LoggerVoidify() & MessageLogger((char *)__FILE__, __LINE__, "native", severity).stream()
#define LOG_IF_FALSE(severity, condition) LOG_IF(severity, !(condition))
#ifdef MAX_LOG_LEVEL
#define LOG(n) LOG_IF(n, n <= MAX_LOG_LEVEL)
#define VLOG(n) LOG_IF(n, n <= MAX_LOG_LEVEL)
#define LG LOG_IF(INFO, INFO <= MAX_LOG_LEVEL)
#define VLOG_IF(n, condition) LOG_IF(n, (n <= MAX_LOG_LEVEL) && condition)
#else
#define LOG(n) MessageLogger((char *)__FILE__, __LINE__, "native", n).stream()
#define VLOG(n) MessageLogger((char *)__FILE__, __LINE__, "native", n).stream()
#define LG MessageLogger((char *)__FILE__, __LINE__, "native", INFO).stream()
#define VLOG_IF(n, condition) LOG_IF(n, condition)
#endif
#ifndef MAX_LOG_LEVEL
#define VLOG_IS_ON(x) (1)
#else
#define VLOG_IS_ON(x) (x <= MAX_LOG_LEVEL)
#endif

#ifndef NDEBUG
#define DLOG LOG
#else
#define DLOG(severity)                                                                                                 \
    true ? (void)0 : LoggerVoidify() & MessageLogger((char *)__FILE__, __LINE__, "native", severity).stream()
#endif
template <class T> void LogMessageFatal(const char *file, int line, const T &message)
{
    MessageLogger((char *)__FILE__, __LINE__, "native", FATAL).stream() << message;
}
#define CHECK(condition) LOG_IF_FALSE(FATAL, condition) << "Check failed: " #condition " "

#ifndef NDEBUG
#define DCHECK(condition) LOG_IF_FALSE(FATAL, condition) << "Check failed: " #condition " "
#else
#define DCHECK(condition)                                                                                              \
    if (false)                                                                                                         \
    LOG_IF_FALSE(FATAL, condition) << "Check failed: " #condition " "
#endif
#define CHECK_OP(val1, val2, op) LOG_IF_FALSE(FATAL, ((val1)op(val2))) << "Check failed: " #val1 " " #op " " #val2 " "
#define CHECK_EQ(val1, val2) CHECK_OP(val1, val2, ==)
#define CHECK_NE(val1, val2) CHECK_OP(val1, val2, !=)
#define CHECK_LE(val1, val2) CHECK_OP(val1, val2, <=)
#define CHECK_LT(val1, val2) CHECK_OP(val1, val2, <)
#define CHECK_GE(val1, val2) CHECK_OP(val1, val2, >=)
#define CHECK_GT(val1, val2) CHECK_OP(val1, val2, >)
#define CHECK_NEAR(val1, val2, margin)                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        CHECK_LE((val1), (val2) + (margin));                                                                           \
        CHECK_GE((val1), (val2) - (margin));                                                                           \
    } while (0)

#ifndef NDEBUG
#define DCHECK_EQ(val1, val2) CHECK_OP(val1, val2, ==)
#define DCHECK_NE(val1, val2) CHECK_OP(val1, val2, !=)
#define DCHECK_LE(val1, val2) CHECK_OP(val1, val2, <=)
#define DCHECK_LT(val1, val2) CHECK_OP(val1, val2, <)
#define DCHECK_GE(val1, val2) CHECK_OP(val1, val2, >=)
#define DCHECK_GT(val1, val2) CHECK_OP(val1, val2, >)
#define DCHECK_NEAR(val1, val2, margin) CHECK_NEAR(val1, val2, margin)
#else
#define DCHECK_EQ(val1, val2)                                                                                          \
    if (false)                                                                                                         \
    CHECK_OP(val1, val2, ==)
#define DCHECK_NE(val1, val2)                                                                                          \
    if (false)                                                                                                         \
    CHECK_OP(val1, val2, !=)
#define DCHECK_LE(val1, val2)                                                                                          \
    if (false)                                                                                                         \
    CHECK_OP(val1, val2, <=)
#define DCHECK_LT(val1, val2)                                                                                          \
    if (false)                                                                                                         \
    CHECK_OP(val1, val2, <)
#define DCHECK_GE(val1, val2)                                                                                          \
    if (false)                                                                                                         \
    CHECK_OP(val1, val2, >=)
#define DCHECK_GT(val1, val2)                                                                                          \
    if (false)                                                                                                         \
    CHECK_OP(val1, val2, >)
#define DCHECK_NEAR(val1, val2, margin)                                                                                \
    if (false)                                                                                                         \
    CHECK_NEAR(val1, val2, margin)
#endif
template <typename T> T &CheckNotNullCommon(const char *file, int line, const char *names, T &t)
{
    if (t == NULL)
    {
        LogMessageFatal(file, line, std::string(names));
    }
    return t;
}

template <typename T> T *CheckNotNull(const char *file, int line, const char *names, T *t)
{
    return CheckNotNullCommon(file, line, names, t);
}

template <typename T> T &CheckNotNull(const char *file, int line, const char *names, T &t)
{
    return CheckNotNullCommon(file, line, names, t);
}
#define CHECK_NOTNULL(val) CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

#ifndef NDEBUG
#define DCHECK_NOTNULL(val) CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))
#else
#define DCHECK_NOTNULL(val)                                                                                            \
    if (false)                                                                                                         \
    CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))
#endif
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define DEXEC(fn)                                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        DLOG(INFO) << "[EXEC " << #fn << " START]";                                                                    \
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();                                \
        fn;                                                                                                            \
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();                                  \
        DLOG(INFO) << "[EXEC " << #fn << " FINISHED in "                                                               \
                   << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " ms]";            \
    } while (0);
#define DTRACE DLOG(INFO) << "of [" << __func__ << "]";
#endif

#ifndef _TIMES_H_
#define _TIMES_H_
#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <ratio>
#include <sstream>
#include <string>
namespace indem
{
namespace times
{
using system_clock = std::chrono::system_clock;
using nanoseconds = std::chrono::nanoseconds;
using microseconds = std::chrono::microseconds;
using milliseconds = std::chrono::milliseconds;
using seconds = std::chrono::seconds;
using minutes = std::chrono::minutes;
using hours = std::chrono::hours;
using days = std::chrono::duration<std::int64_t, std::ratio<86400>>;
template <typename Duration> inline Duration to_duration(const std::int64_t &t)
{
    return Duration{t};
}

template <typename Duration> inline system_clock::time_point to_time_point(const Duration &d)
{
    return system_clock::time_point(d);
}

template <typename Duration> inline system_clock::time_point to_time_point(const std::int64_t &t)
{
    return system_clock::time_point(Duration{t});
}

inline system_clock::time_point to_time_point(std::tm *tm)
{
    return system_clock::from_time_t(std::mktime(tm));
}

inline struct std::tm *to_local_tm(const system_clock::time_point &t)
{
    auto t_c = system_clock::to_time_t(t);
    return std::localtime(&t_c);
}

inline struct std::tm *to_utc_tm(const system_clock::time_point &t)
{
    auto t_c = system_clock::to_time_t(t);
    return std::gmtime(&t_c);
}

template <typename FromDuration, typename ToDuration> inline ToDuration cast(const FromDuration &d)
{
    return std::chrono::duration_cast<ToDuration>(d);
}

template <typename Duration> inline Duration cast(const system_clock::duration &d)
{
    return cast<system_clock::duration, Duration>(d);
}

template <typename FromDuration, typename ToDuration> inline std::int64_t cast(const std::int64_t &t)
{
    return cast<FromDuration, ToDuration>(FromDuration{t}).count();
}

template <typename Duration> inline system_clock::time_point cast(const system_clock::time_point &d)
{
    return std::chrono::time_point_cast<Duration>(d);
}

template <typename Duration> inline system_clock::duration cast_mod(const system_clock::time_point &t)
{
    return t - cast<Duration>(t);
}
template <typename FromDuration, typename ToDuration> inline std::int64_t count(const FromDuration &d)
{
    return cast<FromDuration, ToDuration>(d).count();
}

template <typename Duration> inline std::int64_t count(const system_clock::duration &d)
{
    return cast<Duration>(d).count();
}

inline std::tm *day_beg(std::tm *tm)
{
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    return tm;
}

inline std::tm *day_end(std::tm *tm)
{
    tm->tm_hour = 23;
    tm->tm_min = 59;
    tm->tm_sec = 59;
    return tm;
}

inline system_clock::time_point day_beg(const system_clock::time_point &t)
{
    return cast<days>(t);
}

inline system_clock::time_point day_end(const system_clock::time_point &t)
{
    return day_beg(t) + days(1) - system_clock::duration(1);
}

inline system_clock::duration day_time(const system_clock::time_point &t)
{
    return cast_mod<days>(t);
}

template <typename Duration>
inline std::int64_t between(const system_clock::time_point &t1, const system_clock::time_point &t2)
{
    return count<Duration>(t2 - t1);
}

inline std::int64_t between_days(const system_clock::time_point &t1, const system_clock::time_point &t2)
{
    return between<days>(day_beg(t1), day_beg(t2));
}

template <typename Duration> inline std::int64_t between_days(const std::int64_t &t1, const std::int64_t &t2)
{
    return between_days(to_time_point<Duration>(t1), to_time_point<Duration>(t2));
}

inline system_clock::time_point epoch()
{
    return system_clock::time_point(system_clock::duration{0});
}

template <typename Duration> inline std::int64_t since_epoch(const system_clock::time_point &t)
{
    return count<Duration>(t.time_since_epoch());
}

inline system_clock::time_point now()
{
    return system_clock::now();
}

template <typename Duration> inline std::int64_t now()
{
    return since_epoch<Duration>(now());
}

inline std::string to_string(const system_clock::time_point &t, const std::tm *tm, const char *fmt = "%F %T",
                             std::int32_t precision = 6)
{
    std::stringstream ss;
#if defined(INDEMIND_OS_LINUX)
    char foo[20];
    strftime(foo, sizeof(foo), fmt, tm);
    ss << foo;
#else
    ss << std::put_time(tm, fmt);
#endif
    if (precision > 0)
    {
        if (precision > 6)
            precision = 6;
        ss << '.' << std::setfill('0') << std::setw(precision)
           << static_cast<std::int32_t>(count<microseconds>(cast_mod<seconds>(t)) / std::pow(10, 6 - precision));
    }
    return ss.str();
}

inline std::string to_local_string(const system_clock::time_point &t, const char *fmt = "%F %T",
                                   const std::int32_t &precision = 6)
{
    return to_string(t, to_local_tm(t), fmt, precision);
}

inline std::string to_utc_string(const system_clock::time_point &t, const char *fmt = "%F %T",
                                 const std::int32_t &precision = 6)
{
    return to_string(t, to_utc_tm(t), fmt, precision);
}

};

};

#endif

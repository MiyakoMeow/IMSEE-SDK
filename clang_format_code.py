import subprocess
import os

file_paths = []

for (root, dirs, files) in os.walk("."):
    # 跳过build目录
    if len(root) > 7:
        if root[2:7] == "build" or root[2:].startswith("build-"):
            continue

    for file_name in files:
        # 遍历该目录下文件
        if not (file_name.endswith(".c") or file_name.endswith(".h") \
                or file_name.endswith(".cpp") or file_name.endswith(".hpp")):
            continue

        # 添加文件至format名单
        code_file_path = f"{root}/{file_name}"
        # print("Root:", root, "File:", file_name)
        file_paths.append(code_file_path)

# 实际执行clang-format
for file_path in file_paths:
    command = f"clang-format -i {file_path}"
    print(command)
    subprocess.run(command, shell=True)

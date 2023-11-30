import re
import os

def remove_invisible_chars(s):
    """移除所有不可见字符，除\n外"""
    str = ''
    for x in s:
        if x is not '\n' and not x.isprintable():
            str += ''
        else:
            str += x
    return str


text = 'This is a test string with \t invisible characters.'
clean_text = remove_invisible_chars(text)
print(clean_text)

file_paths = []

for (root, dirs, files) in os.walk("."):
    # print("Walking:", root, "Files:", files)
    for file_name in files:
        if not (file_name.endswith(".c") or file_name.endswith(".h") \
                or file_name.endswith(".cpp") or file_name.endswith(".hpp")):
            continue
        code_file_path = f"{root}/{file_name}"
        file_paths.append(code_file_path)

for file_path in file_paths:
    print("Cleaning:", file_path)
    # Read
    code = ""
    with open(file_path, "rb") as f:
        code = f.read().decode("utf-8", "replace")
    # Clean
    clean_code = remove_invisible_chars(code)
    # Write
    with open(file_path, "wb") as f:
        f.write(clean_code.encode("utf-8"))
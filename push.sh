#!/bin/bash

# 获取提交信息，如果没有提供，使用默认信息
message=${1:-"Auto commit"}

# 执行 Git 命令
git add .
git commit -m "$message"
git push origin main

# 输出提示信息
echo "Changes pushed to remote repository"

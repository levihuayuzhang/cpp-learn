#!/bin/bash
set -e

# 备份原始 sources.list
cp /etc/apt/sources.list /etc/apt/sources.list.bak

# 用我们准备好的清华源覆盖
cp .devcontainer/sources-list-ubuntu-legacy.txt /etc/apt/sources.list

# 更新缓存
apt-get update
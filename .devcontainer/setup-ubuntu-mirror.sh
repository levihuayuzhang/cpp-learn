#!/bin/bash
set -e

cp /etc/apt/sources.list /etc/apt/sources.list.bak

cp .devcontainer/sources-list-ubuntu-legacy.txt /etc/apt/sources.list

apt-get update
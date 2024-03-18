#! /bin/bash

export VULKAN_SDK="/home/shaharsandhaus/1.3.204.1/x86_64"
export PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

python3 -m build

#bear -- python3 setup.py build_ext --inplace

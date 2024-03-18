#! /bin/bash

export VULKAN_SDK="/Users/shaharsandhaus/VulkanSDK/1.3.275.0/macOS"
export PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

python3 -m build

#bear -- python3 setup.py build_ext --inplace

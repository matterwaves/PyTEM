#! /bin/bash

wheel_file=$(find dist -type f -name "*.whl")

# Check if a .whl file was found
if [ -z "$wheel_file" ]; then
    echo "No .whl file found in the dist/ directory."
    exit 1
else
    # Install the .whl file using pip
    echo "Installing $wheel_file..."
    pip install "$wheel_file" --force-reinstall
    echo "Installation completed."
fi
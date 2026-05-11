#!/bin/bash
# Simple script to run the Publisher

# Get the absolute path to the directory containing this script
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define the path to the executable
EXECUTABLE="$DIR/build/publisher"

# Check if the executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Executable not found at $EXECUTABLE"
    echo "Please build the project first by running ./build.sh"
    exit 1
fi

# Run the application
echo "Starting Publisher..."
"$EXECUTABLE"

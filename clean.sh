#!/bin/bash
set -e

echo "Cleaning test_fastdds..."

if [ -d "build" ]; then
    rm -rf build
    echo "Removed 'build' directory."
else
    echo "No 'build' directory found."
fi

# Also remove the copied compile_commands.json from root
if [ -f "compile_commands.json" ] || [ -L "compile_commands.json" ]; then
    rm compile_commands.json
    echo "Removed 'compile_commands.json' from root."
fi

echo "✅ Clean complete!"

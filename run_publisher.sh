#!/bin/bash
if [ ! -f "build/publisher" ]; then
    echo "Publisher not found. Please build the project first."
    exit 1
fi
./build/publisher

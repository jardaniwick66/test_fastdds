#!/bin/bash
if [ ! -f "build/subscriber" ]; then
    echo "Subscriber not found. Please build the project first."
    exit 1
fi
./build/subscriber

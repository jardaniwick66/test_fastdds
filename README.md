# Fast DDS Test Project

A simple C++ Publisher/Subscriber application using **eProsima Fast DDS**. This project demonstrates the basic usage of DDS entities, IDL generation, and CMake integration.

## Project Structure

```text
.
├── CMakeLists.txt          # Build configuration
├── build.sh                # Build the project
├── clean.sh                # Remove build artifacts
├── run_publisher.sh        # Run the publisher
├── run_subscriber.sh       # Run the subscriber
├── idl/
│   ├── testmsg.idl         # Data type definition
│   └── genIdl.sh           # Script to manually regenerate IDL sources
├── src/
│   ├── publisherMain.cpp   # Publisher implementation
│   ├── subscriberMain.cpp  # Subscriber implementation
│   └── idl/                # Generated sources from IDL
└── .vscode/                # IDE configuration for IntelliSense
```

## Prerequisites

- **Fast DDS**: Installed in `~/Fast-DDS/install`.
- **CMake**: Version 3.16 or higher.
- **C++ Compiler**: Supporting C++17.

### Fast DDS Compilation Note for macOS

If you are compiling the **Fast DDS library itself** from source on macOS, you may encounter numerous warnings or errors related to nullability. It is recommended to add the following flags to the Fast DDS `CMakeLists.txt` or pass them during configuration:

```cmake
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-nonnull -Wpedantic -Wextra -Wno-unknown-pragmas -Wno-error=deprecated-declarations")
```

The `-Wno-nonnull` flag is particularly essential on macOS to handle system header compatibility issues during the Fast DDS build process.

## Building and Cleaning

### Build
To configure and build the project:
```bash
./build.sh
```

### Clean
To remove the `build/` directory and generated compile commands:
```bash
./clean.sh
```

## Running the Application

For a complete test, open two terminal windows.

### 1. Start the Subscriber
The subscriber will wait for a publisher to connect and then print received messages.

```bash
./run_subscriber.sh
```

### 2. Start the Publisher
The publisher will wait until it discovers the subscriber, then send 10 messages.

```bash
./run_publisher.sh
```

## Development

### Modifying the Data Type
If you change `idl/testmsg.idl`, you should regenerate the source files:

```bash
cd idl
./genIdl.sh
```

### IDE Support
This project exports `compile_commands.json` to the root directory. Most modern IDEs (VS Code, CLion, etc.) will use this file to provide accurate IntelliSense and autocomplete for Fast DDS types.

## Tested On
This project has been tested on:
- **OS**: macOS 26.3 (Architecture: arm64)
- **Compiler**: Apple Clang 21.0.0
- **Fast DDS Version**: 3.6.1 (Installed in `~/Fast-DDS/install`)

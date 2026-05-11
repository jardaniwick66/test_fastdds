cmake_minimum_required(VERSION 3.16)
project(test_fastdds)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Fast DDS installation path
set(FAST_DDS_INSTALL_DIR "$ENV{HOME}/Fast-DDS/install")
list(APPEND CMAKE_PREFIX_PATH "${FAST_DDS_INSTALL_DIR}")

# Find Fast DDS and its dependencies
find_package(fastdds REQUIRED)
find_package(foonathan_memory REQUIRED)
find_package(fastcdr REQUIRED)

# Include directories
include_directories(
    src
    src/idl
)

# IDL Generation
set(IDL_FILE "${CMAKE_CURRENT_SOURCE_DIR}/idl/testmsg.idl")
set(IDL_GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/idl")
set(FASTDDSGEN "$ENV{HOME}/Fast-DDS/src/fastddsgen/scripts/fastddsgen")

# Note: In a real project, you might want to automate this more robustly,
# but here we'll just ensure the files are included in the build.

# Generated files from IDL
set(IDL_SOURCES
    src/idl/testmsgPubSubTypes.cxx
    src/idl/testmsgTypeObjectSupport.cxx
)

# Publisher executable
add_executable(publisher
    src/publisherMain.cpp
    ${IDL_SOURCES}
)
target_link_libraries(publisher fastdds)

# Subscriber executable
add_executable(subscriber
    src/subscriberMain.cpp
    ${IDL_SOURCES}
)
target_link_libraries(subscriber fastdds)

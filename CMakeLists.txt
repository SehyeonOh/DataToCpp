# Set minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(data2cpp VERSION 0.1.0)

# Debug build configuration
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Arrow library using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(ARROW REQUIRED IMPORTED_TARGET arrow parquet)

# Set library source files
add_library(data2cpp
    data2cpp/parquet/parquet2cpp.cc
    data2cpp/binary/binary2cpp.cc
    data2cpp/vecs/vecs2cpp.cc
)

# Set header files and library links
target_include_directories(data2cpp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link Arrow library
target_link_libraries(data2cpp
    PUBLIC
        PkgConfig::ARROW
)

# Add examples directory
add_subdirectory(examples) 
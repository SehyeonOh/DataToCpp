# Set minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set example project name
project(parquet2cpp_examples)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# data2cpp_test
add_executable(parquet2cpp_test parquet2cpp_test.cc)
add_executable(binary2cpp_test binary2cpp_test.cc)
add_executable(vecs2cpp_test vecs2cpp_test.cc)

# Link data2cpp library (using target already defined in parent CMakeLists.txt)
target_link_libraries(parquet2cpp_test PRIVATE data2cpp)
target_link_libraries(binary2cpp_test PRIVATE data2cpp)
target_link_libraries(vecs2cpp_test PRIVATE data2cpp)

# Set header file paths
target_include_directories(parquet2cpp_test PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(binary2cpp_test PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(vecs2cpp_test PRIVATE ${CMAKE_SOURCE_DIR}) 

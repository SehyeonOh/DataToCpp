# 최소 CMake 버전 설정
cmake_minimum_required(VERSION 3.10)

# 예제 프로젝트 이름 설정
project(parquet2cpp_examples)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# parquet2cpp_test 실행 파일 생성
add_executable(parquet2cpp_test parquet2cpp_test.cc)

# data2cpp 라이브러리 링크 (상위 CMakeLists.txt에서 이미 정의된 타겟 사용)
target_link_libraries(parquet2cpp_test PRIVATE data2cpp)

# 헤더 파일 경로 설정
target_include_directories(parquet2cpp_test PRIVATE ${CMAKE_SOURCE_DIR}) 
# 최소 CMake 버전 설정
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름 및 버전 설정
project(data2cpp VERSION 0.1.0)

# 디버그 빌드 설정
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# pkg-config를 사용하여 Arrow 라이브러리 찾기
find_package(PkgConfig REQUIRED)
pkg_check_modules(ARROW REQUIRED IMPORTED_TARGET arrow parquet)

# 라이브러리 소스 파일 설정
add_library(data2cpp
    data2cpp/parquet/parquet2cpp.cc
)

# 헤더 파일 및 라이브러리 링크 설정
target_include_directories(data2cpp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Arrow 라이브러리 링크
target_link_libraries(data2cpp
    PUBLIC
        PkgConfig::ARROW
)

# examples 디렉토리 추가
add_subdirectory(examples) 
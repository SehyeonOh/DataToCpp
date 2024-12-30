# Data2Cpp - Parquet2Cpp

Parquet2Cpp는 Parquet 파일에서 리스트 타입 컬럼의 데이터를 C++ 프로그램에서 효율적으로 읽을 수 있게 해주는 라이브러리입니다.

## 기능
- Parquet 파일에서 리스트 타입(LIST, FIXED_SIZE_LIST) 컬럼 읽기
- Arrow Table을 통한 데이터 처리
- 원시 데이터(raw data) 접근 지원

## 요구사항
- C++17 이상
- Apache Arrow
- Apache Parquet
- CMake 3.10 이상

## 빌드 방법

    mkdir build
    cd build
    cmake ..
    make

## 사용 예제

    #include "data2cpp/parquet/parquet2cpp.hh"

    // Parquet 파일에서 데이터 읽기
    data2cpp::Parquet2Cpp reader("input.parquet", "column_name");

    // 데이터 정보 확인
    std::cout << reader.ToString() << std::endl;

    // 데이터 접근
    const uint8_t* raw_data = reader.GetRawColumnData();
    int64_t row_count = reader.GetRowCount();
    int64_t width = reader.GetWidth();
    int64_t element_size = reader.GetElementSize();
    std::string element_type = reader.GetElementType();

## 예제 프로그램 실행

    # examples/parquet2cpp_test 실행
    ./build/examples/parquet2cpp_test <parquet_file_path> <column_name>

## 주요 메서드

- `GetRawColumnData()`: 원시 데이터 포인터 반환
- `GetRowCount()`: 행 수 반환
- `GetWidth()`: 리스트의 너비(요소 개수) 반환
- `GetElementSize()`: 각 요소의 크기(바이트) 반환
- `GetElementType()`: 요소의 데이터 타입 문자열 반환
- `ToString()`: 데이터 정보를 문자열로 반환

## 디버깅

디버그 모드로 빌드되어 있어 gdb를 통한 소스 레벨 디버깅이 가능합니다:

    gdb ./build/examples/parquet2cpp_test

## 지원하는 데이터 타입
- LIST
- FIXED_SIZE_LIST

각 리스트는 기본 데이터 타입(정수, 실수 등)의 요소들을 포함할 수 있습니다.

## 라이선스
[라이선스 정보 추가 필요] 
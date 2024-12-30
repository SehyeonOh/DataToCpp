# Data2Cpp

데이터 파일을 C++ 프로그램에서 쉽게 읽을 수 있게 해주는 라이브러리입니다.

## 지원하는 파일 형식
- Parquet
- Binary
- Vecs (ivecs/fvecs)

## 사용 예시

### Parquet 파일
    #include "data2cpp/parquet/parquet2cpp.hh"

    data2cpp::Parquet2Cpp data("example.parquet");
    const uint8_t* raw_data = data.GetRawData();

### Binary 파일
    #include "data2cpp/binary/binary2cpp.hh"

    data2cpp::Binary2Cpp data("example.bin", width, element_size, skip_bytes);
    const uint8_t* raw_data = data.GetRawData();

### Vecs 파일 (ivecs/fvecs)
    #include "data2cpp/vecs/vecs2cpp.hh"

    // ivecs 파일의 경우
    data2cpp::Vecs2Cpp data("example.ivecs", "int32");
    const uint8_t* raw_data = data.GetRawData();

    // fvecs 파일의 경우
    data2cpp::Vecs2Cpp data("example.fvecs", "float");
    const uint8_t* raw_data = data.GetRawData();

## 테스트 프로그램 사용법

### Parquet 테스트
    ./parquet2cpp_test example.parquet

### Binary 테스트
    ./binary2cpp_test example.bin width element_size skip_bytes

### Vecs 테스트
    ./vecs2cpp_test example.ivecs int32
    ./vecs2cpp_test example.fvecs float

## 빌드 방법
    mkdir build
    cd build
    cmake ..
    make 
# Data2Cpp

A library for easily reading data files in C++ programs.

## Supported File Formats
- Parquet
- Binary
- Vecs (ivecs/fvecs)

## Usage Examples

### Parquet File
    #include "data2cpp/parquet/parquet2cpp.hh"

    data2cpp::Parquet2Cpp data("example.parquet");
    const uint8_t* raw_data = data.GetRawData();

### Binary File
    #include "data2cpp/binary/binary2cpp.hh"

    data2cpp::Binary2Cpp data("example.bin", width, element_size, skip_bytes);
    const uint8_t* raw_data = data.GetRawData();

### Vecs File (ivecs/fvecs)
    #include "data2cpp/vecs/vecs2cpp.hh"

    // For ivecs file
    data2cpp::Vecs2Cpp data("example.ivecs", "int32");
    const uint8_t* raw_data = data.GetRawData();

    // For fvecs file
    data2cpp::Vecs2Cpp data("example.fvecs", "float");
    const uint8_t* raw_data = data.GetRawData();

## Test Program Usage

### Parquet Test
    ./parquet2cpp_test example.parquet

### Binary Test
    ./binary2cpp_test example.bin width element_size skip_bytes

### Vecs Test
    ./vecs2cpp_test example.ivecs int32
    ./vecs2cpp_test example.fvecs float

## Build Instructions
    mkdir build
    cd build
    cmake ..
    make 
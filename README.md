# Data2Cpp

A library for easily reading data files in C++ programs.

## Supported File Formats
- Parquet
- Binary
- Vecs (ivecs/fvecs)

## Usage Examples

### Parquet File
    #include "data2cpp/parquet/parquet2cpp.hh"

    // Load data from parquet file(s)
    // Single file
    auto data = std::make_shared<data2cpp::Parquet2Cpp>(std::vector<std::string>{"example.parquet"}, "column_name");

    // Multiple files (data will be concatenated in order)
    auto data_multi = std::make_shared<data2cpp::Parquet2Cpp>(
        std::vector<std::string>{"example1.parquet", "example2.parquet", "example3.parquet"}, 
        "column_name"
    );

    // Option 1: Get raw data if you need to handle the original data type
    const uint8_t* raw_data = data->GetRawData();

    // Option 2: Get float array if you need normalized float array
    // (automatically converts both float and double data to float array)
    const float* float_data = data->GetFloatData();

    // Get data information
    int64_t rows = data->GetRowCount();
    int64_t width = data->GetWidth();
    int64_t element_size = data->GetElementSize();

### Binary File
    #include "data2cpp/binary/binary2cpp.hh"

    data2cpp::Binary2Cpp data("example.bin", width, element_size, skip_bytes);
    // Option 1: Get raw data if you need to handle the original data type
    const uint8_t* raw_data = data.GetRawData();
    // Option 2: Get float array if you need normalized float array
    const float* float_data = data.GetFloatData();

    // Get data information
    int64_t rows = data->GetRowCount();
    int64_t width = data->GetWidth();
    int64_t element_size = data->GetElementSize();

### Vecs File (ivecs/fvecs)
    #include "data2cpp/vecs/vecs2cpp.hh"

    // For ivecs file
    data2cpp::Vecs2Cpp data("example.ivecs", "int32");
    const uint8_t* raw_data = data.GetRawData();

    // For fvecs file
    data2cpp::Vecs2Cpp data("example.fvecs", "float");
    const float* float_data = data.GetFloatData();

    // Get data information
    int64_t rows = data->GetRowCount();
    int64_t width = data->GetWidth();
    int64_t element_size = data->GetElementSize();

## Test Program Usage

### Parquet Test
    ./parquet2cpp_test example.parquet column_name

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

## Supported Data Types
- List<float>
- List<double>
- LargeList<float>
- LargeList<double>
- FixedSizeList<float>
- FixedSizeList<double> 
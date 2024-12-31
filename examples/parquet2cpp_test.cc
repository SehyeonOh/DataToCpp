#include <iostream>
#include "data2cpp/parquet/parquet2cpp.hh"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <parquet_file_path> <column_name>" << std::endl;
        return 1;
    }

    try {
        // Parquet file path and column name
        std::string file_path = argv[1];
        std::string column_name = argv[2];
        
        // Create Parquet2Cpp object
        data2cpp::Parquet2Cpp parquet_reader(file_path, column_name);
        
        // Print data information
        std::cout << "Parquet File Information:" << std::endl;
        std::cout << parquet_reader.ToString(true) << std::endl;

        RAW_DATA_DEFAULT(auto data, float, &parquet_reader);
        std::cout << "data: " << std::endl;
        for (int64_t i = 0; i < parquet_reader.GetWidth(); i++) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
        RAW_DATA(auto data10, float, &parquet_reader, 10);
        std::cout << "data10: " << std::endl;
        for (int64_t i = 0; i < parquet_reader.GetWidth(); i++) {
            std::cout << data10[i] << " ";
        }
        std::cout << std::endl;
        auto float_data = parquet_reader.GetFloatData();
        std::cout << "float_data: " << std::endl;
        for (int64_t i = 0; i < parquet_reader.GetWidth(); i++) {
            std::cout << float_data[i] << " ";
        }
        std::cout << std::endl;
        auto float_data10 = parquet_reader.GetFloatData(10);
        std::cout << "float_data10: " << std::endl;
        for (int64_t i = 0; i < parquet_reader.GetWidth(); i++) {
            std::cout << float_data10[i] << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 
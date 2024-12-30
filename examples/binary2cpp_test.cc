#include <cstdio>
#include <iostream>
#include "data2cpp/binary/binary2cpp.hh"

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <binary_file> <width> <element_size> <skip_bytes>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    const int64_t width = std::stoll(argv[2]);
    const int64_t element_size = std::stoll(argv[3]);
    const int64_t skip_bytes = std::stoll(argv[4]);

    try {
        data2cpp::Binary2Cpp data(filename, width, element_size, skip_bytes);
        std::cout << data.ToString(true) << std::endl;

        RAW_DATA(auto raw, uint64_t, &data, 0);
        for (int i = 0; i < width; i++) {
            printf("%ld ", (int64_t)raw[i]);
        }
        printf("\n");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 
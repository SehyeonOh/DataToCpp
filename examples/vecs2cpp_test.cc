#include <cstdio>
#include <iostream>
#include "data2cpp/vecs/vecs2cpp.hh"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <vecs_file> <element_type>" << std::endl;
        std::cerr << "element_type: int32 or float" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    const std::string element_type = argv[2];

    try {
        data2cpp::Vecs2Cpp data(filename, element_type);
        std::cout << data.ToString() << std::endl;

        RAW_DATA(auto raw, uint8_t, &data, 0);
        for (int i = 0; i < data.GetWidth(); i++) {
            printf("%ld ", (int64_t)raw[i]);
        }
        printf("\n");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 
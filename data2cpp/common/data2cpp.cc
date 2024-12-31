#include "data2cpp.hh"
#include <stdexcept>
#include <iostream>

namespace data2cpp {
    const float *Data2Cpp::GetFloatData(int64_t row_index) {
        return float_data_ + row_index * GetWidth();
    }

    void Data2Cpp::SetFloatData(const uint8_t *data) {
        if (GetElementSize() == sizeof(double)) {
            auto double_data = reinterpret_cast<const double *>(data);
            float* temp_data = new float[GetRowCount() * GetWidth()];
            for (int64_t i = 0; i < GetRowCount() * GetWidth(); i++) {
                temp_data[i] = static_cast<float>(double_data[i]);
            }
            std::cout << "double data is converted to float data" << std::endl;
            float_data_ = temp_data;
            owns_data_ = true;
        } else if (GetElementSize() == sizeof(float)) {
            float_data_ = reinterpret_cast<const float*>(data);
            owns_data_ = false;
        } else {
            throw std::runtime_error("Unsupported element size: " + std::to_string(GetElementSize()));
        }
    }

    Data2Cpp::~Data2Cpp() {
        if (owns_data_ && float_data_) {
            delete[] float_data_;
        }
    }
}
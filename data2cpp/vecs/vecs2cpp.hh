#pragma once

#include "data2cpp/common/data2cpp.hh"
#include <string>
#include <vector>

namespace data2cpp {

class Vecs2Cpp : public Data2Cpp {
    public:
        Vecs2Cpp(const std::string &source_file, const std::string &element_type);
        
        const uint8_t *GetRawData(int64_t row_index = 0) override;
        int64_t GetRowCount() override;
        int64_t GetWidth() override;
        int64_t GetElementSize() override;

        std::string GetElementType() const { return element_type_; }
        std::string ToString(bool verbose = false) override;

    private:
        std::vector<uint8_t> raw_data_;
        int64_t width_;            // dimension of each vector
        int64_t row_count_;        // number of vectors
        std::string element_type_; // "int32" or "float"
        int64_t element_size_;     // 4 bytes for both int32 and float
};

}  // namespace data2cpp 
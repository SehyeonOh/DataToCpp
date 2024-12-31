#pragma once
#include <cstdint>
#include <vector>

#include "data2cpp/common/data2cpp.hh"

namespace data2cpp {
    class Binary2Cpp : public Data2Cpp {
    public:
        Binary2Cpp(const std::string &source_file, int64_t width, int64_t element_size, int64_t skip_bytes = 0);
        ~Binary2Cpp() = default;

        const uint8_t *GetRawData(int64_t row_index = 0) override;
        int64_t GetRowCount() override;
        int64_t GetWidth() override;
        int64_t GetElementSize() override;

        std::string ToString(bool verbose) override;
    private:
        std::vector<uint8_t> raw_data_;
        int64_t width_;
        int64_t element_size_;
        int64_t row_count_;
        int64_t skip_bytes_;
    };
}
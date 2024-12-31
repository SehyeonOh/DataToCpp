#pragma once
#include <string>

namespace data2cpp {
    class Data2Cpp {
    public:
        Data2Cpp(std::string source_file) : source_file_(source_file) {}
        Data2Cpp(std::string source_file, std::string column_name) : source_file_(source_file), column_name_(column_name) {}
        ~Data2Cpp();
        // only support fixed sized list array for now
        virtual const uint8_t *GetRawData(int64_t row_index = 0) = 0;
        virtual int64_t GetRowCount() = 0;
        virtual int64_t GetWidth() = 0;
        virtual int64_t GetElementSize() = 0;

        virtual std::string ToString(bool verbose) = 0;

        const float *GetFloatData(int64_t row_index = 0);
        void SetFloatData(const uint8_t *data);
    protected:
        std::string source_file_ = "";
        std::string column_name_ = "";

        const float* float_data_;
        bool owns_data_ = false;
    };

    // Data access macro
    #define RAW_DATA(lhs, type, class_ptr, row_index) \
        lhs = reinterpret_cast<const type*>((class_ptr)->GetRawData(row_index))

    #define RAW_DATA_DEFAULT(lhs, type, class_ptr) \
        RAW_DATA(lhs, type, class_ptr, 0)
}
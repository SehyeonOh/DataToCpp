#pragma once
#include <string>

namespace data2cpp {
    class Data2Cpp {
    public:
        Data2Cpp(std::string source_file, std::string column_name = "") : source_file_(source_file), column_name_(column_name) {}
        virtual ~Data2Cpp() = default;

        // only support fixed sized list array for now
        virtual const uint8_t *GetRawColumnData() = 0;
        virtual int64_t GetRowCount() = 0;
        virtual int64_t GetWidth() = 0;
        virtual int64_t GetElementSize() = 0;
        virtual std::string GetElementType() = 0;

        virtual std::string ToString(bool verbose = false) = 0;
    protected:
        std::string source_file_;
        std::string column_name_;
    };
}
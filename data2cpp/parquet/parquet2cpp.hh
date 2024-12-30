#pragma once
#include <string>
#include <arrow/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>
#include <arrow/ipc/util.h>
#include <arrow/ipc/reader.h>
#include <arrow/ipc/writer.h>
#include <arrow/record_batch.h>

#include "data2cpp/common/data2cpp.hh"

namespace data2cpp {
    class Parquet2Cpp : public Data2Cpp {
    public:
        Parquet2Cpp(std::string source_file, std::string column_name);
        ~Parquet2Cpp() = default;

        const uint8_t *GetRawData(int64_t row_index = 0) override;
        int64_t GetRowCount() override;
        int64_t GetWidth() override;
        int64_t GetElementSize() override;
        std::string GetElementType();

        std::string ToString(bool verbose) override;

    private:
        std::shared_ptr<arrow::ipc::RecordBatchReader> reader_;
        std::shared_ptr<arrow::Table> table_;
        std::shared_ptr<arrow::RecordBatch> batch_;

        // for single column of column_name_
        std::shared_ptr<arrow::Array> array_;
        const uint8_t *raw_data_;
        int64_t row_count_;
        int64_t width_;
        int64_t element_size_;
        std::string element_type_;
    };
}
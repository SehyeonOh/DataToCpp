#include <iostream>
#include <sstream>
#include <arrow/io/api.h>
#include <arrow/io/file.h>
#include <arrow/ipc/api.h>
#include <arrow/table.h>
#include <arrow/array.h>
#include <arrow/record_batch.h>
#include <arrow/memory_pool.h>
#include <parquet/arrow/reader.h>

#include "data2cpp/parquet/parquet2cpp.hh"

namespace data2cpp {
    std::shared_ptr<arrow::Table> ParquetToArrowTable(const std::string &file_path)
    {
        // Open the Parquet file
        auto infile_result = arrow::io::ReadableFile::Open(file_path);
        if (!infile_result.ok()) {
            throw std::runtime_error("Failed to open Parquet file: " + infile_result.status().ToString());
        }
        auto infile = infile_result.ValueUnsafe();

        // Create Parquet reader
        std::unique_ptr<parquet::arrow::FileReader> parquet_reader;
        auto status = parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &parquet_reader);
        if (!status.ok())
        {
            throw std::runtime_error("Failed to create Parquet reader: " + status.ToString());
        }

        // Read the Parquet file into an Arrow Table
        std::shared_ptr<arrow::Table> table;
        status = parquet_reader->ReadTable(&table);
        if (!status.ok())
        {
            throw std::runtime_error("Failed to read Parquet file into Arrow Table: " + status.ToString());
        }

        return table;
    }

    std::shared_ptr<arrow::RecordBatch> ArrowTableToSingleRecordBatch(const std::shared_ptr<arrow::Table> &table)
    {
        if (table->num_columns() == 0)
        {
            throw std::runtime_error("Arrow Table has no columns.");
        }

        // If the table has a single chunk, directly extract as a RecordBatch
        if (table->column(0)->num_chunks() == 1)
        {
            arrow::TableBatchReader reader(*table);
            std::shared_ptr<arrow::RecordBatch> record_batch;
            auto status = reader.ReadNext(&record_batch);
            if (!status.ok() || !record_batch)
            {
                throw std::runtime_error("Failed to read RecordBatch: " + status.ToString());
            }
            return record_batch;
        }

        // Combine chunks into a single batch
        auto combined_table_result = table->CombineChunks(arrow::default_memory_pool());
        if (!combined_table_result.ok())
        {
            throw std::runtime_error("Failed to combine table chunks: " + combined_table_result.status().ToString());
        }
        auto combined_table = combined_table_result.ValueOrDie();

        // Convert the combined table into a RecordBatch
        arrow::TableBatchReader reader(*combined_table);
        std::shared_ptr<arrow::RecordBatch> record_batch;
        auto status = reader.ReadNext(&record_batch);
        if (!status.ok() || !record_batch)
        {
            throw std::runtime_error("Failed to create RecordBatch from combined table: " + status.ToString());
        }

        return record_batch;
    }

    Parquet2Cpp::Parquet2Cpp(std::string source_file, std::string column_name) : Data2Cpp(source_file, column_name)
    {
        table_ = ParquetToArrowTable(source_file);
        batch_ = ArrowTableToSingleRecordBatch(table_);
        array_ = batch_->GetColumnByName(column_name_);
        
        // List 타입인지 확인
        if (array_->type()->id() == arrow::Type::LIST) {
            auto list_array = std::static_pointer_cast<arrow::ListArray>(array_);
            auto values = list_array->values();
            raw_data_ = values->data()->GetValues<uint8_t>(0, 0);
            
            row_count_ = array_->length();
            width_ = list_array->value_length(0);
            element_size_ = values->type()->bit_width() / 8;
            element_type_ = values->type()->ToString();
        } 
        else if (array_->type()->id() == arrow::Type::FIXED_SIZE_LIST) {
            auto fixed_list_array = std::static_pointer_cast<arrow::FixedSizeListArray>(array_);
            auto values = fixed_list_array->values();
            raw_data_ = values->data()->GetValues<uint8_t>(0, 0);
            
            row_count_ = array_->length();
            width_ = fixed_list_array->list_type()->list_size();
            element_size_ = values->type()->bit_width() / 8;
            element_type_ = values->type()->ToString();
        } 
        else {
            throw std::runtime_error("Column is not a list type array");
        }

        std::cout << "row_count_: " << row_count_ << std::endl;
        std::cout << "width_: " << width_ << std::endl;
        std::cout << "element_size_: " << element_size_ << std::endl;
        std::cout << "element_type_: " << element_type_ << std::endl;
    }

    std::string Parquet2Cpp::ToString(bool verbose)
    {
        std::stringstream ss;
        ss << "Parquet2Cpp" << std::endl;
        ss << "source_file_: " << source_file_ << std::endl;
        ss << "column_name_: " << column_name_ << std::endl;
        ss << "row_count_: " << row_count_ << std::endl;
        ss << "width_: " << width_ << std::endl;
        ss << "data_type_: " << array_->type()->ToString() << std::endl;
        ss << "element_size_: " << element_size_ << std::endl;
        return ss.str();
    }

    const uint8_t *Parquet2Cpp::GetRawColumnData() {
        return raw_data_;
    }

    int64_t Parquet2Cpp::GetRowCount() {
        return row_count_;
    }

    int64_t Parquet2Cpp::GetWidth() {
        return width_;
    }

    int64_t Parquet2Cpp::GetElementSize() {
        return element_size_;
    }

    std::string Parquet2Cpp::GetElementType() {
        return element_type_;
    }
}
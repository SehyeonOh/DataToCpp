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

    Parquet2Cpp::Parquet2Cpp(const std::vector<std::string>& source_files, std::string column_name) 
        : Data2Cpp(source_files[0], column_name)  // Initialize base class with first file
    {
        std::vector<std::shared_ptr<arrow::Table>> tables;
        
        // Load tables from each file
        for (const auto& file : source_files) {
            tables.push_back(ParquetToArrowTable(file));
        }
        
        // Concatenate all tables
        auto combined_table_result = arrow::ConcatenateTables(tables);
        if (!combined_table_result.ok()) {
            throw std::runtime_error("Failed to concatenate tables: " + combined_table_result.status().ToString());
        }
        table_ = combined_table_result.ValueOrDie();

        batch_ = ArrowTableToSingleRecordBatch(table_);
        array_ = batch_->GetColumnByName(column_name_);

        if (array_ == nullptr) {
            std::stringstream ss;
            ss << "Columns: ";
            for (int i = 0; i < batch_->num_columns(); i++) {
                ss << batch_->column_name(i) << " ";
            }
            throw std::runtime_error("Column not found in the Parquet files: " + column_name_ + ". Columns: " + ss.str());
        }

        // Check if the column is a list type
        if (array_->type()->id() == arrow::Type::LIST) {
            auto list_array = std::static_pointer_cast<arrow::ListArray>(array_);
            auto values = list_array->values();
            raw_data_ = values->data()->GetValues<uint8_t>(1, 0);
            row_count_ = array_->length();
            width_ = list_array->value_length(0);
            element_size_ = values->type()->bit_width() / 8;
            element_type_ = values->type()->ToString();
        }
        else if (array_->type()->id() == arrow::Type::LARGE_LIST) {
            auto large_list_array = std::static_pointer_cast<arrow::LargeListArray>(array_);
            auto values = large_list_array->values();
            raw_data_ = values->data()->GetValues<uint8_t>(1, 0);
            row_count_ = array_->length();
            width_ = large_list_array->value_length(0);
            element_size_ = values->type()->bit_width() / 8;
            element_type_ = values->type()->ToString();
        }
        else if (array_->type()->id() == arrow::Type::FIXED_SIZE_LIST) {
            auto fixed_list_array = std::static_pointer_cast<arrow::FixedSizeListArray>(array_);
            auto values = fixed_list_array->values();
            raw_data_ = values->data()->GetValues<uint8_t>(1, 0);
            row_count_ = array_->length();
            width_ = fixed_list_array->list_type()->list_size();
            element_size_ = values->type()->bit_width() / 8;
            element_type_ = values->type()->ToString();
        }
        else {
            throw std::runtime_error("Column is not a list type array: " + column_name_ + ". Column type: " + array_->type()->ToString());
        }

        SetFloatData(raw_data_);
        std::cout << ToString(true) << std::endl;
    }

    std::string Parquet2Cpp::ToString(bool verbose)
    {
        std::stringstream ss;
        ss << "Parquet2Cpp" << std::endl;
        ss << "  source_files: ";
        for (const auto& file : source_files_) {
            ss << file << " ";
        }
        ss << std::endl;
        ss << "  column_name: " << column_name_ << std::endl;
        ss << "  row_count: " << row_count_ << std::endl;
        ss << "  width: " << width_ << std::endl;
        ss << "  data_type: " << array_->type()->ToString() << std::endl;
        ss << "  element_size: " << element_size_ << std::endl;
        return ss.str();
    }

    const uint8_t *Parquet2Cpp::GetRawData(int64_t row_index) {
        return raw_data_ + row_index * width_ * element_size_;
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
#include "data2cpp/binary/binary2cpp.hh"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace data2cpp {

Binary2Cpp::Binary2Cpp(const std::string &source_file, int64_t width, int64_t element_size, int64_t skip_bytes)
    : Data2Cpp(source_file), width_(width), element_size_(element_size), skip_bytes_(skip_bytes) {
    
    std::ifstream file(source_file, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + source_file);
    }

    // Calculate file size
    file.seekg(0, std::ios::end);
    int64_t file_size = file.tellg();
    file.seekg(skip_bytes_, std::ios::beg);

    // Calculate available data size and row count
    int64_t available_size = file_size - skip_bytes_;
    int64_t row_size = width_ * element_size_;
    
    if (available_size % row_size != 0) {
        throw std::runtime_error("Invalid file format: file size is not aligned with row size");
    }
    
    row_count_ = available_size / row_size;

    // Read data
    raw_data_.resize(available_size);
    file.read(reinterpret_cast<char*>(raw_data_.data()), available_size);
    file.close();
    SetFloatData(raw_data_.data());
}

const uint8_t *Binary2Cpp::GetRawData(int64_t row_index) {
    if (row_index >= row_count_) {
        throw std::out_of_range("Row index out of range");
    }
    return raw_data_.data() + (row_index * width_ * element_size_);
}

int64_t Binary2Cpp::GetRowCount() {
    return row_count_;
}

int64_t Binary2Cpp::GetWidth() {
    return width_;
}

int64_t Binary2Cpp::GetElementSize() {
    return element_size_;
}

std::string Binary2Cpp::ToString(bool verbose) {
    std::stringstream ss;
    ss << "Binary2Cpp: " << source_file_ << "\n"
       << "  width: " << width_ << "\n"
       << "  element_size: " << element_size_ << "\n"
       << "  skip_bytes: " << skip_bytes_ << "\n"
       << "  rows: " << row_count_;
    return ss.str();
}

}  // namespace data2cpp
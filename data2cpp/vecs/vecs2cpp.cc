#include "data2cpp/vecs/vecs2cpp.hh"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace data2cpp {

Vecs2Cpp::Vecs2Cpp(const std::string &source_file, const std::string &element_type)
    : Data2Cpp(source_file), element_type_(element_type) {
    
    if (element_type != "int32" && element_type != "float") {
        throw std::runtime_error("Unsupported element type: " + element_type);
    }
    element_size_ = 4;  // both int32 and float are 4 bytes

    std::ifstream file(source_file, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + source_file);
    }

    // Read first vector to get width
    int32_t dim;
    file.read(reinterpret_cast<char*>(&dim), sizeof(dim));
    width_ = dim;

    // Calculate file size and number of vectors
    file.seekg(0, std::ios::end);
    int64_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Each vector has: 4 bytes (dim) + dim * element_size bytes
    int64_t bytes_per_vector = sizeof(int32_t) + width_ * element_size_;
    row_count_ = file_size / bytes_per_vector;

    if (file_size % bytes_per_vector != 0) {
        throw std::runtime_error("Invalid file format: file size is not aligned with vector size");
    }

    // Read only vector data, skipping dimension fields
    raw_data_.resize(row_count_ * width_ * element_size_);
    for (int64_t i = 0; i < row_count_; i++) {
        // Skip dimension field
        file.seekg(sizeof(int32_t), std::ios::cur);
        // Read vector data
        file.read(reinterpret_cast<char*>(raw_data_.data() + i * width_ * element_size_), 
                 width_ * element_size_);
    }
    file.close();
    SetFloatData(raw_data_.data());
}

const uint8_t *Vecs2Cpp::GetRawData(int64_t row_index) {
    if (row_index >= row_count_) {
        throw std::out_of_range("Row index out of range");
    }
    return raw_data_.data() + (row_index * width_ * element_size_);
}

int64_t Vecs2Cpp::GetRowCount() {
    return row_count_;
}

int64_t Vecs2Cpp::GetWidth() {
    return width_;
}

int64_t Vecs2Cpp::GetElementSize() {
    return element_size_;
}

std::string Vecs2Cpp::ToString(bool verbose) {
    std::stringstream ss;
    ss << "Vecs2Cpp: " << source_file_ << "\n"
       << "  width: " << width_ << "\n"
       << "  element_type: " << element_type_ << "\n"
       << "  element_size: " << element_size_ << "\n"
       << "  rows: " << row_count_;
    return ss.str();
}

}  // namespace data2cpp 
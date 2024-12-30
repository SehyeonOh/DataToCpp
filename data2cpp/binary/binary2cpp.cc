#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>

#include "data2cpp/binary/binary2cpp.hh"

namespace data2cpp {
    std::vector<uint8_t> BinaryToVector(const std::string &source_file, int64_t width, int64_t element_size, int64_t skip_bytes) {
        std::ifstream file(source_file, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("파일을 열 수 없습니다: " + source_file);
        }

        // 파일 크기 계산
        file.seekg(0, std::ios::end);
        int64_t file_size = file.tellg();
        
        // 읽을 데이터 크기 계산
        int64_t data_size = file_size - skip_bytes;
        if (data_size <= 0) {
            throw std::runtime_error("읽을 데이터가 없습니다");
        }

        // 데이터 크기가 width * element_size로 나누어 떨어지는지 확인
        if (data_size % (width * element_size) != 0) {
            throw std::runtime_error("데이터 크기가 width * element_size의 배수가 아닙니다");
        }

        // 시작 위치로 이동
        file.seekg(skip_bytes, std::ios::beg);

        // vector 생성 및 데이터 읽기
        std::vector<uint8_t> buffer(data_size);
        file.read(reinterpret_cast<char*>(buffer.data()), data_size);

        if (!file) {
            throw std::runtime_error("파일 읽기 실패");
        }

        return buffer;
    }

    Binary2Cpp::Binary2Cpp(const std::string &source_file, int64_t width, int64_t element_size, int64_t skip_bytes)
        : Data2Cpp(source_file), width_(width), element_size_(element_size), skip_bytes_(skip_bytes) {
        raw_data_ = std::move(BinaryToVector(source_file, width, element_size, skip_bytes));
    }

    const uint8_t *Binary2Cpp::GetRawData(int64_t row_index) {
        return raw_data_.data() + row_index * width_ * element_size_;
    }

    int64_t Binary2Cpp::GetRowCount() {
        return raw_data_.size() / (width_ * element_size_);
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
           << "  rows: " << GetRowCount();
        return ss.str();
    }
}
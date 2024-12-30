#include <iostream>
#include "data2cpp/parquet/parquet2cpp.hh"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "사용법: " << argv[0] << " <parquet_file_path> <column_name>" << std::endl;
        return 1;
    }

    try {
        // Parquet 파일 경로와 컬럼명
        std::string file_path = argv[1];
        std::string column_name = argv[2];
        
        // Parquet2Cpp 객체 생성
        data2cpp::Parquet2Cpp parquet_reader(file_path, column_name);
        
        // 데이터 정보 출력
        std::cout << "Parquet 파일 정보:" << std::endl;
        std::cout << parquet_reader.ToString(true) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "에러 발생: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 
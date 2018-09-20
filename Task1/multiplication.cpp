#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

template <typename Type>
void multiplication(const std::vector<std::vector<Type>>& A, const std::vector<std::vector<Type>>& B,
								const std::string& file_name, const std::size_t line1, const std::size_t column1,
								const std::size_t column2, const int option)
{
	std::vector<std::vector<Type>> result(line1, std::vector<Type>(column2, 0));
	switch (option) {
		case 0:
			{
				auto begin = std::chrono::steady_clock::now();
				for (int i {0}; i < line1; ++i) {
					for (int j {0}; j < column2; ++j) {
						for (int k {0}; k < column1; ++k) {
							result[i][j] += A[i][k] * B[k][j];
						}
					}
				}
				auto end = std::chrono::steady_clock::now();
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	      		//std::cout << "ijk" << "\t"<< elapsed_ms.count() << std::endl;
	      		std::cout << "0" << "\t"<< elapsed_ms.count() << std::endl;
      		}
			break;
		case 1:
			{
				auto begin = std::chrono::steady_clock::now();
				for (int i {0}; i < line1; ++i) {
					for (int k {0}; k < column1; ++k) {
						for (int j {0}; j < column2; ++j) {
							result[i][j] += A[i][k] * B[k][j];	
						}
					}
				}
				auto end = std::chrono::steady_clock::now();
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	      		//std::cout << "ikj" << "\t"<< elapsed_ms.count() << std::endl;
	      		std::cout << "1" << "\t"<< elapsed_ms.count() << std::endl;
      		}
			break;
		case 2:
			{
				auto begin = std::chrono::steady_clock::now();
				for (int k {0}; k < column1; ++k) {
					for (int i {0}; i < line1; ++i) {
						for (int j {0}; j < column2; ++j) {
							result[i][j] += A[i][k] * B[k][j];
						}
					}
				}
				auto end = std::chrono::steady_clock::now();
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	      		//std::cout << "kij" << "\t"<< elapsed_ms.count() << std::endl;
	      		std::cout << "2" << "\t"<< elapsed_ms.count() << std::endl;
      		}
			break;
		case 3:
			{
				auto begin = std::chrono::steady_clock::now();
				for (int j {0}; j < column2; ++j) {
					for (int i {0}; i < line1; ++i) {
						for (int k {0}; k < column1; ++k) {
							result[i][j] += A[i][k] * B[k][j];
						}
					}
				}
				auto end = std::chrono::steady_clock::now();
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	      		//std::cout << "jik" << "\t"<< elapsed_ms.count() << std::endl;
	      		std::cout << "3" << "\t"<< elapsed_ms.count() << std::endl;
			}
			break;
		case 4:
			{
				auto begin = std::chrono::steady_clock::now();
				for (int j {0}; j < column2; ++j) {
					for (int k {0}; k < column1; ++k) {
						for (int i {0}; i < line1; ++i) {
							result[i][j] += A[i][k] * B[k][j];
						}
					}
				}
				auto end = std::chrono::steady_clock::now();
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	      		//std::cout << "jki" << "\t"<< elapsed_ms.count() << std::endl;
	      		std::cout << "4" << "\t"<< elapsed_ms.count() << std::endl;
      		}
			break;
		case 5:
			{
				auto begin = std::chrono::steady_clock::now();
				for (int k {0}; k < column1; ++k) {
					for (int j {0}; j < column2; ++j) {
						for (int i {0}; i < line1; ++i) {
							result[i][j] += A[i][k] * B[k][j];
						}
					}
				}
				auto end = std::chrono::steady_clock::now();
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	      		//std::cout << "kji" << "\t"<< elapsed_ms.count() << std::endl;
      			std::cout << "5" << "\t"<< elapsed_ms.count() << std::endl;
      		}
			break;
		default:
			//error//
			break;
	}
	std::ofstream C(file_name, std::ios::binary);
	if (std::is_same_v<Type, float>) {
		char f = 'f';
		C.write((char*)& f, sizeof f);
	} else {
		char d = 'd';
		C.write((char*)& d, sizeof d);
	}
	C.write((char*)& line1, sizeof line1);
	C.write((char*)& column2, sizeof column2);
	for (int i {0}; i < line1; ++i) {
		for (int j {0}; j < column2; ++j) {
			C.write((char*)& result[i][j], sizeof result[i][j]);
		}
	}
	C.close();
}

template <typename Type>
void arr_input(std::vector<std::vector<Type>>& vec, std::size_t lines, std::size_t column, std::ifstream& file)
{	
	Type tmp;
	for (int i {0}; i < lines; ++i) {
		for (int j {0}; j < column; ++j) {
			file.read((char*)& tmp, sizeof tmp);
			vec[i][j] = tmp;
		}
	}
}

int main(int argc, char* argv[])
{
	std::ifstream fileA(argv[1], std::ios::binary);
	std::ifstream fileB(argv[2], std::ios::binary);
	char type1, type2;
	fileA.read((char *)& type1, sizeof type1);
	fileB.read((char *)& type2, sizeof type2);
	if (type1 != type2) {
		std::cout << "Incompatible types" << std::endl;
		return 1;
	}
	std::size_t line1, line2, column1, column2;
	fileA.read((char *)& line1, sizeof line1);
	fileA.read((char *)& column1, sizeof column1);
	fileB.read((char *)& line2, sizeof line2);
	fileB.read((char *)& column2, sizeof column2);
	if (column1 != line2) {
		std::cout << "Incompatible arrays" << std::endl;
		return 1;	
	}
	if (type1 == 'f') {
		std::vector<std::vector<float>> A(line1, std::vector<float>(column1));
		arr_input(A, line1, column1, fileA);
		std::vector<std::vector<float>> B(line2, std::vector<float>(column2));
		arr_input(B, line2, column2, fileB);
		fileA.close();
		fileB.close();
		multiplication(A, B, argv[3], line1, column1, column2, std::strtoul(argv[4], NULL, 10));
	} else {
		std::vector<std::vector<double>> A(line1, std::vector<double>(column1));
		arr_input(A, line1, column1, fileA);
		std::vector<std::vector<double>> B(line2, std::vector<double>(column2));
		arr_input(B, line2, column2, fileB);
		fileA.close();
		fileB.close();
		multiplication(A, B, argv[3], line1, column1, column2, std::strtoul(argv[4], NULL, 10));
	}
	return 0;
}
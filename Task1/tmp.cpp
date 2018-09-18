#include <iostream>
#include <fstream>
#include <cfloat>

int main(int argc, char* argv[]) {
	std::ifstream file1 (argv[1], std::ifstream::binary);
	std::ifstream file2 (argv[2], std::ifstream::binary);
	char type1, type2;
	std::size_t line1, line2, column1, column2;
	file1.read((char*)& type1, sizeof type1);
	file2.read((char*)& type2, sizeof type2);
	if (type1 != type2) {
		std::cout << "Unequal" << std::endl;
		return 0;
	}
	file1.read((char*)& line1, sizeof line1);
	file2.read((char*)& line2, sizeof line2);
	if (line1 != line2) {
		std::cout << "Unequal" << std::endl;
		return 0;
	}
	file1.read((char*)& column1, sizeof column1);
	file2.read((char*)& column2, sizeof column2);
	if (column1 != column2) {
		std::cout << "Unequal" << std::endl;
		return 0;
	}
	if (type1 == 'f') {
		float arr1, arr2;
		for (std::size_t i = {0}; i < line1; ++i) {
			for (std::size_t j {0}; j < column1; ++j) {
				file1.read((char*)& arr1, sizeof arr1);
				file2.read((char*)& arr2, sizeof arr2);
				if (std::abs(arr1 - arr2) > FLT_EPSILON) {
					std::cout << "Unequal" << std::endl;
					return 0;
				}
			}
		}
	} else {
		double arr1, arr2;
		for (std::size_t i = {0}; i < line1; ++i) {
			for (std::size_t j {0}; j < column1; ++j) {
				file1.read((char*)& arr1, sizeof arr1);
				file2.read((char*)& arr2, sizeof arr2);
				if (std::abs(arr1 - arr2) > FLT_EPSILON) {
					std::cout << "Unequal" << std::endl;
					return 0;
				}
			}
		}
	}
	file1.close();
	file2.close();
	std::cout << "Equal" << std::endl;
	return 0;
}
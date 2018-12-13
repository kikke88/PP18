#include <iostream>
#include <fstream>
#include <cfloat>
template <typename Type>

const std::string cmp(const std::size_t lines, const std::size_t columns, std::ifstream& file1,
								std::ifstream& file2, Type type) {
	Type arr1, arr2;
	for (std::size_t i = {0}; i < lines; ++i) {
		for (std::size_t j {0}; j < columns; ++j) {
			file1.read((char*)& arr1, sizeof arr1);
			file2.read((char*)& arr2, sizeof arr2);
			if (std::abs(arr1 - arr2) > FLT_EPSILON) {
				return "Unequal";
			}
		}
	}
	return "Equal";
}

int main(int argc, char* argv[]) {
	std::ifstream file1 (argv[1], std::ifstream::binary);
	std::ifstream file2 (argv[2], std::ifstream::binary);
	char type1, type2;
	file1.read((char*)& type1, sizeof type1);
	file2.read((char*)& type2, sizeof type2);
	if (type1 != type2) {
		std::cout << "Unequal" << std::endl;
		return 0;
	}
	std::size_t line1, line2;
	file1.read((char*)& line1, sizeof line1);
	file2.read((char*)& line2, sizeof line2);
	if (line1 != line2) {
		std::cout << "Unequal" << std::endl;
		return 0;
	}
	std::size_t column1, column2;
	file1.read((char*)& column1, sizeof column1);
	file2.read((char*)& column2, sizeof column2);
	if (column1 != column2) {
		std::cout << "Unequal" << std::endl;
		return 0;
	}
	if (type1 == 'f') {
		float f {0.0};
		std::cout << cmp(line1, column1, file1, file2, f) << std::endl;
	} else {
		double d {0.0};
		std::cout << cmp(line1, column1, file1, file2, d) << std::endl;
	}
	file1.close();
	file2.close();
	return 0;
}

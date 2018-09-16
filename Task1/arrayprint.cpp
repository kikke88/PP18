#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
	std::ifstream inputfile (argv[1], std::ofstream::binary);
	char type;
	inputfile.read((char *)& type, sizeof type);
	std::size_t line, column;
	inputfile.read((char *)& line, sizeof line);
	inputfile.read((char *)& column, sizeof column);
	std::cout.setf(std::ios::scientific);
	if (type == 'f') {
		float tmp;
		for (std::size_t i {0}; i < line; ++i) {
			for (std::size_t j {0}; j < column; ++j) {
				inputfile.read((char *)& tmp, sizeof tmp);
				std::cout << tmp << "  ";
			}
			std::cout << std::endl;
		}
	} else {
		double tmp;
		for (std::size_t i {0}; i < line; ++i) {
			for (std::size_t j {0}; j < column; ++j) {
				inputfile.read((char *)& tmp, sizeof tmp);
				std::cout << tmp << "  ";
			}
			std::cout << std::endl;
		}
	}
	inputfile.close();
	return 0;
}
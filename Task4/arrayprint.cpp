#include <iostream>
#include <fstream>

void print_array(const std::size_t N, const std::size_t M, const char type, std::ifstream& file) {
	std::cout << type << std::endl;
	std::cout << N << " * " << M << std::endl;
	if (type == 'f') {
		float tmp;
		for (std::size_t i = 0; i < N; ++i) {
			for (std::size_t j = 0; j < M; ++j) {
				file.read((char*)& tmp, sizeof tmp);
				std::cout << tmp << "  ";
			}
			std::cout << std::endl;
		}	
	} else {
		double tmp;
		for (std::size_t i = 0; i < N; ++i) {
			for (std::size_t j = 0; j < M; ++j) {
				file.read((char*)& tmp, sizeof tmp);
				std::cout << tmp << "  ";
			}
			std::cout << std::endl;
		}			
	}
}

int main(int argc, char* argv[]) {
	std::ifstream inputfile (argv[1], std::ofstream::binary);
	char type;
	inputfile.read((char *)& type, sizeof type);
	std::size_t line, column;
	inputfile.read((char *)& line, sizeof line);
	inputfile.read((char *)& column, sizeof column);
	std::cout.setf(std::ios::scientific);
	print_array(line, column, type, inputfile);
	inputfile.close();
	return 0;
}
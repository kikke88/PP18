#include <iostream>
#include <fstream>

template <typename Type>
void print_array(const std::size_t N, const std::size_t M, const Type tp, std::ifstream& file) {
	char type;
	if constexpr (std::is_same_v<Type, float>) {
		type = 'f';
	} else {
		type = 'd';
	}
	std::cout << type << std::endl;
	std::cout << N << " * " << M << std::endl;
	Type tmp;
	for (std::size_t i {0}; i < N; ++i) {
		for (std::size_t j {0}; j < M; ++j) {
			file.read((char*)& tmp, sizeof tmp);
			std::cout << tmp << "  ";
		}
		std::cout << std::endl;
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
	if (type == 'f') {
		float f {0.0};
		print_array(line, column, f, inputfile);
	} else {
		double d {0.0};
		print_array(line, column, d, inputfile);
	}

	inputfile.close();
	return 0;
}
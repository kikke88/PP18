#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

template <typename Type>
void gen_array(const std::size_t N, const std::size_t M, const Type tp, const std::string& file_name) {
	char type;
	std::ofstream outfile (file_name, std::ofstream::binary);
	if constexpr (std::is_same_v<Type, float>) {
		type = 'f';
	} else {
		type = 'd';
	}
	outfile.write((char*)& type, sizeof type);
	outfile.write((char*)& N, sizeof N);
	outfile.write((char*)& M, sizeof M);
	const double from = -100.0;
	const double to = 100.0;
	std::default_random_engine engine(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	std::uniform_real_distribution<Type> distr(from, to);
	auto gen_number = [&engine, &distr] () { return distr(engine); };
	Type tmp;
	for (std::size_t i {0}; i < N; ++i) {
		for (std::size_t j {0}; j < M; ++j) {
			tmp = gen_number();
			outfile.write((char*)& tmp, sizeof tmp);
		}
	}
	outfile.close();
}

int main(int argc, char* argv[]) {
	std::size_t N {std::strtoul(argv[2], NULL, 10)};
	std::size_t M {std::strtoul(argv[3], NULL, 10)};
	if (argv[1][0] == 'f') {
		float f {0.0};
		gen_array(N, M ,f, argv[4]);//
	} else {
		double d {0.0};
		gen_array(N, M, d, argv[4]);
	}
	return 0;
}
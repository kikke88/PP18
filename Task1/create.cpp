#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

int main(int argc, char* argv[]) {
	

	std::ofstream outfile (argv[4], std::ofstream::binary);

	const double from = -100.0;
	const double to = 100.0;
	std::default_random_engine engine(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	std::uniform_real_distribution<> distr(from, to);
	auto gen_number = [&engine, &distr] () { return distr(engine); };

	char type {argv[1][0]};
	std::size_t N {std::strtoul(argv[2], NULL, 10)};
	std::size_t M {std::strtoul(argv[3], NULL, 10)};
	std::cout << N << "  " << M << std::endl;
	outfile.write((char*)& type, sizeof(type));
	outfile.write((char*)& N, sizeof(N));
	outfile.write((char*)& M, sizeof(M));
	double tmp;
	for (std::size_t i {0}; i < N; ++i) {
		for (std::size_t j {0}; j < M; ++j) {
			tmp = gen_number();
			outfile.write((char*)& tmp, sizeof(tmp));
		}
	}
	outfile.close();
	return 0;
}
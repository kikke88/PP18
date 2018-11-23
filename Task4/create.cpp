#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

void gen_array(const std::size_t N, const std::size_t M, const char type, const std::string& file_name) {
	std::ofstream outfile (file_name, std::ofstream::binary);
	outfile.write((char*)& type, sizeof type);
	outfile.write((char*)& N, sizeof N);
	outfile.write((char*)& M, sizeof M);
	const double from = -100.0;
	const double to = 100.0;
	std::default_random_engine engine(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	if (type == 'f') {
		std::uniform_real_distribution<float> distr(from, to);
		auto gen_number = [&engine, &distr] () { return distr(engine); };
		float tmp;
		for (std::size_t i {0}; i < N; ++i) {
			for (std::size_t j {0}; j < M; ++j) {
				tmp = gen_number();
				outfile.write((char*)& tmp, sizeof tmp);
			}
		}		
	} else {
		std::uniform_real_distribution<double> distr(from, to);
		auto gen_number = [&engine, &distr] () { return distr(engine); };
		double tmp;
		for (std::size_t i {0}; i < N; ++i) {
			for (std::size_t j {0}; j < M; ++j) {
				tmp = gen_number();
				outfile.write((char*)& tmp, sizeof tmp);
			}
		}			
	}
	outfile.close();
}

int main(int argc, char* argv[]) {
	std::size_t N {std::strtoul(argv[2], NULL, 10)};
	std::size_t M {std::strtoul(argv[3], NULL, 10)};
	char type = argv[1][0];
	gen_array(N, M ,type, argv[4]);
	return 0;
}
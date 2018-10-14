#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <chrono>
#include <random>

int main(int argc, char* argv[]) {
	std::ofstream ofile (argv[1], std::ofstream::binary);
	const char type {'f'};
	const std::size_t size = std::atol(argv[2]);//{}
	ofile.write((char*)& type, sizeof type);
	ofile.write((char*)& size, sizeof size);
	ofile.write((char*)& size, sizeof size);
	const float from {-100}, to {100};
	std::default_random_engine engine(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	std::uniform_real_distribution<float> distr(from, to);
	auto gen_number = [&engine, &distr] () { return distr(engine); };
	float tmp;
	for (std::size_t i {0}; i < size; ++i) {
		for (std::size_t j {0}; j < size; ++j) {
			tmp = gen_number();
			//ofile << tmp << "\t";
			ofile.write((char*)& tmp, sizeof tmp);
		}
	}
	return 0;
}
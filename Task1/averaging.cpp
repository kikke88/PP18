#include <iostream>
#include <vector>
#include <fstream>

int main(int argc, char* argv[]) {
	std::vector<int> avg (6, 0);
	std::ifstream file(argv[1]);
	int tmp;
	for (int i {0}; i < 7; ++i) {
		for (int j {0}; j < 6; ++j) {
			file >> tmp;	
			file >> tmp;
			avg[j] = avg[j] + tmp;
		}
	}
	std::ofstream ofile(argv[1]);
	for (int i {0}; i < 6; ++i) {
		ofile << i  << "\t" << avg[i] / 7.0 << std::endl;
	}
	return 0;
}
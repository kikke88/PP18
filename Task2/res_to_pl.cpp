#include <iostream>
#include <fstream>
#include <vector>

#define MODE 3
#define VAR_SIZE 2//5

int main(int argc, char* argv[]) {
	std::ifstream ifile(argv[1]);
	std::ofstream ofile(argv[2]);
	std::vector<std::vector<int>> res(VAR_SIZE * 2, std::vector<int>(MODE));
	std::vector<int> line2(MODE);
	float tmp;
	for (std::size_t i {0}; i < VAR_SIZE * 2; i += 2) {
		for (std::size_t j {0}; j < MODE; ++j) {
			ifile >> tmp;
			res[i][j] = tmp;
			res[i + 1][j] = tmp;
		}	 
	}
	for (int i = 0; i < VAR_SIZE * 2; ++i)
	{
		for (int j = 0; j < MODE; ++j)
		{
			std::cout << res[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::vector<int> Y(VAR_SIZE * 2);
	Y[0] = 0;
	for (std::size_t i {0}; i < VAR_SIZE - 1; ++i) {
		Y[1 + 2 * i] = i + 1;
		Y[2 + 2 * i] = i + 1;
	}
	Y[VAR_SIZE * 2 - 1] = VAR_SIZE;

	for (std::size_t i {0}; i < MODE; ++i) {
		for(std::size_t j {0}; j < VAR_SIZE * 2; ++j) {
			ofile << i << " " << Y[j] << " " << res[j][i] << std::endl;
		}
		ofile << std::endl;
		for(std::size_t j {0}; j < VAR_SIZE * 2; ++j) {
			ofile << i + 1 << " " << Y[j] << " " << res[j][i] << std::endl;
		}
		ofile << std::endl;
	}
	return 0;
}
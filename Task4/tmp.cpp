#include <iostream>

int main() {
	std::size_t i {3.4};
	int k {35}, size {34};
	for (int i {0}; i < size; ++i) {
		std::cout << i << "\t" << k * i / size << "\t" << k * (i + 1) / size << std::endl;
	}
	return 0;
}
#include <cstdlib>
#include <iostream>
#include <ctime>

int main() {
	std::srand(std::time(0));
	for (int i {0}; i < 10; ++i) {
		std::cout << rand() << std::endl;
	}
	std::cout << RAND_MAX << std::endl;
	bool a = 1 == 0;
	std::cout << a << std::endl;
	return 0;
}
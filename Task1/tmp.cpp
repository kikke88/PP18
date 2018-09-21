#include <iostream>
#include <utility>
	
std::pair<double, double> f(double r) {
	return std::pair<double, double>(2 * r, r); 
}

int main() {
  const auto [a, b] = f(6.55);

  std::cout << a << std::endl << b << std::endl;
}
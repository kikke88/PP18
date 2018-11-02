#include <iostream>
#include <algorithm>
#include <cmath>


int main(int argc, char* argv[]) {


	int sqrt_from_r_border;
	std::cin >> sqrt_from_r_border;
	
	int l_border {std::atoi(argv[1])}, r_border {std::atoi(argv[2])};
    l_border = std::max(l_border, (int)std::sqrt(r_border) + 1);
    int size = sqrt_from_r_border;
    for (int i = 1; i < sqrt_from_r_border; ++i) {
    	int own_l_border {(r_border - l_border + 1) * (i - 1) / (size - 1) + l_border};
    	int own_r_border {(r_border - l_border + 1) * i / (size - 1) + l_border - 1};	
    	std::cout << own_l_border << "  " << own_r_border << std::endl;
    }

/*
	int arr_size {sqrt_from_r_border / 32 + 1};
    int* sqrt_prime {new int [arr_size]};
    for (int i {0}; i < arr_size; ++i) {
        sqrt_prime[i] = -1;
    }
    for (int i = 2; i * i <= sqrt_from_r_border; ++i) {
        if (sqrt_prime[i >> 5] & 1 << (i & 31)) {
            for (int j = i * i; j <= sqrt_from_r_border; j += i) {
                sqrt_prime[j >> 5] &= ~(1 << (j & 31));    
            }
        }
    }
    for (int i = 0; i <= sqrt_from_r_border; ++i) {
    	if (sqrt_prime[i >> 5] & 1 << (i & 31)) {
    		std::cout << i << std::endl;
    	}
    }
*/


}
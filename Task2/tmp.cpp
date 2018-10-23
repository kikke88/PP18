#include <iostream>
#include "papi.h"
#include <vector>


#define NUM_EVENTS 3


void handle_error (int retval)
{
    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
    exit(1);
}


int main() {
	int Events[NUM_EVENTS] {PAPI_TOT_CYC, PAPI_L1_DCM, PAPI_L2_DCM};//, PAPI_L2_TCM};
	long long values[NUM_EVENTS];
	float real_time, proc_time, mflops;
	long long flpins;

	std::size_t size {1000};
	std::vector<std::vector<float>> A(size, std::vector<float>(size, 1)),
									B(size, std::vector<float>(size, 2)),
									C(size, std::vector<float>(size, 0));
	std::size_t block_size = 48;
	int retval;	
	if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
		handle_error(retval);
	}
	for (std::size_t i {0}; i < size; i += block_size) {
		for (std::size_t j {0}; j < size; j += block_size) {
			for (std::size_t k {0}; k < size; k += block_size) {
				for (std::size_t m {i}; m < i + block_size and m < size; ++m) {
					for (std::size_t n {j}; n < j + block_size and n < size; ++n) {
						for (std::size_t l {k}; l < k + block_size and l < size; ++l) {
							C[m][n] += A[m][l] * B[l][n];
						}
					}
				}
			}
		}
	}
	if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
		handle_error(retval);
	}
	PAPI_shutdown();
	/*
	for (std::size_t l {0}; l < NUM_EVENTS; ++l) {
		std::cout << values[l] << std::endl;						
	}
	*/
	std::cout << real_time << std::endl << proc_time << std::endl << flpins << std::endl << mflops << std::endl;
	float** Aa = new float* [size];
	for(int i = 0; i < size; ++i) {
		Aa[i] = new float [size];
	}
	float** Bb = new float* [size];
	for(int i = 0; i < size; ++i) {
		Bb[i] = new float [size];
	}
	float** Cc = new float* [size];
	for(int i = 0; i < size; ++i) {
		Cc[i] = new float [size];
	}
	std::cout << "FFF" << std::endl;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			Aa[i][j] = 0;
			Bb[i][j] = 0;
			Cc[i][j] = 0;
		}
	}
	std::size_t i, j, k, m, n, l, m_min, n_min, l_min;
	if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
		handle_error(retval);
	}
	
	for (i = 0; i < size; i += block_size) {
		for (j = 0; j < size; j += block_size) {
			for (k = 0; k < size; k += block_size) {
				for (m = i, m_min = std::min(size, i + block_size); m < m_min; ++m) {
					for (n = j, n_min = std::min(size, j + block_size); n < n_min; ++n) {
						for (l = k, l_min = std::min(size, k + block_size); l < l_min; ++l) {
							Cc[m][n] += Aa[m][l] * Bb[l][n];
						}
					}
				}
			}
		}
	}
	if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
		handle_error(retval);
	}
	PAPI_shutdown();
	std::cout << real_time << std::endl << proc_time << std::endl << flpins << std::endl << mflops << std::endl;
	
	for (int i = 0; i < size; i++) {
    	delete[] Aa[i];
    }
    delete [] Aa;
    for (int i = 0; i < size; i++) {
    	delete[] Bb[i];
    }
    delete [] Bb;
    for (int i = 0; i < size; i++) {
    	delete[] Cc[i];
    }
    delete [] Cc;
	return 0;
}
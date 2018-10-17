#include <iostream>
#include "papi.h"
#include <vector>


#define NUM_EVENTS 1


void handle_error (int retval)
{
    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
    exit(1);
}


int main() {
	std::cout << PAPI_num_counters() << "/////////" << sizeof(float) << std::endl;
	//PAPI_TOT_CYC, PAPI_TLB_TL, PAPI_L1_TCM, PAPI_L2_TCM
	int Events[NUM_EVENTS] {PAPI_L1_TCM};//, PAPI_L2_TCM};
	long long values[NUM_EVENTS];
	//int Events[NUM_EVENTS];
	//float real_time, proc_time, mflops;
	//long long flpins;
	std::size_t size {1000};
	std::vector<std::vector<float>> A(size, std::vector<float>(size, 1)),
									B(size, std::vector<float>(size, 2)),
									C(size, std::vector<float>(size, 0));
	//PAPI_start_counters(Events, 2);
	std::size_t block_size = 24;
	//PAPI_flops(&real_time, &proc_time, &flpins, &mflops);
	int retval;
	if ((retval= PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK) {
		handle_error(retval);
	}
	for (std::size_t i {0}; i < size; i+= block_size) {
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
	//PAPI_flops(&real_time, &proc_time, &flpins, &mflops);
	PAPI_stop_counters(values, NUM_EVENTS);
	for (std::size_t l {0}; l < NUM_EVENTS; ++l) {
		std::cout << values[l] << std::endl;						
	}
	//std::cout << real_time << std::endl << proc_time << std::endl << flpins << std::endl << mflops << std::endl;
	PAPI_shutdown();
	return 0;
}
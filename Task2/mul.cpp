#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "papi.h"

#define NUM_EVENTS 2

void handle_error (int retval)
{
    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
    exit(1);
}

//мб вообще не надо использовать массов С
void mul(std::size_t size, std::vector<std::vector<float>>& A, std::vector<std::vector<float>>& B, int counter_num)
{
	std::size_t block_size;
	std::vector<std::vector<float>> C(size, std::vector<float>(size, 0));//
	if (counter_num == 0 or counter_num == 1) {
		int first_group_of_events[4] {PAPI_TOT_CYC, PAPI_TLB_TL, PAPI_L1_TCM, PAPI_L2_TCM};
		long long values[NUM_EVENTS];
		int Events[NUM_EVENTS];
		std::string first_event, second_event;
		if (counter_num == 0) {
			first_event = "CPU cycles";
			Events[0] = first_group_of_events[0];
			second_event = "Total translation lookaside buffer misses";
			Events[1] = first_group_of_events[1];
		} else {
			first_event = "Level 1 cache misses";
			Events[0] = first_group_of_events[2];
			second_event = "Level 2 cache misses";
			Events[1] = first_group_of_events[3];
		}
		std::cout << first_event << "\t" << second_event << std::endl;;
		//32*32 ijk
		block_size = 32;
		PAPI_start_counters(Events, NUM_EVENTS);
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
		PAPI_stop_counters(values, NUM_EVENTS);
		std::cout << values[0] << "\t" << values[1] << "\t" << " - ijk 32*32" << std::endl;
		
		//32*32 ikj
		block_size = 32;
		PAPI_start_counters(Events, NUM_EVENTS);
		for (std::size_t i {0}; i < size; i+= block_size) {
			for (std::size_t k {0}; k < size; k += block_size) {
				for (std::size_t j {0}; j < size; j += block_size) {
					for (std::size_t m {i}; m < i + block_size and m < size; ++m) {
						for (std::size_t l {k}; l < k + block_size and l < size; ++l) {
							for (std::size_t n {j}; n < j + block_size and n < size; ++n) {
								C[m][n] += A[m][l] * B[l][n];
							}
						}
					}
				}
			}
		}
		PAPI_stop_counters(values, NUM_EVENTS);
		std::cout << values[0] << "\t" << values[1] << "\t" << " - ikj 32*32" << std::endl;
		
		//b*b ikj
		//3 * b * b = 256 * 1024 / 16 (L2)  ~73.9
		//3 * b * b = 32 * 1024 / 16 (L1) ~26.1
 		if (counter_num == 1) {//L1 cache misses
 			block_size = 24;
 		} else {/////////////////////////////////////////////////////////////////////////разделить в два разных L1 i L2
 			block_size = 72;
 		}
		PAPI_start_counters(Events, NUM_EVENTS);
		for (std::size_t i {0}; i < size; i+= block_size) {
			for (std::size_t k {0}; k < size; k += block_size) {
				for (std::size_t j {0}; j < size; j += block_size) {
					for (std::size_t m {i}; m < i + block_size and m < size; ++m) {
						for (std::size_t l {k}; l < k + block_size and l < size; ++l) {
							for (std::size_t n {j}; n < j + block_size and n < size; ++n) {
								C[m][n] += A[m][l] * B[l][n];
							}
						}
					}
				}
			}
		}
		PAPI_stop_counters(values, NUM_EVENTS);
		std::cout << values[0] << "\t" << values[1] << "\t" << " - ikj " << block_size << "*" << block_size << std::endl;
	} else {
		std::cout << "Proc time" << "\t" << "MFLOP" << std::endl;
		float real_time, proc_time, mflops;
		long long flpins;
		//32*32 ijk
		block_size = 32;
		PAPI_flops(&real_time, &proc_time, &flpins, &mflops);
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
		PAPI_flops(&real_time, &proc_time, &flpins, &mflops);
		std::cout << proc_time << "\t" << mflops << "\t" << " - ijk 32*32" << std::endl;
		PAPI_shutdown();

		//32*32 ikj
		block_size = 32;
		PAPI_flops(&real_time, &proc_time, &flpins, &mflops);
		for (std::size_t i {0}; i < size; i+= block_size) {
			for (std::size_t k {0}; k < size; k += block_size) {
				for (std::size_t j {0}; j < size; j += block_size) {
					for (std::size_t m {i}; m < i + block_size and m < size; ++m) {
						for (std::size_t l {k}; l < k + block_size and l < size; ++l) {
							for (std::size_t n {j}; n < j + block_size and n < size; ++n) {
								C[m][n] += A[m][l] * B[l][n];
							}
						}
					}
				}
			}
		}
		PAPI_flops(&real_time, &proc_time, &flpins, &mflops);
		std::cout << proc_time << "\t" << mflops << "\t" << " - ikj 32*32" << std::endl;
		PAPI_shutdown();

		//b*b ikj
		//3 * b * b = 256 * 1024 / 16 (L2)  ~73.9
		//3 * b * b = 32 * 1024 / 16 (L1) ~26.1
 		block_size = 72;
		PAPI_flops(&real_time, &proc_time, &flpins, &mflops);
		for (std::size_t i {0}; i < size; i+= block_size) {
			for (std::size_t k {0}; k < size; k += block_size) {
				for (std::size_t j {0}; j < size; j += block_size) {
					for (std::size_t m {i}; m < i + block_size and m < size; ++m) {
						for (std::size_t l {k}; l < k + block_size and l < size; ++l) {
							for (std::size_t n {j}; n < j + block_size and n < size; ++n) {
								C[m][n] += A[m][l] * B[l][n];
							}
						}
					}
				}
			}
		}
		PAPI_flops(&real_time, &proc_time, &flpins, &mflops);
		std::cout << proc_time << "\t" << mflops << " - ijk " << block_size << "*" << block_size << std::endl;

	}
}
/*
	PAPI_library_init(PAPI_VER_CURRENT);
	PAPI_create_eventset(&EventSet);
	PAPI_add_events(EventSet, Events, NUM_EVENTS);
	PAPI_start(EventSet);
	PAPI_stop(EventSet, values);
*/


int main(int argc, char* argv[])
{
	std::ifstream arrA (argv[1], std::ifstream::binary);
	std::ifstream arrB (argv[2], std::ifstream::binary);
	char type;
	std::size_t size;
	arrA.read((char*)& type, sizeof type);
	arrB.read((char*)& type, sizeof type);
	arrA.read((char*)& size, sizeof size);
	arrB.read((char*)& size, sizeof size);
	arrA.read((char*)& size, sizeof size);
	arrB.read((char*)& size, sizeof size);
	std::vector<std::vector<float>> A(size, std::vector<float>(size, 0)),
									B(size, std::vector<float>(size, 0));
	float tmp;
	for (std::size_t i {0}; i < size; ++i) {
		for (std::size_t j {0}; j < size; ++j) {
			arrA.read((char*)& tmp, sizeof tmp);
			A[i][j] = tmp;
			arrB.read((char*)& tmp, sizeof tmp);
			B[i][j] = tmp;
		}
	}
	std::cout << "Choose counter pair number:" << std::endl
				<< "0 - CPU cycles / Total translation lookaside buffer misses" << std::endl
				<< "1 - Level 1 cache misses / Level 2 cache misses" << std::endl
				<< "2 - Proc time / MFLOP" << std::endl;
	int num;
	std::cin >> num;
	mul(size, A, B, num);
	return 0;
}
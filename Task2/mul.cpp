#include <iostream>
#include <fstream>
#include <vector>
#include "papi.h"

#define NUM_EVENTS 1

void handle_error (int retval)
{
    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
    exit(1);
}
/*
static void test_fail(char *file, int line, char *call, int retval){
    printf("%s\tFAILED\nLine # %d\n", file, line);
    if ( retval == PAPI_ESYS ) {
        char buf[128];
        memset( buf, '\0', sizeof(buf) );
        sprintf(buf, "System error in %s:", call );
        perror(buf);
    }
    else if ( retval > 0 ) {
        printf("Error calculating: %s\n", call );
    }
    else {
        printf("Error in %s: %s\n", call, PAPI_strerror(retval) );
    }
    printf("\n");
    exit(1);
}
*/

//мб вообще не надо использовать массов С
void mul(std::size_t size, std::vector<std::vector<float>>& A, std::vector<std::vector<float>>& B, int counter_num)
{
	
	std::size_t block_size;
	std::vector<std::vector<float>> C(size, std::vector<float>(size, 0));//
	if (counter_num >= 0 and counter_num < 4) {
		int first_group_of_events[4] {PAPI_TOT_CYC, PAPI_L1_TCM, PAPI_L2_TCM, PAPI_TLB_TL};
		long long values[1];
		int Event[1] = first_group_of_events[counter_num];
		
		//32*32 ijk
		block_size = 32;
		PAPI_start_counters(Event, 1);
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
		PAPI_stop_counters(values, 1);
		std::cout << "ijk 32*32 - " << values[0] << std::endl;
		
		//32*32 ikj
		block_size = 32;
		PAPI_start_counters(Event, 1);
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
		PAPI_stop_counters(values, 1);
		std::cout << "ikj 32*32 - " << values[0] << std::endl;
		
		//b*b ikj
		//3 * b * b = 256 * 1024 / 16 (L2)  ~73.9
		//3 * b * b = 32 * 1024 / 16 (L1) ~26.1
 		if (counter_num == 1) {
 			block_size = 24;
 		} else {
 			block_size = 72;
 		}
		PAPI_start_counters(Event, 1);
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
		PAPI_stop_counters(values, 1);
		std::cout << "ikj b*b - " << values[0] << std::endl;
	} else {

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
		if (counter_num == 4) {
			std::cout << "Proc time - " << proc_time << std::endl;
		} else {
			std::cout << "MFLOP - " << mflops << std::endl;
		}
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
		if (counter_num == 4) {
			std::cout << "Proc time - " << proc_time << std::endl;
		} else {
			std::cout << "MFLOP - " << mflops << std::endl;
		}

		//b*b ikj
		//3 * b * b = 256 * 1024 / 16 (L2)  ~73.9
		//3 * b * b = 32 * 1024 / 16 (L1) ~26.1
 		if (counter_num == 1) {
 			block_size = 24;
 		} else {
 			block_size = 72;
 		}
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
		if (counter_num == 4) {
			std::cout << "Proc time - " << proc_time << std::endl;
		} else {
			std::cout << "MFLOP - " << mflops << std::endl;
		}
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
	std::cout << "Choose counter number:" << std::endl
				<< "0 - CPU cycles" << std::endl
				<< "1 - Level 1 cache misses" << std::endl
				<< "2 - Level 2 cache misses" << std::endl
				<< "3 - Total translation lookaside buffer misses" << std::endl
				<< "4 - Proc ime" << std::endl
				<< "5 - MFLOP" << std::endl;
	int num;
	std::cin >> num;
	mul(size, A, B, num);
	return 0;
}
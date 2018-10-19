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

void C_reset(int size, std::vector<std::vector<float>>& C){
	for (std::size_t i {0}; i < size; ++i) {
		for (std::size_t j {0}; j < size; ++j) {
			C[i][j] = 0;
		}
	}
}

void mul(std::size_t size, std::vector<std::vector<float>>& A, std::vector<std::vector<float>>& B, int stat_flag, int counter_num)
{
	std::size_t block_size;
	int retval;
	std::string ofile1_name;
	std::string ofile2_name;
	std::vector<std::vector<float>> C(size, std::vector<float>(size, 0));//
	if (counter_num == 0 or counter_num == 1) {
		std::string first_event, second_event;
		if (counter_num == 0) {
			ofile1_name = "results/cpu_cycles.txt";
			ofile2_name = "results/l1_misses.txt";
			first_event = "CPU cycles";
			second_event = "Level 1 cache misses";
		} else {
			ofile1_name = "results/total_tlm_misses.txt";
			ofile2_name = "results/l2_misses.txt";
			first_event = "Total translation lookaside buffer misses";
			second_event = "Level 2 cache misses";
		}
		std::ofstream ofile1 (ofile1_name, std::ios_base::app);
		std::ofstream ofile2 (ofile2_name, std::ios_base::app);
		int first_group_of_events[4] {PAPI_TOT_CYC, PAPI_TLB_TL, PAPI_L1_TCM, PAPI_L2_TCM};
		long long values[NUM_EVENTS];
		int Events[NUM_EVENTS];		
		Events[0] = first_group_of_events[counter_num];
		Events[1] = first_group_of_events[counter_num + 2];
		if (!stat_flag) {
			std::cout << first_event << "\t" << second_event << std::endl;
		}

		//32*32 ijk
		block_size = 32;
		if ((retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK) {
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
		if ((retval = PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK) {
			handle_error(retval);
		}
		C_reset(size, C);
		if (!stat_flag) {
			std::cout << values[0] << "\t" << values[1] << "\t" << " - ijk 32*32" << std::endl;
		} else {
			ofile1 << values[0] << "\t";
			ofile2 << values[1] << "\t";
		}

		//32*32 ikj
		block_size = 32;
		if ((retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK) {
			handle_error(retval);
		}
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
		if ((retval = PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK) {
			handle_error(retval);
		}
		C_reset(size, C);
		if (!stat_flag) {
			std::cout << values[0] << "\t" << values[1] << "\t" << " - ikj 32*32" << std::endl;
		} else {
			ofile1 << values[0] << "\t";
			ofile2 << values[1] << "\t";
		}

		//b*b ikj
		//3 * b * b = 256 * 1024 / 4 (L2)  b~147.8
		//3 * b * b = 32 * 1024 / 4 (L1) b~52.25
		block_size = 48;
/*
 		if (counter_num == 0) {
 			block_size = 48;
 		} else {
 			block_size = 144;
 		}
*/
		if ((retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK) {
			handle_error(retval);
		}
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
		if ((retval = PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK) {
			handle_error(retval);
		}
		if (!stat_flag) {
			std::cout << values[0] << "\t" << values[1] << "\t" << " - ikj " << block_size << "*" << block_size << std::endl;
		} else {
			ofile1 << values[0] << std::endl;
			ofile2 << values[1] << std::endl;	
		}
	} else {
		if (!stat_flag) {
			std::cout << "Proc time" << "\t" << "MFLOP" << std::endl;	
		}
		ofile1_name = "results/proc_time.txt";
		ofile2_name = "results/mflops.txt";
		std::ofstream ofile1 (ofile1_name, std::ios_base::app);
		std::ofstream ofile2 (ofile2_name, std::ios_base::app);
		float real_time, proc_time, mflops;
		long long flpins;

		//32*32 ijk
		block_size = 32;
		if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
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
		if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
			handle_error(retval);
		}
		PAPI_shutdown();
		C_reset(size, C);
		if (!stat_flag) {
			std::cout << proc_time << "\t" << mflops << "\t" << " - ijk 32*32" << std::endl;
		} else {
			ofile1 << proc_time << "\t";
			ofile2 << mflops << "\t";	
		}
		
		//32*32 ikj
		block_size = 32;
		if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
			handle_error(retval);
		}
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
		if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
			handle_error(retval);
		}
		PAPI_shutdown();
		C_reset(size, C);
		if (!stat_flag) {
			std::cout << proc_time << "\t" << mflops << "\t" << " - ikj 32*32" << std::endl;	
		} else {
			ofile1 << proc_time << "\t";
			ofile2 << mflops << "\t";	
		}

		//b*b ikj
		//3 * b * b = 256 * 1024 / 4 (L2)  b~147.8
		//3 * b * b = 32 * 1024 / 4 (L1) b~52.25
		//block_size = 144;
 		block_size = 48;
		if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
			handle_error(retval);
		}
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
		if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
			handle_error(retval);
		}
		if (!stat_flag) {
			std::cout << proc_time << "\t" << mflops << "\t" << " - ikj " << block_size << "*" << block_size << std::endl;	
		} else {
			ofile1 << proc_time << std::endl;
			ofile2 << mflops << std::endl;			
		}
	}
}

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
	int stat_flag {std::atoi(argv[3])};
	if (!stat_flag) {
		std::cout << "Choose counter pair number:" << std::endl
			<< "0 - CPU cycles / Level 1 cache misses" << std::endl
			<< "1 - Total translation lookaside buffer misses / Level 2 cache misses" << std::endl
			<< "2 - Proc time / MFLOP" << std::endl;	
	}
	int pair_num;
	if (stat_flag) {
		pair_num = std::atoi(argv[4]);
		std::cout << "RRRRR" << std::endl;	
	} else {
		std::cin >> pair_num;
	}
	mul(size, A, B, stat_flag, pair_num);
	return 0;
}
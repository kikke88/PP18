#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "papi.h"

#define NUM_EVENTS 3

void handle_error (int retval)
{
    printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
    exit(1);
}

void C_reset(int size, float** C)
{
	for (std::size_t i {0}; i < size; ++i) {
		for (std::size_t j {0}; j < size; ++j) {
			C[i][j] = 0;
		}
	}
}

void mul(std::size_t size, float** A, float** B, float** C, int stat_flag, int counter_num)
{
	std::size_t block_size;
	std::size_t i, j, k, m, n, l, m_min, n_min, l_min;
	int retval;
	std::string ofile1_name;
	std::string ofile2_name;
	if (counter_num == 0) {
		std::string first_event, second_event, third_event;
		first_event = "CPU cycles";
		second_event = "Level 1 data cache misses";
		third_event = "Level 2 data cache misses";
		std::string ofile3_name;
		ofile1_name = "results/CpuCycles";
		ofile2_name = "results/L1DataMisses";
		ofile3_name = "results/L2DataMisses";
		std::ofstream ofile1 (ofile1_name, std::ios_base::app);
		std::ofstream ofile2 (ofile2_name, std::ios_base::app);
		std::ofstream ofile3 (ofile3_name, std::ios_base::app);
		int Events[NUM_EVENTS] {PAPI_TOT_CYC, PAPI_L1_DCM, PAPI_L2_DCM};
		long long values[NUM_EVENTS];
		if (!stat_flag) {
			std::cout << first_event << "\t" << second_event << "\t" << third_event << std::endl;
		}
		//32*32 ijk
		block_size = 32;
		if ((retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK) {
			handle_error(retval);
		}
		for (i = 0; i < size; i += block_size) {
			for (j = 0; j < size; j += block_size) {
				for (k = 0; k < size; k += block_size) {
					for (m = i, m_min = std::min(size, i + block_size); m < m_min; ++m) {
						for (n = j, n_min = std::min(size, j + block_size); n < n_min; ++n) {
							for (l = k, l_min = std::min(size, k + block_size); l < l_min; ++l) {
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
			std::cout << values[0] << "\t"
						<< values[1] << "\t"
						<< values[2] << "\t"
						<< " - ijk 32*32" << std::endl;
		} else {
			ofile1 << size << " " << values[0] << "\t";
			ofile2 << size << " " << values[1] << "\t";
			ofile3 << size << " " << values[2] << "\t";
		}

		//32*32 ikj
		block_size = 32;
		if ((retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK) {
			handle_error(retval);
		}
		for (i = 0; i < size; i += block_size) {
			for (k = 0; k < size; k += block_size) {
				for (j = 0; j < size; j += block_size) {
					for (m = i, m_min = std::min(size, i + block_size); m < m_min; ++m) {
						for (l = k, l_min = std::min(size, k + block_size); l < l_min; ++l) {
							for (n = j, n_min = std::min(size, j + block_size); n < n_min; ++n) {
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
			std::cout << values[0] << "\t"
						<< values[1] << "\t"
						<< values[2] << "\t"
						<< " - ikj 32*32" << std::endl;
		} else {
			ofile1 << size << " " << values[0] << "\t";
			ofile2 << size << " " << values[1] << "\t";
			ofile3 << size << " " << values[2] << "\t";
		}
		//block_size = 70;polus
		block_size = 50;
		if ((retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK) {
			handle_error(retval);
		}
		for (i = 0; i < size; i += block_size) {
			for (k = 0; k < size; k += block_size) {
				for (j = 0; j < size; j += block_size) {
					for (m = i, m_min = std::min(size, i + block_size); m < m_min; ++m) {
						for (l = k, l_min = std::min(size, k + block_size); l < l_min; ++l) {
							for (n = j, n_min = std::min(size, j + block_size); n < n_min; ++n) {
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
			std::cout << values[0] << "\t" 
						<< values[1] << "\t"
						<< values[2] << "\t" 
						<< " - ikj " << block_size << "*" << block_size << std::endl;
		} else {
			ofile1 << size << " " << values[0] << std::endl;
			ofile2 << size << " " << values[1] << std::endl;	
			ofile3 << size << " " << values[2] << std::endl;
		}
	} else {
		if (!stat_flag) {
			std::cout << "Proc time" << "\t" << "MFLOP" << std::endl;	
		}
		ofile1_name = "results/ProcTime";
		ofile2_name = "results/MFlops";
		std::ofstream ofile1 (ofile1_name, std::ios_base::app);
		std::ofstream ofile2 (ofile2_name, std::ios_base::app);
		float real_time, proc_time, mflops;
		long long flpins;

		//32*32 ijk
		block_size = 32;
		if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
			handle_error(retval);
		}
		for (i = 0; i < size; i += block_size) {
			for (j = 0; j < size; j += block_size) {
				for (k = 0; k < size; k += block_size) {
					for (m = i, m_min = std::min(size, i + block_size); m < m_min; ++m) {
						for (n = j, n_min = std::min(size, j + block_size); n < n_min; ++n) {
							for (l = k, l_min = std::min(size, k + block_size); l < l_min; ++l) {
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
			std::cout << proc_time << "\t"
						<< mflops << "\t"
						<< " - ijk 32*32" << std::endl;
		} else {
			ofile1 << size << " " << proc_time << "\t";
			ofile2 << size << " " << mflops << "\t";	
		}
		
		//32*32 ikj
		block_size = 32;
		if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
			handle_error(retval);
		}
		for (i = 0; i < size; i += block_size) {
			for (k = 0; k < size; k += block_size) {
				for (j = 0; j < size; j += block_size) {
					for (m = i, m_min = std::min(size, i + block_size); m < m_min; ++m) {
						for (l = k, l_min = std::min(size, k + block_size); l < l_min; ++l) {
							for (n = j, n_min = std::min(size, j + block_size); n < n_min; ++n) {
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
			std::cout << proc_time << "\t"
						<< mflops << "\t"
						<< " - ikj 32*32" << std::endl;	
		} else {
			ofile1 << size << " " << proc_time << "\t";
			ofile2 << size << " " << mflops << "\t";	
		}
 		//block_size = 70; polus
 		block_size = 50;
		if ((retval = PAPI_flops(&real_time, &proc_time, &flpins, &mflops)) != PAPI_OK) {
			handle_error(retval);
		}
		for (i = 0; i < size; i += block_size) {
			for (k = 0; k < size; k += block_size) {
				for (j = 0; j < size; j += block_size) {
					for (m = i, m_min = std::min(size, i + block_size); m < m_min; ++m) {
						for (l = k, l_min = std::min(size, k + block_size); l < l_min; ++l) {
							for (n = j, n_min = std::min(size, j + block_size); n < n_min; ++n) {
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
			std::cout << proc_time << "\t"
						<< mflops << "\t"
						<< " - ikj " << block_size << "*" << block_size << std::endl;	
		} else {
			ofile1 << size << " " << proc_time << std::endl;
			ofile2 << size << " " << mflops << std::endl;			
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

	//std::vector<std::vector<float>> A(size, std::vector<float>(size, 0)),
	//								B(size, std::vector<float>(size, 0));
	float** A = new float* [size];
	for(int i = 0; i < size; ++i) {
		A[i] = new float [size];
	}
	float** B = new float* [size];
	for(int i = 0; i < size; ++i) {
		B[i] = new float [size];
	}
	float** C = new float* [size];
	for(int i = 0; i < size; ++i) {
		C[i] = new float [size];
	}

	float tmp;
	for (std::size_t i {0}; i < size; ++i) {
		for (std::size_t j {0}; j < size; ++j) {
			arrA.read((char*)& tmp, sizeof tmp);
			A[i][j] = tmp;
			arrB.read((char*)& tmp, sizeof tmp);
			B[i][j] = tmp;
			C[i][j] = 0;
		}
	}
	arrA.close();
	arrB.close();
	int stat_flag {std::atoi(argv[3])};
	if (!stat_flag) {
		std::cout << "Choose counter set:" << std::endl
			<< "0 - CPU cycles / Level 1 data cache misses / Level 2 data cache misses" << std::endl
			<< "1 - Proc time / MFLOP" << std::endl;	
	}
	int pair_num;
	if (stat_flag) {
		pair_num = std::atoi(argv[4]);
	} else {
		std::cin >> pair_num;
	}
	mul(size, A, B, C, stat_flag, pair_num);
	for (std::size_t i {0}; i < size; ++i) {
    	delete[] A[i];
    	delete[] B[i];
    	delete[] C[i];
    }
    delete [] A;
    delete [] B;
    delete [] C;
	return 0;
}
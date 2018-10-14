#include <iostream>
#include <papi.h>
#include <vector>

int main() {
	float real_time, proc_time, mflops;
	long long flpins;
	int Events[2] = {PAPI_TOT_CYC, PAPI_FP_OPS};
	long long values[2];
	
	std::size_t size {600};
	std::vector<std::vector<float>> A(size, std::vector<float>(size, 0)),
									B(size, std::vector<float>(size, 0)),
									C(size, std::vector<float>(size, 0));
	//PAPI_start_counters(Events, 2);
	PAPI_flops( &real_time, &proc_time, &flpins, &mflops);
	for (std::size_t m {0}; m < size; ++m) {
		for (std::size_t n {0}; n < size; ++n) {
			for (std::size_t l {0}; l < size; ++l) {
				C[m][n] += A[m][l] * B[l][n];
			}
		}
	}
	//PAPI_stop_counters(values, 2);
	PAPI_flops( &real_time, &proc_time, &flpins, &mflops);
	std::cout << real_time << std::endl << proc_time << std::endl << flpins << std::endl << mflops << std::endl;
	PAPI_shutdown();
	PAPI_flops( &real_time, &proc_time, &flpins, &mflops);
	for (std::size_t m {0}; m < size; ++m) {
		for (std::size_t n {0}; n < size; ++n) {
			for (std::size_t l {0}; l < size; ++l) {
				C[m][n] += A[m][l] * B[l][n];
			}
		}
	}
	//PAPI_stop_counters(values, 2);
	PAPI_flops( &real_time, &proc_time, &flpins, &mflops);
	std::cout << real_time << std::endl << proc_time << std::endl << flpins << std::endl << mflops << std::endl;
	//std::cout << "tot_cyc - " << values[0] << std::endl
	//			<< "fp_ops - " << values[1] << std::endl;

	return 0;
}
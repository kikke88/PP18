#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <mpi.h>

int main(int argc, char* argv[])
{   
	MPI_Init(&argc, &argv);	
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (size == 1) {
		std::cerr << "Need more then 1 mpi nodes" << std::endl;
		exit(1);
	}
	double my_time = 0, max_time = 0, total_time = 0;
	my_time -= MPI_Wtime();
	int l_border = std::atoi(argv[1]), r_border = std::atoi(argv[2]),
		sqrt_from_r_border = std::sqrt(r_border), arr_size = sqrt_from_r_border / 32 + 1;
	int* sqrt_prime = new int [arr_size];
	for (int i = 0; i < arr_size; ++i) {
		sqrt_prime[i] = -1;
	}
	for (int i = 2; i * i <= sqrt_from_r_border; ++i) {
		if (sqrt_prime[i >> 5] & 1 << (i & 31)) {
			for (int j = i * i; j <= sqrt_from_r_border; j += i) {
				sqrt_prime[j >> 5] &= ~(1 << (j & 31));    
			}
		}
	}
	int prime_before_sqrt_arr_size = 0;
	for (int i = 2; i <= sqrt_from_r_border; ++i) {
		if (sqrt_prime[i >> 5] & 1 << (i & 31)) {
			++prime_before_sqrt_arr_size;
		}
	}
	int* prime_before_sqrt = new int [prime_before_sqrt_arr_size];
	for (int i = 2, j = 0; i <= sqrt_from_r_border; ++i) {
		if (sqrt_prime[i >> 5] & 1 << (i & 31)) {
			prime_before_sqrt[j] = i;
			++j;
		}
	}
	delete[] sqrt_prime;
	int new_l_border = std::max(l_border, sqrt_from_r_border + 1);
	int buf_size = ((r_border - new_l_border) / (size - 1) + 1) / 32 + 1;
	int* buf = new int [buf_size];
	if (!rank) {
		std::ofstream ofile (argv[3]);
		int prime_numbers_counter = 0;
		if (l_border < sqrt_from_r_border + 1) {//   ------A----sqrt(B)+1------
			int tmp_index = prime_before_sqrt_arr_size - 1;
			while (tmp_index >= 0 and prime_before_sqrt[tmp_index] >= l_border) {
				ofile << prime_before_sqrt[tmp_index] << std::endl;
				--tmp_index;
				++prime_numbers_counter;
			}
		}
		MPI_Status status;
		for (int cur_l, cur_r, i = 1; i < size; ++i) {
			MPI_Recv(buf, buf_size, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			cur_l = (r_border - new_l_border + 1) * 1ll * (status.MPI_SOURCE - 1) / (size - 1) + new_l_border;
			cur_r = (r_border - new_l_border + 1) * 1ll * status.MPI_SOURCE / (size - 1) + new_l_border - 1;
			for (int max = cur_r - cur_l + 1, j = 0; j < max; ++j) {
				if (buf[j >> 5] & 1 << (j & 31)) {
					ofile << j + cur_l << std::endl;
					++prime_numbers_counter;
				}
			}
		}
		my_time += MPI_Wtime();
/*		total_time += my_time;
 		if (my_time > max_time) {
 			max_time = my_time;
 		}
 		double i_time;
 		for (int i = 1; i < size; ++i) {
 			MPI_Recv(&i_time, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
 			if (i_time > max_time) {
 				max_time = i_time;
 			}
 			total_time += i_time;
 		}
*/	
		std::cout << rank << "   " << my_time << std::endl;
		std::cout << "total prime numbers - " << prime_numbers_counter << std::endl;
	} else {
		for (int i = 0; i < buf_size; ++i) {
			buf[i] = -1;
		}
		int own_l_border = (r_border - new_l_border + 1) * 1ll * (rank - 1) / (size - 1) + new_l_border;
		int own_r_border = (r_border - new_l_border + 1) * 1ll * rank / (size - 1) + new_l_border - 1;
		int delta = own_r_border - own_l_border + 1;
		for (int cur_prime, j, i = 0; i < prime_before_sqrt_arr_size; ++i) {
			cur_prime = prime_before_sqrt[i];
			for (int k = 0; k < cur_prime; ++k) {
				if ((own_l_border + k) % cur_prime == 0) {
					j = k;
					break;
				}
			}
			for (int k = j; k < delta; k += cur_prime) {
				buf[k >> 5] &= ~(1 << (k & 31));
			}
		}
		MPI_Send(buf, buf_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
		//MPI_Request req;
		//MPI_Isend(buf, buf_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
		my_time += MPI_Wtime();
		std::cout << rank << "   " << my_time << std::endl;
		//MPI_Send(&my_time, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	}
	MPI_Reduce(&my_time, &total_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&my_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	delete[] buf;
	if (!rank) {
		std::cout << "total time - " << total_time << std::endl;
		std::cout << "max time - " << max_time << std::endl;
	}
	MPI_Finalize();
	return 0;
}

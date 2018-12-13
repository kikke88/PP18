#include <iostream>
#include <fstream>
#include <cmath>
#include <mpi.h>

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);	
	int rank, size, cub;
	double compute_time = 0, IO_time = 0;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int ndims = 3, reorder = false;
	int i = 1;
	while (i * i * i != size) {
		++i;
	}
	cub = i;
	int dims[3] = {cub, cub, cub};
	int periods[3] = {false, false, false};
	MPI_Comm CUB_COMM, LEVEL_COMM;
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &CUB_COMM);
	MPI_Comm_rank(CUB_COMM, &rank);	
	int coords[3];
	MPI_Cart_coords(CUB_COMM, rank, 3, coords);
	std::size_t arr_size;
	int arr_size_int;
	int array_of_subsizes[2];
	MPI_File fh_A, fh_B;
	MPI_Datatype SUBARR_TYPE;
	MPI_Barrier(CUB_COMM);
	IO_time -= MPI_Wtime();
	int remain_dims[3] = {false, true, true};
	MPI_Cart_sub(CUB_COMM, remain_dims, &LEVEL_COMM);
	if (sizeof(std::size_t) == 8) {
		if (coords[0] == 0) {
			MPI_File_open(LEVEL_COMM, argv[1],  MPI_MODE_RDONLY, MPI_INFO_NULL, &fh_A);
			if (rank == 0) {
				char type;
				MPI_File_read(fh_A, &type, 1, MPI_CHAR, MPI_STATUS_IGNORE);
				MPI_File_read(fh_A, &arr_size, 1, MPI_LONG_LONG, MPI_STATUS_IGNORE);
				MPI_File_read(fh_A, &arr_size, 1, MPI_LONG_LONG, MPI_STATUS_IGNORE);
				MPI_Bcast(&arr_size, 1, MPI_LONG_LONG, 0, CUB_COMM);
			} else {
				MPI_Bcast(&arr_size, 1, MPI_LONG_LONG, 0, CUB_COMM);
			}
			MPI_File_close(&fh_A);
		} else {
			MPI_Bcast(&arr_size, 1, MPI_LONG_LONG, 0, CUB_COMM);
		}
	} else {
		if (coords[0] == 0) {
			MPI_File_open(LEVEL_COMM, argv[1],  MPI_MODE_RDONLY, MPI_INFO_NULL, &fh_A);
			if (rank == 0) {
				char type;
				MPI_File_read(fh_A, &type, 1, MPI_CHAR, MPI_STATUS_IGNORE);
				MPI_File_read(fh_A, &arr_size, 1, MPI_INT, MPI_STATUS_IGNORE);
				MPI_File_read(fh_A, &arr_size, 1, MPI_INT, MPI_STATUS_IGNORE);
				MPI_Bcast(&arr_size, 1, MPI_INT, 0, CUB_COMM);
			} else {
				MPI_Bcast(&arr_size, 1, MPI_INT, 0, CUB_COMM);
			}
			MPI_File_close(&fh_A);
		} else {
			MPI_Bcast(&arr_size, 1, MPI_INT, 0, CUB_COMM);
		}
	}
	arr_size_int = arr_size;
	array_of_subsizes[0] = array_of_subsizes[1] =  arr_size / cub;
	if (arr_size % cub) {
		++array_of_subsizes[0];
		++array_of_subsizes[1];
	}
	if (coords[1] == cub - 1) {
		array_of_subsizes[1] = arr_size - coords[1] * array_of_subsizes[1];
	}
	if (coords[2] == cub - 1) {
		array_of_subsizes[0] = arr_size - coords[2] * array_of_subsizes[0];
	}
	double* sub_arr_A;
	double* sub_arr_B;
	if (coords[0] == 0) {
		double* lower_level_subarr_A = new double [array_of_subsizes[0] * array_of_subsizes[1]];
		double* lower_level_subarr_B = new double [array_of_subsizes[0] * array_of_subsizes[1]];
		int shift = arr_size / cub;
		if (arr_size % cub) {
			++shift;
		}
		int start = coords[1] * shift;
		MPI_File_open(LEVEL_COMM, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fh_A);
		MPI_File_open(LEVEL_COMM, argv[2], MPI_MODE_RDONLY, MPI_INFO_NULL, &fh_B);////////
		MPI_Type_create_subarray(1, &arr_size_int, &array_of_subsizes[1], &start, MPI_ORDER_C, MPI_DOUBLE, &SUBARR_TYPE);
		MPI_Type_commit(&SUBARR_TYPE);
		MPI_Offset offset = sizeof(std::size_t) * 2 + sizeof(char) + (coords[2] * arr_size * shift) * sizeof(double);
		MPI_File_set_view(fh_A, offset, MPI_DOUBLE, SUBARR_TYPE, "native", MPI_INFO_NULL);
		MPI_File_set_view(fh_B, offset, MPI_DOUBLE, SUBARR_TYPE, "native", MPI_INFO_NULL);
		MPI_File_read_all(fh_A, lower_level_subarr_A, array_of_subsizes[0] * array_of_subsizes[1], MPI_DOUBLE, MPI_STATUS_IGNORE);
		MPI_File_read_all(fh_B, lower_level_subarr_B, array_of_subsizes[0] * array_of_subsizes[1], MPI_DOUBLE, MPI_STATUS_IGNORE);
		if (coords[1] > 0) {
			int send_rank_A;
			int send_coords[3] = {coords[1], coords[1], coords[2]};
			MPI_Cart_rank(CUB_COMM, send_coords, &send_rank_A);
			MPI_Send(lower_level_subarr_A, array_of_subsizes[0] * array_of_subsizes[1], MPI_DOUBLE, send_rank_A, 1, CUB_COMM);
			delete[] lower_level_subarr_A;
		} else {
			sub_arr_A = lower_level_subarr_A;
		}
		if (coords[2] > 0) {
			int send_rank_B;
			int send_coords[3] = {coords[2], coords[1], coords[2]};
			MPI_Cart_rank(CUB_COMM, send_coords, &send_rank_B);
			MPI_Send(lower_level_subarr_B, array_of_subsizes[0] * array_of_subsizes[1], MPI_DOUBLE, send_rank_B, 2, CUB_COMM);
			delete[] lower_level_subarr_B;
		} else {
			sub_arr_B = lower_level_subarr_B;
		}
	} else {
		if (coords[0] == coords[1]) {
			sub_arr_A = new double [array_of_subsizes[0] * array_of_subsizes[1]];
			int recv_rank_A;
			int recv_coords[3] = {0, coords[1], coords[2]};
			MPI_Cart_rank(CUB_COMM, recv_coords, &recv_rank_A);
			MPI_Recv(sub_arr_A, array_of_subsizes[0] * array_of_subsizes[1], MPI_DOUBLE, recv_rank_A, 1, CUB_COMM, MPI_STATUS_IGNORE);
		}
		if (coords[0] == coords[2]) {
			sub_arr_B = new double [array_of_subsizes[0] * array_of_subsizes[1]];
			int recv_rank_B;
			int recv_coords[3] = {0, coords[1], coords[2]};
			MPI_Cart_rank(CUB_COMM, recv_coords, &recv_rank_B);
			MPI_Recv(sub_arr_B, array_of_subsizes[0] * array_of_subsizes[1], MPI_DOUBLE, recv_rank_B, 2, CUB_COMM, MPI_STATUS_IGNORE);
		}
	}
	MPI_Comm LINES_COMM, COLUMNS_COMM, VERTICAL_COMM;
	remain_dims[0] = false;	remain_dims[1] = true;	remain_dims[2] = false;
	MPI_Cart_sub(CUB_COMM, remain_dims, &LINES_COMM);
	int root_coord[3] = {coords[0], coords[0], coords[2]};
	int root_rank;
	MPI_Cart_rank(LINES_COMM, root_coord, &root_rank);
	int array_of_subsizes_b[2] = {array_of_subsizes[0], array_of_subsizes[1]};
	MPI_Bcast(array_of_subsizes, 2, MPI_INT, root_rank, LINES_COMM);
	if (coords[0] != coords[1]) {
		sub_arr_A = new double [array_of_subsizes[0] * array_of_subsizes[1]];
	}
	MPI_Bcast(sub_arr_A, array_of_subsizes[0] * array_of_subsizes[1], MPI_DOUBLE, root_rank, LINES_COMM);
	remain_dims[0] = false;	remain_dims[1] = false;	remain_dims[2] = true;
	MPI_Cart_sub(CUB_COMM, remain_dims, &COLUMNS_COMM);
	root_coord[0] = coords[0]; root_coord[1] = coords[1]; root_coord[2] = coords[0];
	MPI_Cart_rank(COLUMNS_COMM, root_coord, &root_rank);	
	MPI_Bcast(array_of_subsizes_b, 2, MPI_INT, root_rank, COLUMNS_COMM);
	if (coords[0] != coords[2]) {
		sub_arr_B = new double [array_of_subsizes_b[0] * array_of_subsizes_b[1]];
	}
	MPI_Bcast(sub_arr_B, array_of_subsizes_b[0] * array_of_subsizes_b[1], MPI_DOUBLE, root_rank, COLUMNS_COMM);
	MPI_Comm_free(&LINES_COMM);
	MPI_Comm_free(&COLUMNS_COMM);
	double* sub_arr_C = new double [array_of_subsizes[0] * array_of_subsizes_b[1]];
	for (std::size_t i = 0; i < array_of_subsizes[0] * array_of_subsizes_b[1]; ++i) {
		sub_arr_C[i] = 0;
	}
	IO_time += MPI_Wtime();
	MPI_Barrier(CUB_COMM);
	compute_time -= MPI_Wtime();
	for (std::size_t i = 0; i < array_of_subsizes[0]; ++i) {
		for (std::size_t k = 0; k < array_of_subsizes[1]; ++k) {
			for (std::size_t j = 0; j < array_of_subsizes_b[1]; ++j) {
				sub_arr_C[i * array_of_subsizes_b[1] + j] += sub_arr_A[i * array_of_subsizes[1] + k]  * sub_arr_B[k * array_of_subsizes_b[1] +j];
			}
		}
	}
	remain_dims[0] = true;	remain_dims[1] = false;	remain_dims[2] = false;
	MPI_Cart_sub(CUB_COMM, remain_dims, &VERTICAL_COMM);
	root_coord[0] = 0; root_coord[1] = coords[1]; root_coord[2] = coords[2];
	MPI_Cart_rank(VERTICAL_COMM, root_coord, &root_rank);
	if (coords[0] == 0) {
		MPI_Reduce(MPI_IN_PLACE, sub_arr_C, array_of_subsizes[0] * array_of_subsizes_b[1], MPI_DOUBLE, MPI_SUM, root_rank, VERTICAL_COMM);	
	} else {
		MPI_Reduce(sub_arr_C, NULL, array_of_subsizes[0] * array_of_subsizes_b[1], MPI_DOUBLE, MPI_SUM, root_rank, VERTICAL_COMM);	
	}
	compute_time += MPI_Wtime();
	MPI_Barrier(CUB_COMM);
	IO_time -= MPI_Wtime();
	MPI_Comm_free(&VERTICAL_COMM);
	if (coords[0] == 0) {
		MPI_File fh_C;
		MPI_File_open(LEVEL_COMM, argv[3], MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &fh_C);
		if (coords[0] == 0 && coords[1] == 0 && coords[2] == 0) {
			char type = 'd';
			MPI_File_write(fh_C, &type, 1, MPI_CHAR, MPI_STATUS_IGNORE);
			MPI_File_write(fh_C, &arr_size, 1, MPI_LONG_LONG, MPI_STATUS_IGNORE);
			MPI_File_write(fh_C, &arr_size, 1, MPI_LONG_LONG, MPI_STATUS_IGNORE);	
		}
		int shift = arr_size / cub;
		if (arr_size % cub) {
			++shift;
		}
		MPI_Offset offset = sizeof(std::size_t) * 2 + sizeof(char) + (coords[2] * arr_size * shift) * sizeof(double);
		MPI_File_set_view(fh_C, offset, MPI_DOUBLE, SUBARR_TYPE, "native", MPI_INFO_NULL);
		MPI_File_write_all(fh_C, sub_arr_C, array_of_subsizes[0] * array_of_subsizes_b[1], MPI_DOUBLE, MPI_STATUS_IGNORE);
	}
	IO_time += MPI_Wtime();
	if (rank == 0) {
		MPI_Reduce(MPI_IN_PLACE, &IO_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
		MPI_Reduce(MPI_IN_PLACE, &compute_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
		std::ofstream ofile ("OFILE", std::ios::app);
			
		ofile << "Size " << arr_size << std::endl
					<< "Proc num " << size << std::endl
					<< "IO time " << IO_time << std::endl
					<< "Compute time " << compute_time << std::endl;
	} else {
		MPI_Reduce(&IO_time, NULL, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
		MPI_Reduce(&compute_time, NULL, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	}
	MPI_Comm_free(&CUB_COMM);
	delete[] sub_arr_C;
	MPI_Finalize();
	return 0;
}

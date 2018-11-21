#include <iostream>
#include <fstream>
#include <mpi.h>

int main(int argc, char* argv[])//A, b, c 
{   
	MPI_Init(&argc, &argv);	
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	std::size_t lines, columns, first, second, delta;
	MPI_Status status;
	if (!rank) {
		std::ifstream A(argv[1], std::ios::binary), b(argv[2], std::ios::binary);
		char type;
		A.read((char*)& type, sizeof type);
		A.read((char*)& lines, sizeof lines);
		A.read((char*)& columns, sizeof columns);
		double* tmp {new double [lines * columns]};
		double** array_A {new double* [lines]};
		for (std::size_t i {0}; i < lines; ++i) {
			array_A[i] = tmp + i * columns;
		}
		double* array_b {new double [columns]};
		double elem;
		for (std::size_t i {0}; i < lines; ++i) {
			for (std::size_t j {0}; j < columns; ++j) {
				A.read((char*)& elem, sizeof elem);
				array_A[i][j] = elem;
			}
		}
		for (std::size_t i {0}; i < columns; ++i) {
			b.read((char*)& elem, sizeof elem);
			array_b[i] = elem;
		}
		for (std::size_t i {1}; i < size; ++i) {
			MPI_Send(&lines, 1, MPI_UNSIGNED_LONG, i, 0, MPI_COMM_WORLD);
			MPI_Send(&columns, 1, MPI_UNSIGNED_LONG, i, 1, MPI_COMM_WORLD);
		}
		if (lines >= columns) {
			first = lines * rank / size;
			second = lines * (rank + 1) / size;
			delta = second - first;
			int* scounts {new int [size]};
			int* displs {new int [size]};
			for (std::size_t i {0}; i < size; ++i) {
				scounts[i] = columns * (lines * (i + 1) / size - lines * i / size);
				displs[i] = columns * (lines * i / size); 
			}
			MPI_Scatterv(tmp, scounts, displs, MPI_DOUBLE, MPI_IN_PLACE, 0, 
				MPI_DOUBLE, 0, MPI_COMM_WORLD);
			for (std::size_t i {1}; i < size; ++i) {
				MPI_Send(array_b, columns, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
			}
			delete array_b;
			double* array_c {new double [lines]};
			for (std::size_t i {0}; i < delta; ++i) {
				array_c[i] = 0;
			}
			for (std::size_t j {0}; j < delta; ++j) {
				for (std::size_t i {0}; i < columns; ++i) {
					array_c[j] += tmp[i][j] * array_b[i]; 
				}
			}
			delete[] array_A;
			delete[] tmp;
			for (std::size_t i {0}; i < size; ++i) {
				scounts[i] /= columns;
				displs[i] /= columns;
			}
			MPI_Gatherv(MPI_IN_PLACE, delta, MPI_DOUBLE, array_c, scounts,
				displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
			delete[] scounts;
			delete[] displs;
			std::ofstream c (argv[3], std::ios::binary);
			char type = 'd';
			c.write((char*)&type, sizeof type);
			c.write((char*)&lines, sizeof lines);
			c.write((char*)&columns, sizeof columns);
			double cur_elem;
			for (std::size_t i {0}; i < lines; ++i) {
				c.write((char*)&array_c[i], sizeof(double));
			}
			delete array_c;
		} else {
			first = columns * rank / size;
			second = columns * (rank + 1) / size;
			delta = second - first;
			double* big_line = new double [lines * delta * 2];
			std::size_t index {0}, delta_i;
			for (std::size_t i {1}; i < size; ++i) {
				delta_i = columns * (i + 1) / size - columns * i / size;
					for (std::size_t j {0}; j < lines; ++j) {
						for (std::size_t k = columns * i / size; k < delta_i + columns * i / size; ++k) {
							big_line[index++] = array_A[j][k];
						}
					}
					MPI_Send(big_line, lines * delta_i, MPI_DOUBLE, i, 3, MPI_COMM_WORLD);
					index = 0;
					MPI_Send(array_b, delta_i, MPI_DOUBLE, i, 4, MPI_COMM_WORLD);
			}
			delete big_line;
			double* array_c {new double [lines]};
			for (std::size_t i {0}; i < lines; ++i) {
				array_c[i] = 0;
			}
			for (std::size_t i {0}; i < lines; ++i) {
				for (std::size_t j {0}; j < delta; ++j) {
					array_c[i] += array_A[i][j] * array_b[j];
				}
			}
			delete[] array_A;
			delete[] tmp;
			MPI_Reduce(MPI_IN_PLACE, array_c, lines, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
			std::ofstream c (argv[3], std::ios::binary);
			char type = 'd';
			c.write((char*)&type, sizeof type);
			c.write((char*)&lines, sizeof lines);
			c.write((char*)&columns, sizeof columns);
			double cur_elem;
			for (std::size_t i {0}; i < lines; ++i) {
				c.write((char*)&array_c[i], sizeof(double));
			}
			delete array_c;
		}


	} else {//1..size-1//
		
		MPI_Recv(&lines, 1, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&columns, 1, MPI_UNSIGNED_LONG, 0, 1, MPI_COMM_WORLD, &status);
		if (lines >= columns) {
			first = lines * rank / size;
			second = lines * (rank + 1) / size;
			delta = second - first;
			double* tmp_arr_A {new double [columns * delta]};
			double** sub_arr_A {new double* [delta]};
			for (std::size_t i {0}; i < delta; ++i) {
				sub_arr_A[i] = tmp_arr_A + i * columns;
			}
			MPI_Scatterv(NULL, 0, NULL, MPI_DOUBLE, sub_arr_A, columns * delta, 
				MPI_DOUBLE, 0, MPI_COMM_WORLD);
			double* array_b = new double [columns];
			MPI_Recv(array_b, columns, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status);
			double* sub_array_c {new double [delta]};
			for(std::size_t i {0}; i < delta; ++i) {
				sub_array_c[i] = 0;
			}
			for (std::size_t j {0}; j < delta; ++j) {
				for (std::size_t i {0}; i < columns; ++i) {
					sub_array_c[j] += sub_arr_A[i][j] * array_b[i]; 
				}
			}
			delete[] sub_arr_A;
			delete[] tmp_arr_A;
			delete[] array_b;
			MPI_Gatherv(sub_array_c, delta, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD);
			delete[] sub_array_c;

		} else {
			first = columns * rank / size;
			second = columns * (rank + 1) / size;
			delta = second - first;
			double* tmp_arr_A {new double [lines * delta]};
			double** sub_arr_A {new double* [delta]};
			for (std::size_t i {0}; i < delta; ++i) {
				sub_arr_A[i] = tmp_arr_A + i * columns;
			}
			double* sub_array_b {new double [delta]};
			MPI_Recv(tmp_arr_A, lines * delta, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD, &status);
			MPI_Recv(sub_array_b, delta, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD, &status);
			double* array_c {new double [lines]};
			for (std::size_t i {0}; i < lines; ++i) {
				array_c[i] = 0;
			}
			for (std::size_t i {0}; i < lines; ++i) {
				for (std::size_t j {0}; j < delta; ++j) {
					array_c[i] += sub_array_A[i][j] * array_b[j];
				}
			}
			MPI_Reduce(array_c, NULL, lines, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		}
	}	
	
	MPI_Finalize();
	return 0;
}

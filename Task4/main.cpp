#include <iostream>
#include <fstream>
#include <mpi.h>

int main(int argc, char* argv[])//A, b, c 
{   
	MPI_Init(&argc, &argv);	
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	double time = 0;
	std::size_t lines, columns, first, second, delta, shift = sizeof(char) + 2 * sizeof(std::size_t);
	std::FILE* A  = std::fopen(argv[1], "rb");
	std::FILE* b  = std::fopen(argv[2], "rb");
	char type;
	std::fread(&type, sizeof type, 1, A);
	std::fread(&lines, sizeof lines, 1, A);
	std::fread(&columns, sizeof columns, 1, A);
	if (lines >= columns) {
		first = lines * rank / size;
		second = lines * (rank + 1) / size;
		delta = second - first;
		double* tmp_A = new double [delta * columns];
		double** array_A = new double* [delta];
		for (std::size_t i = 0; i < delta; ++i) {
			array_A[i] = tmp_A + i * columns;
		}
		std::fseek(A, first * columns * sizeof(double) + shift, SEEK_SET);
		std::fread(tmp_A, sizeof (double), delta * columns, A);
		std::fclose(A);
		double* array_b = new double [columns];
		std::fseek(b, shift, SEEK_SET);
		std::fread(array_b, sizeof (double), columns, b);
		std::fclose(b);
		double* i_result = new double [delta];
		for (std::size_t i = 0; i < delta; ++i) {
			i_result[i] = 0;
		}
		time -= MPI_Wtime();
		for (std::size_t i = 0; i < delta; ++i) {
			for (std::size_t j = 0; j < columns; ++j) {
				i_result[i] += array_A[i][j] * array_b[j];
			}
		}
		time += MPI_Wtime();
		if (!rank) {
			double* result = new double [lines];	
			if (!(lines % size)) {
				MPI_Barrier(MPI_COMM_WORLD);
				time -= MPI_Wtime();
				MPI_Gather(i_result, delta, MPI_DOUBLE, result, delta, MPI_DOUBLE, 0, MPI_COMM_WORLD);
				time += MPI_Wtime();	
			} else {
				int* recvcounts = new int [size];
				int* displs = new int [size];
				for (std::size_t i = 0; i < size; ++i) {
					recvcounts[i] = (lines * (i + 1) / size - lines * i / size);
					displs[i] = lines * i / size;
				}
				MPI_Barrier(MPI_COMM_WORLD);
				time -= MPI_Wtime();
				MPI_Gatherv(i_result, delta, MPI_DOUBLE, result, recvcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
				time += MPI_Wtime();
				delete[] recvcounts;
				delete[] displs;
			}			
			std::FILE* c = std::fopen(argv[3], "wb");
			std::fwrite(&type, sizeof type, 1, c);
			std::size_t tmp_size = lines;
			std::fwrite(&tmp_size, sizeof tmp_size, 1, c);
			tmp_size = 1;
			std::fwrite(&tmp_size, sizeof tmp_size, 1, c);
			std::fwrite(result, sizeof (double), lines, c);
			std::fclose(c);
			delete[] result;
		} else {
			if(!(lines % size)) {
				MPI_Barrier(MPI_COMM_WORLD);
				time -= MPI_Wtime();
				MPI_Gather(i_result, delta, MPI_DOUBLE, NULL, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);			
				time += MPI_Wtime();
			} else {
				MPI_Barrier(MPI_COMM_WORLD);
				time -= MPI_Wtime();
				MPI_Gatherv(i_result, delta, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD);			
				time += MPI_Wtime();	
			}			
		}
		delete[] tmp_A;
		delete[] array_A;		
		delete[] array_b;
		delete[] i_result;
	} else {
		first = columns * rank / size;
		second = columns * (rank + 1) / size;
		delta = second - first;
		double* tmp_A = new double [lines * delta];
		double** array_A = new double* [lines];
		for (std::size_t i = 0; i < lines; ++i) {
			array_A[i] = tmp_A + i * delta;
		}
		for (std::size_t i = 0; i < lines; ++i) {
			std::fseek(A, (i * columns + first) * sizeof(double) + shift, SEEK_SET);
			std::fread(tmp_A + i * delta, sizeof (double), delta, A);	
		}
		std::fclose(A);
		double* array_b = new double [delta];
		std::fseek(b, first * sizeof(double) + shift, SEEK_SET);
		std::fread(array_b, sizeof (double), delta, b);	
		std::fclose(b);
		double* i_result = new double [lines];
		for (std::size_t i = 0; i < lines; ++i) {
			i_result[i] = 0;
		}
		time -= MPI_Wtime();
		for (std::size_t i = 0; i < lines; ++i) {
			for (std::size_t j = 0; j < delta; ++j) {
				i_result[i] += array_A[i][j] * array_b[j];
			}
		}
		time += MPI_Wtime();
		if (!rank) {
			MPI_Barrier(MPI_COMM_WORLD);
			time -= MPI_Wtime();
			MPI_Reduce(MPI_IN_PLACE, i_result, lines, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
			time += MPI_Wtime();
		} else {
			MPI_Barrier(MPI_COMM_WORLD);
			time -= MPI_Wtime();
			MPI_Reduce(i_result, NULL, lines, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);	
			time += MPI_Wtime();
		}		
		if (!rank) {
			std::FILE* c = std::fopen(argv[3], "wb");
			std::fwrite(&type, sizeof type, 1, c);
			std::size_t tmp_size = lines;
			std::fwrite(&tmp_size, sizeof tmp_size, 1, c);
			tmp_size = 1;
			std::fwrite(&tmp_size, sizeof tmp_size, 1, c);
			std::fwrite(i_result, sizeof (double), lines, c);
			std::fclose(c);
		}
		delete[] array_A;
		delete[] tmp_A;
		delete[] array_b;
		delete[] i_result;
	}
	if (!rank) {
		double max, sum;
		MPI_Reduce(&time, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&time, &max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
		std::ofstream out ("OUT", std::ios::app);
		out << size << std::endl 
			<< "lines - " << lines << "\t" << "columns - " << columns << std::endl 
			<< "sum - " << sum << std::endl 
			<< "max - " << max << std::endl;
	} else {
		MPI_Reduce(&time, NULL, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&time, NULL, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}


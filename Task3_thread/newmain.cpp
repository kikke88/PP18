#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <pthread.h>

typedef struct Info_for_threads {
    int num_threads,
        l_border,
        r_border,
        number_of_prime_num_before_sqrt;
    std::string file_name;
} Info;

typedef struct Ret_struct {
    int counter;
    double time;
} Ret_struct;

pthread_mutex_t write_lock,
                id_lock;

int* prime_num_before_sqrt;
long id = 0;

void *run(void *params)
{   
    Info* inf = (Info*) params;
    std::ofstream ofile (inf -> file_name, std::ios_base::app);
    pthread_mutex_lock(&id_lock);
    long my_id = id;
    ++id;
    pthread_mutex_unlock(&id_lock);
    struct timespec start, finish;
    double elapsed;
    int counter = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int sqrt_from_r_border = std::sqrt(inf -> r_border);
    int new_l_border = std::max(inf -> l_border, sqrt_from_r_border + 1);
    int buf_size = ((inf -> r_border - new_l_border) / inf -> num_threads + 1) / 32 + 1;
    int* buf = new int [buf_size];
    for (int i = 0; i < buf_size; ++i) {
            buf[i] = -1;
    }
    int own_l_border = (inf -> r_border - new_l_border + 1) * 1ll * my_id / inf -> num_threads + new_l_border;
    int own_r_border = (inf -> r_border - new_l_border + 1) * 1ll * (my_id + 1) / inf -> num_threads + new_l_border - 1;
    int delta = own_r_border - own_l_border + 1;
    for (int cur_prime, j, i = 0; i < inf -> number_of_prime_num_before_sqrt; ++i) {
        cur_prime = prime_num_before_sqrt[i];
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
    for (int max = own_r_border - own_l_border + 1, j = 0; j < max; ++j) {
        if (buf[j >> 5] & 1 << (j & 31)) {
            // pthread_mutex_lock(&write_lock);            
            // ofile << j + own_l_border << std::endl;
            // pthread_mutex_unlock(&write_lock);
            ++counter;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    delete[] buf;
    elapsed = (finish.tv_sec - start.tv_sec) + 1e-9 * (finish.tv_nsec - start.tv_nsec);
    Ret_struct* ret = new Ret_struct;
    ret -> counter = counter;
    ret -> time = elapsed;
    //double* ret_value = new double (elapsed);
.}

int main(int argc, char* argv[])
{
    std::cout.precision(9);
    int num_threads = std::atoi(argv[4]),
        number_of_prime_num_before_sqrt,
        //l_border = std::atoi(argv[1]),
        sscanf(argv[1], "%d", &l_border);
        r_border = std::atoi(argv[2]);
    std::string file_name = argv[3];
    pthread_t* threads = new pthread_t [num_threads];
    pthread_mutex_init(&write_lock, NULL);
    //pthread_mutex_init(&counter_lock, NULL);
    pthread_mutex_init(&id_lock, NULL);
    std::ofstream ofile (argv[3]);
    struct timespec start, finish;
    double elapsed;
    int counter = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);    
    int sqrt_from_r_border = std::sqrt(r_border), arr_size = sqrt_from_r_border / 32 + 1;
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
    number_of_prime_num_before_sqrt = 0;
    for (int i = 2; i <= sqrt_from_r_border; ++i) {
        if (sqrt_prime[i >> 5] & 1 << (i & 31)) {
            ++number_of_prime_num_before_sqrt;
        }
    }
    prime_num_before_sqrt = new int [number_of_prime_num_before_sqrt];
    for (int i = 2, j = 0; i <= sqrt_from_r_border; ++i) {
        if (sqrt_prime[i >> 5] & 1 << (i & 31)) {
            prime_num_before_sqrt[j] = i;
            ++j;
        }
    }
    delete[] sqrt_prime;
    Info info {num_threads, l_border, r_border, number_of_prime_num_before_sqrt, file_name};
    for (long i = 0; i < num_threads; ++i){
        pthread_create(threads + i, NULL, run, &info);
    }
    if (l_border < sqrt_from_r_border + 1) {//   ------A----sqrt(B)+1------
        for (int cur_prime, i = number_of_prime_num_before_sqrt - 1; i >= 0; --i) {
            cur_prime = prime_num_before_sqrt[i];
            if (cur_prime >= l_border) {
                // pthread_mutex_lock(&write_lock);
                // ofile << cur_prime << std::endl;
                // pthread_mutex_unlock(&write_lock);
                //pthread_mutex_lock(&counter_lock);
                ++counter;
                //pthread_mutex_unlock(&counter_lock);
            } else {
                break;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec) + 1e-9 * (finish.tv_nsec - start.tv_nsec);
    void* ret;
    Ret_struct i_struct;
    double max_time = elapsed, total_time = elapsed;
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], &ret);
        i_struct = *static_cast<Ret_struct*>(ret);
        delete static_cast<Ret_struct*>(ret);
        total_time += i_struct.time;
        if (i_struct.time > max_time) {
            max_time = i_struct.time;
        }
        counter += i_struct.counter;
        //std::cout << i << "   " << i_time << std::endl;
    }
    std::cout << "max time - " << max_time << " s" << std::endl;
    std::cout << "average time - " << total_time / (num_threads + 1) << std::endl;
    std::cout << "total time - " << total_time << " s" << std::endl;
    std::cout << "total prime number - " << counter << std::endl;
    pthread_mutex_destroy(&write_lock);
    //pthread_mutex_destroy(&counter_lock);
    pthread_mutex_destroy(&id_lock);
    delete[] threads;
    delete[] prime_num_before_sqrt;
    return 0;
}
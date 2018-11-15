#include <iostream>
#include <fstream>
#include <chrono>
//#include <stdio.h>
#include <pthread.h>


typedef struct I {
    pthread_mutex_t mutex;
} Info;

#define NUM_THREADS 10

long aaa = 0;

void *greeting(void *id)
{	
	//std::ofstream ofile ("prime.txt", std::ios_base::app);
	Info* inf = (Info *)id;
	pthread_mutex_lock(&(inf->mutex));
	long my_id = aaa;
	++aaa;
	pthread_mutex_unlock(&(inf->mutex));
	printf("I'm %ld. Hello, World\n", my_id);	
	//for (int i = 0; i < 1000; ++i) {
	//	ofile << i + (long)id << std::endl;
	//}
	pthread_exit((void *)my_id);
}

int main() {
	pthread_t threads[NUM_THREADS];
	Info param;
	pthread_mutex_init(&(param.mutex), NULL);
	auto begin = std::chrono::steady_clock::now();
	Info inf;
	for(long id = 0; id < NUM_THREADS; id++){
		
		pthread_create(threads+id, NULL, greeting, &param);
	}
	auto end = std::chrono::steady_clock::now();  
 	auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	std::cout << "The time: " << elapsed_ms.count() << " * 10^-9 s" << std::endl;
	long int i {elapsed_ms.count()};
	std::cout << i << std::endl;
	void *status;
	for(int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], &status);
		printf("joined with status: %ld\n", (long) status);
	}
	pthread_mutex_destroy(&(param.mutex));
	std::cout << sizeof(long) << "*******" << sizeof(long long) << std::endl;
	return 0;
}

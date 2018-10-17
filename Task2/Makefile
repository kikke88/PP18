CXX=g++
CFLAGS=-std=c++1z
NECESSARY_PAPI_FILE=/usr/local/lib/libpapi.a
.PHONY: all clean tmp generate_array array_multiplication

all: array_multiplication generate_array tmp
array_multiplication:
	${CXX} -I /usr/local/include/ -o array_multiplication mul.cpp ${NECESSARY_PAPI_FILE}
generate_array:
	${CXX} -o generate_array gen_array.cpp
clean:
	rm -rf tests/* array_multiplication generate_array a.out tmp pmt results/*
tmp:
	${CXX} -I /usr/local/include/ -o tmp tmp.cpp ${NECESSARY_PAPI_FILE}
pmt:
	${CXX} -I /usr/local/include/ -o pmt pmt.cpp ${NECESSARY_PAPI_FILE}

create_tests:
#добавить 3000 4000 5000
	for i in 1000 2000 ; do \
		./generate_array tests/A_$$i.dat  $$i ; \
		./generate_array tests/B_$$i.dat  $$i ; \
	done;
	
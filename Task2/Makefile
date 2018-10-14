CXX=g++
CFLAGS=-std=c++1z
.PHONY: all clean generate_array array_multiplication

all: array_multiplication generate_array
array_multiplication:
	${CXX} -I /usr/local/include/ -o array_multiplication mul.cpp /usr/local/lib/libpapi.a
generate_array:
	${CXX} -o generate_array gen_array.cpp
clean:
	rm -rf tests/* array_multiplication generate_array
create_tests:
#добавить 3000 4000 5000
	for i in 1000 2000 ; do \
		./generate_array tests/A_$$i.dat  $$i ; \
		./generate_array tests/B_$$i.dat  $$i ; \
	done;
mode1:
	
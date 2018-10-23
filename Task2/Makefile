CXX=g++
CFLAGS=-std=c++1z
NECESSARY_PAPI_FILE=/usr/local/lib/libpapi.a
.PHONY: all clean tmp generate_array array_multiplication

all: array_multiplication generate_array tmp
array_multiplication:
	${CXX} -o array_multiplication mul.cpp -lpapi
generate_array:
	${CXX} -o generate_array gen_array.cpp
clean:
	rm -rf tests/* array_multiplication generate_array a.out tmp special_representation pmt results/* *.png
tmp:
	${CXX} -o tmp tmp.cpp -lpapi
pmt:
	${CXX} -o pmt pmt.cpp -lpapi

create_tests: generate_array
#добавить 3000 4000 5000
	for i in 1000 2000 ; do \
		./generate_array tests/A_$$i.dat  $$i ; \
		./generate_array tests/B_$$i.dat  $$i ; \
	done;

res:
#добавить 1 2 3
	for i in 0 ; do \
		for j in 1000 2000 ; do \
			./array_multiplication tests/A_$$j.dat tests/B_$$j.dat  1 $$i ; \
		done; \
	done 
draw:
	./draw.sh
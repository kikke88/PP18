COMPILER=g++ 
MPI_COMPILER=mpicxx
CFLAGS=
.PHONY: all clean main print gen run
all: main print gen
main: main.cpp
	${MPI_COMPILER} -o main -O3 main.cpp
print: arrayprint.cpp
	$(COMPILER) -o print arrayprint.cpp
gen: create.cpp 
	$(COMPILER) -o gen create.cpp
run:
	mpisubmit.bg -n 64 --stderr err --stdout out main a b c 
clean:
	rm -rf main print gen compare *.dat *.data

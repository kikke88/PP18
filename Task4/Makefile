COMPILER=g++ 
MPI_COMPILER=mpicxx
CFLAGS=
.PHONY: all clean main print gen compare run
all: main print gen compare
main: main.cpp
	${MPI_COMPILER} -o main main.cpp
print: arrayprint.cpp
	$(COMPILER) -o print arrayprint.cpp
gen: create.cpp 
	$(COMPILER) -o gen create.cpp
compare: equal.cpp
	$(COMPILER) -o compare equal.cpp
run:
	mpisubmit.bg -n 64 --stderr err --stdout out main a b c 
clean:
	rm -rf main print gen compare *.data

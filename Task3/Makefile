CXX=g++
CFLAGS=
.PHONY: all clean main run

all: main
main:
	mpicxx -o prime main.cpp
run:
	mpisubmit.bg -n 64 --stderr err --stdout out prime -- 2 100000000 prime.txt
clean:
	rm -rf prime.txt prime *.png
draw:
	./draw.sh
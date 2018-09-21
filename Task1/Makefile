COMPILE = g++-7 -std=c++1z
basic = ./main ./tests/A.dat ./tests/B.dat 
.PHONY: all clean install uninstall
all: print gen compare main
createtest:
	./gen d 503 798 ./tests/A.dat
	./gen d 798 631 ./tests/B.dat
clean:
	rm -rf gen print compare main tmp *.out *.o rex.txt ./tests/*.* *.png
result:
	rm result.txt
print: arrayprint.cpp
	$(COMPILE) -o print arrayprint.cpp
gen: create.cpp
	$(COMPILE) -o gen create.cpp
compare: equal.cpp
	$(COMPILE) -o compare equal.cpp
main: multiplication.cpp
	$(COMPILE) -o main multiplication.cpp
tmp: tmp.cpp
	$(COMPILE) -o tmp tmp.cppl
test1:
	$(basic) ./tests/C0.dat 0 > result.txt
	$(basic) ./tests/C1.dat 1 >> result.txt
	$(basic) ./tests/C2.dat 2 >> result.txt
	$(basic) ./tests/C3.dat 3 >> result.txt
	$(basic) ./tests/C4.dat 4 >> result.txt
	$(basic) ./tests/C5.dat 5 >> result.txt
	./compare ./tests/C0.dat ./tests/C1.dat
	./compare ./tests/C1.dat ./tests/C2.dat
	./compare ./tests/C2.dat ./tests/C3.dat
	./compare ./tests/C3.dat ./tests/C4.dat
	./compare ./tests/C4.dat ./tests/C5.dat
report:
	$(basic) ./tests/C0.dat 0 > result.txt
	$(basic) ./tests/C1.dat 1 >> result.txt
	$(basic) ./tests/C2.dat 2 >> result.txt
	$(basic) ./tests/C3.dat 3 >> result.txt
	$(basic) ./tests/C4.dat 4 >> result.txt
	$(basic) ./tests/C5.dat 5 >> result.txt
	gnuplot -p plotsettings.txt
	subl graph.png
COMPILE = g++-7 -std=c++1z
basic = ./main A.dat B.dat C.dat
.PHONY: all clean install uninstall
all:
clean:
	rm -rf gen print compare main tmp *.out *.o
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
	$(basic) 0 >> rez.txt
	$(basic) 1 >> rez.txt
	$(basic) 2 >> rez.txt
	$(basic) 3 >> rez.txt
	$(basic) 4 >> rez.txt
	$(basic) 5 >> rez.txt

COMPILE = g++
.PHONY: all clean install uninstall
all: print gen compare main
clean:
	rm -rf gen print compare main tmp avg *.out 
print: arrayprint.cpp
	$(COMPILE) -o print arrayprint.cpp
gen: create.cpp 
	$(COMPILE) -o gen create.cpp
compare: equal.cpp
	$(COMPILE) -o compare equal.cpp
main: main.cpp
	mpicxx -o main main.cpp
tmp: tmp.cpp
	$(COMPILE) -o tmp tmp.cppl
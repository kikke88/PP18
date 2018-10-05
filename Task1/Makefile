COMPILE = g++-8 -std=c++1z
basic = ./main ./tests/T1.dat ./tests/T2.dat
stat = ./main ./tests/A_stat_500.dat ./tests/B_stat_500.dat ./tests/tmp.dat
.PHONY: all clean install uninstall
all: print gen compare main
clean:
	rm -rf gen print compare main tmp avg *.out *.o rex.txt ./tests/A*.* ./tests/B*.* ./tests/C*.* *.png result.txt *.png tmp.dat avg.png
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
test: main compare
	$(basic) ./tests/C0.dat 0 > result.txt
	$(basic) ./tests/C1.dat 1 >> result.txt
	$(basic) ./tests/C2.dat 2 >> result.txt
	$(basic) ./tests/C3.dat 3 >> result.txt
	$(basic) ./tests/C4.dat 4 >> result.txt
	$(basic) ./tests/C5.dat 5 >> result.txt
	./compare ./tests/C0.dat ./tests/Result.dat
	./compare ./tests/C1.dat ./tests/Result.dat
	./compare ./tests/C2.dat ./tests/Result.dat
	./compare ./tests/C3.dat ./tests/Result.dat
	./compare ./tests/C4.dat ./tests/Result.dat
	./compare ./tests/C5.dat ./tests/Result.dat
	rm ./tests/C*.*
report: gen main
	./plot 50
	./plot 200
	./plot 300
	./plot 500
	./plot 1000
graph_statistic: gen main
	g++-8 -std=c++1z -o avg averaging.cpp
	./gen d 500 500 ./tests/A_stat_500.dat
	./gen d 500 500 ./tests/B_stat_500.dat
	touch statistic.txt
	for i in 1 2 3 4 5 6 7 ; do \
		for j in 0 1 2 3 4 5 ; do \
			$(stat) $$j >> statistic.txt ; \
		done; \
	done 
	rm ./tests/tmp.dat
	rm ./tests/*_stat_500.dat
	./avg statistic.txt
	gnuplot plotsettings.txt -e "set output 'avg.png';plot \"statistic.txt\" title \"500 * 500 * 500\" with linespoints lw 2 ps 2 pt 3 lt rgb 'blue'"
	rm statistic.txt
	rm ./avg
	xdg-open avg.png
#!/bin/bash
for file in $(ls results) ; do \
	gnuplot plotsetting.txt -e "set title \"$file\"; set ylabel \"$file\";set output '$file.png'; \
								plot \"results/$file\" u 1:2 title \"ijk 32*32\", \
								'' u 3:4 title \"ikj 32*32\", \
								'' u 5:6 title \"ikj 48*48\""; \
done

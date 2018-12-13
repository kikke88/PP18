#!/bin/bash

for j in 512 1024 2048 4096
do
	mpisubmit.bg -n ${1} -w 00:30:00 ./main -- A_{j} B{j} C{j}	
done

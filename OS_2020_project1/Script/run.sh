#!/bin/bash
for scheduling in FIFO SJF PSJF RR; do
	for i in 1 2 3 4 5;
	do
		sudo dmesg --clear;
		./OS_project < ./OS_PJ1_Test/${scheduling}_${i}.txt > output/${scheduling}_${i}_stdout.txt;
		dmesg -t | grep Project1 > output/${scheduling}_${i}_dmesg.txt;
	done;
done;

sudo dmesg --clear;
./OS_project < ./OS_PJ1_Test/TIME_MEASUREMENT.txt > output/TIME_MEASUREMENT_stdout.txt;
dmesg -t | grep Project1 > output/TIME_MEASUREMENT_dmesg.txt;

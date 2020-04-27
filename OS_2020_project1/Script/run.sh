#!/bin/bash
for scheduling in FIFO SJF PSJF RR; do
	for i in {1..5};
	do
		sudo dmesg --clear;
		./OS_project < ./OS_PJ1_Test/${scheduling}_${i}.txt > outputs/${scheduling}_${i}_stdout.txt;
		dmesg | grep Project1 > outputs/${scheduling}_${i}_dmesg.txt;
	done;
done;

sudo dmesg --clear;
./OS_project < ./OS_PJ1_Test/TIME_MEASUREMENT.txt > outputs/TIME_MEASUREMENT_stdout.txt;
dmesg | grep Project1 > outputs/TIME_MEASUREMENT_dmesg.txt;

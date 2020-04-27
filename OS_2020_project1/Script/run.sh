#!/bin/bash
for scheduling in FIFO SJF PSJF RR; do
	for i in {1..5};
	do
		sudo dmesg --clear;
		./main < ../OS_PJ1_Test/${scheduling}_${i}.txt > output1/${scheduling}_${i}_stdout.txt;
		dmesg | grep Project1 > output1/${scheduling}_${i}_dmesg.txt;
	done;
done;

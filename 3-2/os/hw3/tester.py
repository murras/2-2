#!/usr/bin/python3

import os, sys, subprocess, random, itertools


trace_path = "../mtraces/"
trace_list = [trace_path + trace for trace in os.listdir(trace_path)]

trace_args = []
for i in range(1, 4):
	trace_args += list(map(list, itertools.product(trace_list, repeat=i)))

subprocess.call("gcc -o memsimhw memsimhw.c -O2 -W -Wall", shell=True)

i = 1
while True:
	firstLvBits = random.randint(0, 32)
	phyMem = random.randint(0, 32)
	traces = random.choice(trace_args)

	args = [str(firstLvBits), str(phyMem)] + traces
	try:
		answer = subprocess.check_output(["./memsim"] + args)
	except:
		continue
	mine = subprocess.check_output(["./memsimhw"] + args)

	if answer == mine:
		print(i,[firstLvBits, phyMem]+traces, "PASSED")
	else:
		print(" ".join(args))
		exit(1)

	i += 1


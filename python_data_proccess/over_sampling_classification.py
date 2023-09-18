#!/usr/bin/env python

from imblearn.over_sampling import BorderlineSMOTE
import pandas as pd
import csv
import sys

if len(sys.argv) != 3:
	print("ERROR: Pass by argument the data file name and the output dir.")
	exit()

dataset_path = sys.argv[1]
output_dir = sys.argv[2]

with open(dataset_path) as csv_file:
    # leemos del csv
	reader = csv.reader(csv_file,  delimiter = ',')
    # por cada linea
	lines = [i for i in reader]
	X = [data[:-1] for data in lines]
	y = [data[-1] for data in lines]

oversample = BorderlineSMOTE()
X, y = oversample.fit_resample(X, y)

X = [ list(map(lambda x: round(x), data)) for data in X]

f = open(output_dir, "w")

i = 0
for data in X:
	for value in data:
		f.write(str(value))
		f.write(',')
	f.write(y[i])
	f.write("\n")
	i += 1

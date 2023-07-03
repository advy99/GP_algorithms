#!/usr/bin/env python

from imblearn.over_sampling import BorderlineSMOTE
import pandas as pd
import csv
import sys

if len(sys.argv) != 3:
	print("ERROR: Introduzca el nombre del fichero de data y la ruta de salida")
	exit()

conjunto = sys.argv[1]
salida = sys.argv[2]

with open(conjunto) as archivo_csv:
    # leemos del csv
	reader = csv.reader(archivo_csv,  delimiter = ',')
    # por cada linea
	lineas = [i for i in reader]
	X = [dato[:-1] for dato in lineas]
	y = [dato[-1] for dato in lineas]

oversample = BorderlineSMOTE()
X, y = oversample.fit_resample(X, y)

X = [ list(map(lambda x: round(x), dato)) for dato in X]

f = open(salida, "w")

i = 0
for dato in X:
	for value in dato:
		f.write(str(value))
		f.write(',')
	f.write(y[i])
	f.write("\n")
	i += 1

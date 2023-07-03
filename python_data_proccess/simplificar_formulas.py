from sympy import *

x_0, x_1, x_2, x_3, x_4, x_5, x_6, x_7, x_8 = symbols('x_0 x_1 x_2 x_3 x_4 x_5 x_6 x_7 x_8')


import sys

if len(sys.argv)!= 3:
    print("ERROR: introduce el archivo y la salida")
    exit()

archivo = sys.argv[1]
salida = open(sys.argv[2], "w")

with open(archivo) as file:
    data = [line.rstrip('\n') for line in file]


for linea in data:
    resultado = simplify(linea)
    salida.write("$")
    salida.write(str(latex(resultado)))
    salida.write("$")
    salida.write("\n") 

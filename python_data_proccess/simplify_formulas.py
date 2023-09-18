from sympy import *

x_0, x_1, x_2, x_3, x_4, x_5, x_6, x_7, x_8 = symbols('x_0 x_1 x_2 x_3 x_4 x_5 x_6 x_7 x_8')


import sys

if len(sys.argv)!= 3:
    print("ERROR: Pass by argument the input file and the output")
    exit()

input_file = sys.argv[1]
output_stream = open(sys.argv[2], "w")

with open(input_file) as file:
    data = [line.rstrip('\n') for line in file]


for line in data:
    result = simplify(line)
    output_stream.write("$")
    output_stream.write(str(latex(result)))
    output_stream.write("$")
    output_stream.write("\n") 

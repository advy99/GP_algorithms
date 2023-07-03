import pandas as pd
import csv
import smogn
from smogn.over_sampling import over_sampling
import sys
import seaborn
import matplotlib.pyplot as plt


if len(sys.argv) != 3:
	print("ERROR: Introduzca el nombre del fichero de data y la ruta de salida")
	exit()


caracteristicas = []

for i in range(0, 10):
	caracteristicas.append("C{}".format(i))

print(caracteristicas)

data = pd.read_csv(sys.argv[1], header=None, names=caracteristicas)


smogn.box_plot_stats(data['C9'])['stats']
seaborn.kdeplot(data['C9'], label = "Conjunto original")

plt.ylabel("Densidad en el conjunto de data")
plt.xlabel("Edad")

# plt.legend()
plt.savefig("{}.png".format(sys.argv[1]), dpi=300)
plt.clf()


# buenos resultados entre 0.6 y 0.8 de rel_coef

datos_over_sampling = smogn.smoter(data = data, y = 'C9', k = 5, pert = 0.02, samp_method = "extreme",
									under_samp = True, drop_na_col = True, drop_na_row = True, replace = False,
									rel_thres = 0.5,
								    rel_method = "auto",
								    rel_xtrm_type = "low",
								    rel_coef = 0.7)

print(data.shape)
print(datos_over_sampling.shape)

plt.ylabel("Densidad en el conjunto de data")
plt.xlabel("Edad")

smogn.box_plot_stats(data['C9'])['stats']
smogn.box_plot_stats(datos_over_sampling['C9'])['stats']

seaborn.kdeplot(data['C9'], label = "Conjunto original")
seaborn.kdeplot(datos_over_sampling['C9'], label = "Conjunto tras SMOGN")

plt.legend()
plt.savefig("{}.png".format(sys.argv[2]), dpi=300)
datos_over_sampling.to_csv(sys.argv[2], index = False, header = False)

import pandas as pd
import csv
import smogn
from smogn.over_sampling import over_sampling
import sys
import seaborn
import matplotlib.pyplot as plt


if len(sys.argv) != 3:
	print("ERROR: Pass by argument the data file name and the output dir.")
	exit()


features = []

for i in range(0, 10):
	features.append("C{}".format(i))

print(features)

data = pd.read_csv(sys.argv[1], header=None, names=features)


smogn.box_plot_stats(data['C9'])['stats']
seaborn.kdeplot(data['C9'], label = "Original dataset")

plt.ylabel("Density")
plt.xlabel("Age")

# plt.legend()
plt.savefig("{}.png".format(sys.argv[1]), dpi=300)
plt.clf()


# good results with rel_coef between 0.6 and 0.8

datos_over_sampling = smogn.smoter(data = data, y = 'C9', k = 5, pert = 0.02, samp_method = "extreme",
									under_samp = True, drop_na_col = True, drop_na_row = True, replace = False,
									rel_thres = 0.5,
								    rel_method = "auto",
								    rel_xtrm_type = "low",
								    rel_coef = 0.7)

print(data.shape)
print(datos_over_sampling.shape)

plt.ylabel("Density")
plt.xlabel("Age")

smogn.box_plot_stats(data['C9'])['stats']
smogn.box_plot_stats(datos_over_sampling['C9'])['stats']

seaborn.kdeplot(data['C9'], label = "Original dataset")
seaborn.kdeplot(datos_over_sampling['C9'], label = "Dataset after SMOGN")

plt.legend()
plt.savefig("{}.png".format(sys.argv[2]), dpi=300)
datos_over_sampling.to_csv(sys.argv[2], index = False, header = False)

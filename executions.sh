#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Three parameters needed. Train, test and validation data files."
	exit 1
fi

POPULATION_SIZE=1000
PROB_VAR=0.3
NUM_EVALS=1000000
GP_CROSSOVER_PROB=0.75
GA_CROSSOVER_PROB=0.75
GP_MUTATION_PROB=0.05
GA_MUTATION_PROB=0.05
INTERNICHE_CROSSOVER_PROB=0.3
TAM_TOURNAMENT=100
NUM_THREADS=8

mkdir -p executions_output/

seeds=(12345 92034 8324 34679 34634)


for depth in $(seq 20 40 60)
do
	echo "Starting execution with depth $depth"

	printf "# seed \t MSE 5cv \t RMSE 5cv \t MAE 5cv \t Best expression  \t Execution time\n" > executions_output/$(basename ${1})_prof_${depth}.dat
	for seed in ${seeds[*]}
	do
		./bin/main $1 $2 $3 $POPULATION_SIZE $PROB_VAR $depth $NUM_EVALS $GP_CROSSOVER_PROB $GA_CROSSOVER_PROB $GP_MUTATION_PROB $GA_MUTATION_PROB $INTERNICHE_CROSSOVER_PROB $TAM_TOURNAMENT $NUM_THREADS $seed >> executions_output/$(basename ${1})_prof_${depth}.dat &
	done
	wait
	echo "Executed with depth $depth"
done


echo "Finished"

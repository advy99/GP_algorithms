#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Se necesitan un parámetros, el fichero de datos de train"
	exit -1
fi

TAM_POBLACION=1000
PROB_VAR=0.3
NUM_EVALS=1000000
PROB_CRUCE_GP=0.75
PROB_CRUCE_GA=0.75
PROB_MUTACION_GP=0.05
PROB_MUTACION_GA=0.05
PROB_CRUCE_INTRANICHO=0.3
TAM_TORNEO=100
NUM_HILOS=8

mkdir -p salidas_ejecuciones/

semillas=(12345 92034 8324 34679 34634)


for profundidad in $(seq 20 40 60)
do
	echo "Ejecutando con profundidad $profundidad"

	printf "# semilla \t ECM 5cv \t RECM 5cv \t MAE 5cv \t Mejor expresión  \t Tiempo de ejecucion\n" > salidas_ejecuciones/$(basename ${1})_prof_${profundidad}.dat
	for semilla in ${semillas[*]}
	do
		./bin/main $1 $2 $3 $TAM_POBLACION $PROB_VAR $profundidad $NUM_EVALS $PROB_CRUCE_GP $PROB_CRUCE_GA $PROB_MUTACION_GP $PROB_MUTACION_GA $PROB_CRUCE_INTRANICHO $TAM_TORNEO $NUM_HILOS $semilla >> salidas_ejecuciones/$(basename ${1})_prof_${profundidad}.dat &
	done
	wait
	echo "Ejecutado con profundidad $profundidad"
done


echo "Ejecutado"

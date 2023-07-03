/**
  * \@file preprocess.hpp
  * @brief Fichero cabecera de las funciones de preprocess
  *
  */

#ifndef PREPROCESS_H_INCLUDED
#define PREPROCESS_H_INCLUDED

#include "expressions_algs/aux_expressions_alg.hpp"


template <class T>
using matriz = std::vector<std::vector<T>>;


namespace expressions_algs :: preprocess {

/**
 * @brief Separar unos data y labels dadas en dos conjuntos de entrenamiento y test,
 *  de forma aleatorio o no aleatoria, si COMIENZO != 1
 *
 * @tparam T type de los data a separar
 * @param data Características a separar
 * @param labels Etiquetas de los data a separar
 * @param PORCENTAJE_TEST Porcentaje de data que formaran parte del conjunto de test, por defecto 0.2
 * @param COMIENZO Indice a partir de donde los data pasaran a ser de test. Si vale -1, se escogeran los
 *  data de test de forma aleatoria. Por defecto -1
 *
 * @return Pareja de entrenamiento y test. Cada uno de estos es una pareja de data y labels
 *
 */

template <class T>
std::pair<std::pair<matriz<T>, std::vector<T> >, std::pair<matriz<T>, std::vector<T> > >
	split_train_test(matriz<T> data, std::vector<T> labels,
							 const double PORCENTAJE_TEST = 0.2, const int COMIENZO = -1);


/**
  * @brief Leer data de entrada para el algoritmo
  *
  * @tparam T type de los data a leer
  * @param data_file Fichero donde se almacenan los data
  * @param comment_char Caracter que marca que una linea es un
  * comentario y ha de ser ignorada
  * @param delimiter Caracter que marca como están separados los
  * data de entrada
  *
  * @pre Los data a leer vienen dados separados por delimiter. Cada dato es una linea.
  *
  * @return Pareja de data y labels leidas
  *
  */

template <class T>
std::pair<matriz<T>, std::vector<T> >	read_data(const std::string & data_file,
				  								  		  const char comment_char, const char delimiter = ',');


/**
  * @brief Escribir data en un fichero
  *
  * @tparam T type de los data a escribir
  * @param salida Nombre del fichero donde escribir los data
  * @param data Datos a escribir
  * @param labels Eiquetas asociadas a los data dados
  * @param delimiter Caracter que marca como están separados los data
  *
  */

template <class T>
void write_data(const std::string & salida, const matriz<T> & data, const std::vector<T> & labels, const char delimiter = ',');


/**
  * @brief Reordenar de forma aleatoria los data
  *
  * @tparam T type de los data a escribir
  * @param data Datos a reordenar
  * @param labels Eiquetas asociadas a los data dados
  *
  * @return Pareja de data y labels, reordenados de forma aleatoria
  */

template <class T>
std::pair<matriz<T>, std::vector<T> > random_data_reorder (matriz<T> data, std::vector<T> labels);


}

#include "preprocess.tpp"

#endif

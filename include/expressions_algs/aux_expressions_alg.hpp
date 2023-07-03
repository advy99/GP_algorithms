/**
  * \@file aux_expressions_alg.hpp
  * @brief File with some auxiliar functions for expressions_algs
  *
  */

#ifndef AUX_PG_ALGS
#define AUX_PG_ALGS

#ifdef _OPENMP
	#include <omp.h>
#endif

#include <algorithm>
#include <random>
#include "Random.hpp"
#include <cstring>
#include <stack>
#include <limits>
#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <set>
#include <type_traits>


namespace expressions_algs :: aux {

/**
 * @brief Definition of an evaluation function type
 *
 */

typedef double (*eval_function_t)(const std::vector<double> & data, const std::vector<double> & labels);


/**
  * @brief Compare if two floating point values are equals considering the IEEE float representation using an epsilon.
  * 
  *
  * @param a Number a to be compared.
  * @param b Number b to be compared.
  * @param epsilon Epsilon to consider if two values are equals. Defaults to 0.0005
  *
  * @return Boolean: True if a and b only differ in epsilon.
  *
  */

bool compare_floats(const double a, const double b, const double epsilon = 0.005);

/**
  * @brief Compute the cuadratic mean error between two set of values
  *
  * @param predicted_values Predicted values.
  * @param real_values Real values.
  *
  * @return Cuadratic mean error between predicted_values and real_values
  *
  */

double cuadratic_mean_error(const std::vector<double> & predicted_values,
										const std::vector<double> & real_values);

/**
  * @brief Compute the root of the cuadratic mean error between two set of values
  *
  * @param predicted_values Predicted values.
  * @param real_values Real values.
  *
  * @return Root of the cuadratic mean error between predicted_values and real_values
  *
  */

double root_cuadratic_mean_error(const std::vector<double> & predicted_values,
											  const std::vector<double> & real_values);

/**
 * @brief Compute the mean absolute error between two set of values
 *
 * @param predicted_values Predicted values.
 * @param real_values Real values.
 *
 * @return Mean absolute error between predicted_values and real_values
 *
 */

double mean_absolute_error(const std::vector<double> & predicted_values,
									const std::vector<double> & real_values);


}

#include "preprocess.hpp"


#endif

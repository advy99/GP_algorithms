/**
  * \@file Parameters.hpp
  * @brief Header file of the Parameters class
  *
  */

#ifndef PARAMETROS_H_INCLUDED
#define PARAMETROS_H_INCLUDED


#include "expressions_algs/aux_expressions_alg.hpp"

namespace expressions_algs {



/**
  *  @brief  Parameters Class
  *
  * An instance of the type Parameters will represent the parameters we can use to fit a Population_alg
  *
  *
  * @author Antonio David Villegas Yeguas
  * @date Abril 2021
  */

class Parameters {
	private:

		/**
		  * @page repParameters Parameters class representation
		  *
		  * @section faParameters Abstraction function
		  *
		  * A valid object @e rep of class Parameters is set by:
		  *
		  *
		  * rep.num_evaluations_
		  * rep.gp_crossover_probability_
		  * rep.ga_crossover_probability_
		  * rep.gp_mutation_probability_
		  * rep.ga_mutation_probability_
		  * rep.tournament_size_
		  * rep.show_evolution_
		  *
		  */


		/**
		 *
		 *  @brief Number of evaluations to fit
		 *
		 */

		int num_evaluations_;



		/**
		 *
		 *  @brief Probability of applying a crossover in the genetic programming part.
		 *
		 */

		double gp_crossover_probability_;

		/**
		 *
		 *  @brief Probability of applying a crossover in the part of genetic algorithm
		 *
		 * @pre  GA_P_alg is used
		 */

		double ga_crossover_probability_;

		/**
		 *
		 *  @brief Probability of applying a mutation in the genetic programming part.
		 *
		 */

		double gp_mutation_probability_;

		/**
		 *
		 *  @brief Probability of applying a mutation in the genetic algorithm part.
		 *
		 * @pre GA_P_alg is used
		 */

		double ga_mutation_probability_;


		/**
		 *
		 * @brief Probability of applying an intra-niche crossover in GAP
		 *
		 */

		double probabilidad_cruce_intranicho_;

		/**
		 *
		 *  @brief Size of the tournament to fit the population
		 *
		 */

		int tournament_size_;

		/**
		 *
		 *  @brief Boolean to control whether the evolution of the algorithm is displayed.
		 *
		 */
		bool show_evolution_;


		/**
		 *
		 * @brief Pointer to the evaluation function to be used for training
		 *
		 */

		 aux::eval_function_t eval_f_;

		 /**
 		 *
 		 * @brief Vector of pointers to the evaluation functions to be used to obtain the error, 
		 * in addition to the evaluation function. Only to be used in cross validation
		 *
 		 */

 		 std::vector<aux::eval_function_t> eval_error_functions_;

	public:

		/**
		 *
		 *  @brief Constructor with seven parameters
		 *
		 * @param N_EVALS Number of evaluations, default 100000
		 * @param PROB_CROSSOVER_GP Probability of crossover on the GP side, by default 0.8
		 * @param PROB_CROSSOVER_GA Probability of crossover on the GA side, by default 0.8
		 * @param PROB_MUTA_GP Probability of mutation in the GP part, by  0.01
		 * @param PROB_MUTA_GA Probability of mutation in GA part, by default 0.01
		 * @param INTERNICHE_CROSSOVER_PROB Probability of intranicho crossover in GA-P, by default 0.4
		 * @param TOURNAMENT_SIZE Tournament size, default 15
		 * @param SHOW_EVOLUTION Show evolution of the algorithm, by default enabled.
		 *
		 */

		Parameters(const int N_EVALS = 100000,
					  aux::eval_function_t = expressions_algs::aux::cuadratic_mean_error,
			 		  const double PROB_CROSSOVER_GP = 0.8,
					  const double PROB_CROSSOVER_GA = 0.8,
					  const double PROB_MUTA_GP = 0.01,
					  const double PROB_MUTA_GA = 0.01,
					  const double INTERNICHE_CROSSOVER_PROB = 0.4,
					  const int TOURNAMENT_SIZE = 15,
					  const bool SHOW_EVOLUTION = true);


		/**
		 *
		 *  @brief Constructor con cinco parámetros
		 *
		 * @param N_EVALS Number of evaluations, default 100000
		 * @param PROB_CROSSOVER_GP Probability of crossover on the PG side, by default 0.8
		 * @param PROB_MUTA_GP Probability of mutation in the GP part, by  0.01
		 * @param TOURNAMENT_SIZE Tournament size, default 15
		 * @param SHOW_EVOLUTION Show evolution of the algorithm, by default enabled.
		 *
		 */

		Parameters(const int N_EVALS = 100000,
					  aux::eval_function_t = expressions_algs::aux::cuadratic_mean_error,
			 		  const double PROB_CROSSOVER_GP = 0.8,
					  const double PROB_MUTA_GP = 0.1,
					  const int TOURNAMENT_SIZE = 15,
					  const bool SHOW_EVOLUTION = false);


		/**
		 *  @brief Obtain the number of evaluations with which fit
		 *  @return Number of evaluations with which you fit
		 */

		int get_num_evaluations() const;

		/**
		 *  @brief Obtain the probability of crossing the GP
		 *  @return Probability of crossing the GP
		 */
		double get_pg_crossover_probability() const;

		/**
		 *  @brief Obtain the probability of crossing the GA
		 *  @return Probability of crossing the GA
		 */
		double get_ga_crossover_probability() const;


		/**
		 *  @brief Obtener la probabilidad de mutar la parte GP
		 *  @return Probabilidad de mutar la parte GP
		 */
		double get_pg_mutation_probability() const;

		/**
		 *  @brief Obtener la probabilidad de mutar la parte GA
		 *  @return Probabilidad de mutar la parte GA
		 */
		double get_ga_mutation_probability() const;

		/**
		 *  @brief Obtener la probabilidad de realizar un cruce intranicho en GA-P
		 *  @return Probabilidad de realizar un cruce intranicho en GA-P
		 */
		double get_ga_inter_niche_crossover_probability() const;

		/**
		 *  @brief Obtener el tamaño del torneo
		 *  @return Tamaño del torneo
		 */
		int get_tournament_size() const;

		/**
		 *  @brief Obtener si se muestra la evolucion
		 *  @return Booleano: verdadero si se muestra la evolucion, falso si no
		 */
		bool get_show_evaluation() const;

		/**
		 *  @brief Obtener la funcion de evaluacion
		 *  @return Puntero a la función de evaluacion a utilizar
		 */
		aux::eval_function_t get_evaluation_functions() const;

		/**
		 *  @brief Añadir una función de error para estimar un modelo con dicha función
		 *
		 *  @param func Función a añadir para devolver su error en cross-validation
		 *
		 */

		void add_error_function(const aux::eval_function_t & func);

		/**
		 *  @brief Obtener una función de error a utilizar
		 *
		 *  @param index Indice de la función de error a obtener
		 *
		 * @return Funcion de error en la posición index
		 */

		aux::eval_function_t get_error_function(const unsigned index) const;

		/**
		 *  @brief Obtener el numero de funciones de error a utilizar
		 *
		 *
		 * @return Numero de funciones de error a utilizar
		 */

		unsigned get_num_evaluation_functions() const;

};

}

#endif

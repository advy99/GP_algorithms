/**
  * @file GA_P_alg.hpp
  * @brief Header file class for GA_P_alg
  *
  */

#ifndef GA_P_H_INCLUDED
#define GA_P_H_INCLUDED

#include "expressions_algs/Population.hpp"
#include "expressions_algs/Population_alg.hpp"

#include "Random.hpp"



namespace expressions_algs {

/**
  *  @brief Clase GA_P_alg
  *
  *  Una instancia del type GA_P_alg representará un estimador para los data
  *  dados, utilizando una Population, conjunto de GA_P_Expression.
  *
  *  An instance of type GA_P_alg will represent an estimator for the given data, 
  * using a Population ( a set of GA_P_Expression)
  *
  * @author Antonio David Villegas Yeguas
  * @date Abril 2021
  */

class GA_P_alg : public Population_alg<GA_P_Expression> {
	private:
		using Population_alg<GA_P_Expression>::population_;
		using Population_alg<GA_P_Expression>::data_;
		using Population_alg<GA_P_Expression>::output_data_;
		using Population_alg<GA_P_Expression>::expressions_depth_;

		using Population_alg<GA_P_Expression>::read_data;
		using Population_alg<GA_P_Expression>::initialize_empty;
		using Population_alg<GA_P_Expression>::get_num_variables;
		using Population_alg<GA_P_Expression>::get_expressions_max_depth;
		using Population_alg<GA_P_Expression>::tournament_selection;
		using Population_alg<GA_P_Expression>::generate_population;
		using Population_alg<GA_P_Expression>::apply_elitism;
		using Population_alg<GA_P_Expression>::apply_GP_mutations;
		using Population_alg<GA_P_Expression>::initialize;

		/**
		  * @page repGA_P Representation of GA_P_alg class
		  *
		  * @section invGA_P Invariant of the class
		  *
		  *  data_.size > 0 and output_data_.size == data.size
		  *
		  * @section faGA_P Abstraction function
		  *
		  * A valid object @e rep of the GA_P_alg class 
		  * represents a set of data with its respective tags
		  *
		  * rep.data_
		  * rep.output_data_
		  *
		  * As well as a population of expressions to estimate the data.
		  *
		  * rep.population_
		  *
		  */

		  /**
		   * @brief Intra-niche individual selection for GA-P
			*
			* @param mom Index of the individual that has to be in the same niche
			* @param chosen Array of booleans that tells us which elements have already been chosen for crossover.
			*
			* @return Index of an individual in the same niche
			*/

		  int inter_niche_selection(const int mom, const std::vector<bool> & chosen) const;


	public:

		/**
		 *  @brief Contructor with 6 parameters, to use the GA_P_alg given a set of data with their associated tags
		 *
		 *  @param data Data to fit
		 *  @param labels Tags associated to data
		 *  @param seed Random seed to use
		 *  @param population_size Number of individuals in the population
		 *  @param max_depth Maximum depth of expressions used
		 *  @param prob_var Probability of a Node being a variable
		 *
		 */

		GA_P_alg(const std::vector<std::vector<double> > & data, const std::vector<double> & labels,
			 			  const unsigned long seed, const unsigned population_size, const unsigned max_depth, const double prob_var);


		/**
		  * @brief Constructor with seven parameters
		  *
		  * @param data_file File where data is stored
		  *
		  * @param comment_char Character that marks that a line is a comment and must be ignored.
		  *
		  * @param population_size Size of the population that will make up the GA_P_alg
		  *
		  * @param prob_var Probability that in a population expression a leaf node is a variable.
		  *
		  * @param seed Seed to be used in GA_P_alg. No effect if the seed has already been initialized before.
		  *
		  * @param delimiter Character that marks how the input data is separated. Defaults to ','
		  *
		  * @param max_depth Maximum depth of the expressions with which 
		  * the algorithm will learn. Default 10 
		  *
		  *
		  */

		GA_P_alg(const std::string data_file, const char comment_char,
			  const unsigned population_size, const double prob_var,
			  const unsigned long seed = time(nullptr),
			  const char delimiter = ',', const unsigned max_depth = 20);

		/**
		  * @brief Destructor
		  *
		  */

		~GA_P_alg() = default;


		/**
		 *  @brief Fit the population using the current data with the given parameters
		 *
		 *  @param parameters Parameters to be used in algorithm adjustment
		 *
		 **/

		void fit(const Parameters & parameters) ;


};

} // namespace expressions_algs


#endif

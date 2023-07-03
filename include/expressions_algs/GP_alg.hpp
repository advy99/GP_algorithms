/**
  * \@file GP_alg.hpp
  * @brief Header file of GP_alg class
  *
  */


#ifndef PG_H_INCLUDED
#define PG_H_INCLUDED


#include "expressions_algs/Population_alg.hpp"


namespace expressions_algs {

/**
  *  @brief  GP_alg Class
  *
  *  An instance of type GP_alg will represent an estimator for the given data, using a Population, set of GP_alg.
  *
  *
  * @author Antonio David Villegas Yeguas
  * @date Abril 2021
  */

class GP_alg : public Population_alg<Expression> {
	private:

		using Population_alg<Expression>::population_;
		using Population_alg<Expression>::data_;
		using Population_alg<Expression>::output_data_;
		using Population_alg<Expression>::expressions_depth_;

		using Population_alg<Expression>::read_data;
		using Population_alg<Expression>::initialize_empty;
		using Population_alg<Expression>::get_num_variables;
		using Population_alg<Expression>::get_expressions_max_depth;
		using Population_alg<Expression>::tournament_selection;
		using Population_alg<Expression>::generate_population;
		using Population_alg<Expression>::apply_elitism;
		using Population_alg<Expression>::apply_GP_mutations;
		using Population_alg<Expression>::initialize;




		/**
		  * @page repPG GP_alg class representation
		  *
		  * @section invPG Representation invariant
		  *
		  * The invariant is; data_.size > 0 and output_data.size == data_.size
		  *
		  * @section faPG Abstraction function
		  *
		  * A valid object @e rep of class GP_alg represents a set of data with their respective labels
		  *
		  * rep.data_
		  * rep.output_data_
		  *
		  * As well as a population of expressions to estimate the data.
		  *
		  * rep.population_
		  *
		  */


	public:

		/**
		 *  @brief Contructor with 6 parameters, to use the GP_alg given a set of data with their associated tags
		 *
		 *  @param data Data to be used for fit
		 *  @param labels Tags associated with such data
		 *  @param seed Random seed to be used
		 *  @param population_size Algorithm population size
		 *  @param depth Maximum depth of expressions used
		 *  @param prob_var Probability of a Node being a variable
		 *
		 */

		GP_alg(const std::vector<std::vector<double> > & data, const std::vector<double> & labels,
						const unsigned long seed, const unsigned population_size, const unsigned depth, const double prob_var);


		/**
		  * @brief Constructor with seven parameters
		  *
		  * @param data_file File where data are stored
		  *
		  * @param comment_char Character that marks that a line is a comment and must be ignored.
		  *
		  * @param population_size Size of the population that will make up the GA_P_alg
		  *
		  * @param prob_var Probability that in a population expression a leaf node is a variable.
		  *
		  * @param seed Seed to be used in GA_P_alg. No effect if the seed has already been initialized before.
		  *
		  * @param delimiter haracter that marks how the input data is separated. Defaults to ','.
		  *
		  * @param depth Maximum depth of the expressions with which the algorithm will learn. Default 10
		  *
		  *
		  */

		GP_alg(const std::string data_file, const char comment_char,
			  const unsigned population_size, const double prob_var,
			  const unsigned long seed = time(nullptr),
			  const char delimiter = ',', const unsigned depth = 20);


		/**
		  * @brief Destructor
		  *
		  */

		~GP_alg();


		/**
		 *  @brief Adjust the population using the current data with the given parameters
		 *
		 *  @param parameters Parameters to be used in algorithm adjustment
		 *
		 **/

		void fit(const Parameters & parameters);

};

}



#endif

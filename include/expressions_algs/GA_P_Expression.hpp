/**
  * \@file GA_P_Expression.hpp
  * @brief Header file of class GA_P_Expression
  *
  */

#ifndef GAP_EXPRESSION_H_INCLUDED
#define GAP_EXPRESSION_H_INCLUDED


#include "expressions_algs/Expression.hpp"
#include "expressions_algs/aux_expressions_alg.hpp"

/**
 * @brief Classes, definitions and structures necessary for GA_P_Expression
 *
 */

namespace expressions_algs {

/**
  *  @brief GA_P_Expression Class
  *
  *  An instance of type GA_P_Expression will represent a
  *  mathematical expression, as well as its fitness for a given data.
  *
  *
  * @author Antonio David Villegas Yeguas
  * @date Abril 2021
  */

class GA_P_Expression : public Expression {
	private:
		/**
		  * @page repGA_P_Expression Representation of the class GA_P_Expression
		  *
		  * @section invGA_P_Expression Representation invariant
		  *
		  * max_depth_ > 0
		  *
		  * @section faGA_P_Expression Abstraction function
		  *
		  * A valid object @e rep of class GA_P_Expression represents
		  * an expression given in
		  *
		  * rep.tree_
		  *
		  * His chromosome
		  *
		  * rep.chromosome_
		  *
		  * As well as its adjustment value in
		  *
		  * rep.fitness_
		  *
		  * We can know if it is evaluated with
		  *
		  * rep.is_evaluated_
		  *
		  */


		/**
		  * @brief Array where we will store the values of the constants numerical constants to learn them
		  *
		  */

		std::vector<double> chromosome_;

		/**
		  * @brief Initializing an empty expression.
		  *
		  * @pre The expression has no reserved dynamic memory.
		  *
		  * @post The expression is set as an empty expression.
		  */

		void initialize_empty() override;

		/**
		  * @brief Copy data from a given expression to this expression.
		  *
		  * @param another Expression of where to copy the data.
		  *
		  * @pre This expression is empty: tree == nullptr && chromosome == nullptr
		  *
		  */

		void copy_data(const GA_P_Expression & another);



		/**
		 * @brief Initialize the chromosome of constants associated with Expression
		 *
		 *
		 */

		void initialize_chromosome(const unsigned length);

		/**
		 * @brief Copy given chromosome to current Expression chromosome
		 *
		 * @param another_chromosome chromosome to copy
		 *
		 */

		void copy_chromosome(const std::vector<double> & another_chromosome);


	 	/**
		  * @brief Delta function for non-uniform GA mutation
		  *
		  * @param generation Generation in which the mutation is applied
		  * @param max_gen Maximum number of generations when training.
		  * @param value Chromosome value when applying the mutation
		  *
		  * @return Value to modify the chromosome with the given value
		  */


		double delta(const int generation, const int max_gen, const double value);

		/**
		  * @brief Get the numerical value of the given Node
		  *
		  * @param n Node from which to obtain the value
		  *
		  * @return Numerical value of the given Node
		  */

		double get_number(const Node & n) const override;


	public:
		/**
		  * @brief Constructor with one parameter, generates an empty expression.
		  *
		  * @param max_depth Maximum depth of expression
		  *
		  */

		GA_P_Expression(const unsigned max_depth = 10);

		/**
		  * @brief Constructor with one parameter. Create an expression from the given subtree.
		  *
		  * @param subtree Subtree to create the expression
		  * @param max_depth Maximum depth of the Expression to create
		  *
		  * @pre subtree != nullptr
		  */

		GA_P_Expression(const std::vector<Node> & subtree, const unsigned max_depth = 10);



		/**
		  * @brief Constructor with two parameters, generate a random expression.
		  *
		  * @param max_length Length of the expression to be generated..
		  * @param prob_variable Probability of a node to be a variable.
		  * @param num_vars Number of variables that can use the Expression
		  * @param max_depth Maximum depth of the Expression to create
		  *
		  */

		GA_P_Expression(const unsigned max_length, const double prob_variable,
					 	  const unsigned num_vars, const unsigned max_depth = 10);

		/**
		  * @brief Copy constructor.
		  *
		  * @param another Expression to copy.
		  *
		  */

		GA_P_Expression(const GA_P_Expression & another);

		/**
		  * @brief Destructor.
		  *
		  */

		~GA_P_Expression() = default;

		/**
		  * @brief Generate the expression randomly.
		  *
		  * @param max_length Maximum length of the random expression
		  * @param prob_variable Probability that a terminal symbol is a variable and not a number
		  * @param num_variables Number of variables available for use in the expression
		  *
		  * @return Boolean: True if you have succeeded in generating the expression correctly, false otherwise
		  */

		bool generate_random_expression(const unsigned max_length,
			 									 const double prob_variable,
												 const unsigned num_variables) override;

		/**
 		  * @brief Consult the length of the chromosome.
 		  *
 		  * @return length of the chromosome.
 		  */

 		unsigned get_chromosome_length() const;


		/**
		 * @brief Given a chromosome, assign that chromosome to the current expression.
		 *
		 * @param new_chromosome Chromosome that will form the expression
		 *
		 */

		void assign_chromosome(const std::vector<double> & new_chromosome);

		/**
		  * @brief Assignment operator of an expression. We assign an expression to another.
		  *
		  * @param another Expression to copy.
		  *
		  * @return Reference to this Expressions, allows a = b = c
		  */

		GA_P_Expression & operator= (const GA_P_Expression & another);


		/**
		 * @brief Crossover operator for Expression chromosomes. By crossing two chromosomes we will obtain two new chromosomes.
		 *
		 * @param another Expression with which to cross the chromosome.
		 * @param son1 Expression where the result of the crossing will be stored.
		 * @param son2 Expression where the result of the crossing will be stored.
		 * @param alfa BLX-alpha method alpha value, possible range extension between two values
		 *
		 */


		void blx_alpha_crossover(const GA_P_Expression & another, GA_P_Expression & son1, GA_P_Expression & son2, const double alfa = 0.3) const;


		/**
		 * @brief Obtain where the chromosome is stored
		 *
		 * @return Pointer to the chromosome
		 */

		std::vector<double> get_chromosome() const;

		/**
		 *
		 *  @brief Non-uniform mutation of the genetic algorithm part (chromosome)
		 *
		 *  @param generation Generation in which the mutation is applied
		 *  @param max_gen Maximum number of generations when training
		 *
		 */

		void mutate_ga(const int generation, const int max_gen);


		/**
		 * @brief Check that the given Expression and another belong to the same niche
		 *
		 * @param another Expression with which to check if you are in the same niche
		 *
		 * @return True if they are in the same niche, false if they are not.
		 *
		 */

		bool same_niche(const GA_P_Expression & another) const;

		/**
		 * @brief Operator to compare if two expressions have the same chromosome.
		 *
		 * @param another Expression to be compared with the current one
		 *
		 *
		 * @return True if the expressions have the same chromosome
		 */

		bool same_chromosome(const GA_P_Expression & another) const;

		/**
		 * @brief Operator to compare if two expressions are the same
		 *
		 * @param another Expression to be compared with the current one
		 *
		 * @return True if the expressions are equal
		 */

		bool operator== (const GA_P_Expression & another) const;

};



}



#endif

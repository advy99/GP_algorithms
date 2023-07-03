/**
  * \@file Expression.hpp
  * @brief File with the header of Expression class.
  *
  */

#ifndef EXPRESION_H_INCLUDED
#define EXPRESION_H_INCLUDED

#include "expressions_algs/Node.hpp"
#include "expressions_algs/aux_expressions_alg.hpp"


namespace expressions_algs {

/**
  *  @brief Class Expression
  *
  *  An instance of type Expression represents a mathematical expression, 
  * with a fitness associated to a certain data.
  *
  *
  * @author Antonio David Villegas Yeguas
  * @date Julio 2020
  */

class Expression{
	protected:

		/**
		  * @page repExpression Representation of Expression class.
		  *
		  * @section invExpression Invariant of Expression.
		  *
		  *  max_depth > 0
		  *
		  * @section faExpression Abstraction function
		  *
		  * A valid object @e rep of Expression represents an expression in
		  *
		  * rep.tree_
		  *
		  * Is fitness value in
		  *
		  * rep.fitness_
		  *
		  * And if is evaluated at a certain moment with
		  *
		  * rep.is_evaluated_
		  *
		  */

		/**
		  * @brief Fitness value of the Expression with a set of data
		  */

		double fitness_;

		/**
		  * @brief Attribute to check if the expresion has been evaluated on data.
		  */

		bool is_evaluated_;


		/**
		  * @brief Max depth for the tree representing the expression.
		  */
		unsigned max_depth_;

		/**
		  * @brief Set of nodes that represents the expression..
		  */
		std::vector<Node> tree_;


		/**
		  * @brief Number of variables that can take the expression.
		  */
		unsigned num_variables_;

		/**
		  * @brief Initialize an expression as empty.
		  *
		  * @pre The expression has no dynamic memory reserved.
		  *
		  * @post The expression is set with a valid state, but empty.
		  */

		virtual void initialize_empty();

		/**
		  * @brief Copy data from another expression.
		  *
		  * @param another Expression from where to copy data.
		  *
		  * @pre This expression is empty: tree == nullptr && chromosome == nullptr
		  *
		  */

		void copy_data(const Expression & another);

		/**
		  * @brief Count the levels of the tree that represents an Expression.
		  *
		  * @param stack Stack representing the expression
		  * @param actual_level Actual level of the expression
		  *
		  * @return Number of levels of the expression
		  */

		unsigned count_levels(std::stack<Node> & stack, unsigned actual_level) const;

		/**
		  * @brief Evaluate the expression with a set of data. 
		  *
		  * @param stack Stack with the expression to evaluate
		  * @param dato Data to evaluate
		  *
		  * @return Regression value obtained from evaluating data in stack.
		  */

		double evaluate_data(std::stack<Node> & stack,
								 const std::vector<double> & dato) const;

		/**
		  * @brief Obtain the numeric value of a node
		  *
		  * @param n Node to obtain its value
		  *
		  * @return Numeric value of n
		  */

		virtual double get_number(const Node & n) const;

		/**
		  * @brief Obtain an expression stored in a string
		  *
		  * @param expression_str String of an expression
		  *
		  * @return Array of the expression represented in preorder
		  */

		static std::vector<Node> get_expression(const std::string & expression_str);

		/**
		  * @brief Obatain the subexpression between parenthesis in a general expression
		  *
		  * @param buffer_exp Stream where the expression is being readed
		  *
		  * @return subexpression between parenthesis
		  */

		static std::string get_parenthesis_string(std::istringstream & buffer_exp);

		public:

		/**
		  * @brief Constructor with one parameter, generate empty expression.
		  *
		  * @param max_depth Max depth allowed for this expression.
		  *
		  */

		Expression(const unsigned max_depth = 10);

		/**
		  * @brief Constructor with two parameters. Creates an expression based on 
		  *  a subtree and set a max depth for this expression.
		  *
		  * @param subtree Subtree to create the expression
		  * @param max_depth Max depth allowed in the new expression
		  *
		  * @pre subtree != nullptr
		  */

		Expression(const std::vector<Node> & subtree, const unsigned max_depth = 10);



		/**
		  * @brief Constructor with four parameters, creates a random expression.
		  *
		  * @param max_length Max length allowed to generate an expression.
		  * @param prob_variable Probability of a Node to be a variable.
		  * @param num_vars Number of variables that an expression can use.
		  * @param max_depth Max depth allowed in the new expression
		  *
		  */

		Expression(const unsigned max_length, const double prob_variable,
					 const unsigned num_vars, const unsigned max_depth = 10);

		/**
		  * @brief Copy constructor.
		  *
		  * @param another Expression to copy.
		  *
		  */

		Expression(const Expression & another);


		/**
		  * @brief Constructor with three parameters, read an expression from a file.
		  *
		  * @param filename File path where the expression will be readed.
		  * @param max_length Max length to read.
		  * @param num_variables Number of variables used in this expression.
		  *
		  * @pre The Expression in filename is valid.
		  *
		  */

		Expression ( const std::string & filename, const unsigned max_length, const unsigned num_variables);


		/**
		  * @brief Destructor of an Expression.
		  *
		  */

		~Expression() = default;


		/**
		  * @brief Generate a random expression.
		  *
		  * @param max_length Max allowed length for the random expression
		  * @param prob_variable Probability of a terminal symbol to be a variable
		  * @param num_variables Number of variables to use in the expression.
		  *
		  * @return Boolean: True if the expression has been generated correctly
		  */

		virtual bool generate_random_expression(const unsigned max_length,
					 									 	const double prob_variable,
														 	const unsigned num_variables);

		/**
		  * @brief Check if the expression has been evaluated on a set of data since the last change.
		  *
		  * @return Boolean: True if it is evaluated, false otherwise.
		  */

		bool is_evaluated() const;

		/**
		  * @brief Get the fitness value of the expression in certain data.
		  *
		  * @return Fitness value obtained by this expression.
		  */

		double get_fitness() const;


		/**
		  * @brief Check the tree length.
		  *
		  * @return Tree length.
		  */

		unsigned get_tree_length() const;

		/**
		 * @brief Given a tree, asign that tree to the actual expression 
		 *
		 * @param new_tree Arbol que formará la expresion
		 *
		 *
		 */

		void assign_tree ( const std::vector<Node> & new_tree);


		/**
		  * @brief Evaluate a expression with new data.
		  *
		  * @param data Data to evaluate the expression
		  * @param labels Labels associated to data.
		  * @param evaluation_f Function used to evaluate the expression
		  * @param force_evaluation Boolean to force evaluation, if false, if the expression has not changed since the last evaluation, it will not be evaluated
		  *
		  * @post fitness = Error returned by evaluation_f in data using the expression.
		  */

		void evaluate_expression(const std::vector<std::vector<double>> & data,
									 const std::vector<double> & labels,
								 	 aux::eval_function_t evaluation_f,
								 	 const bool force_evaluation = false);

		/**
		  * @brief Evaluate a unique data using the expression.
		  *
		  * @param data Data to evaluate
		  *
		  * @return Estimated value by expression.
		  */

		double evaluate_data(const std::vector<double> & data) const ;

		/**
		  * @brief Exchange certain part of the expression by another given expression. 
		  * 
		  * @param another Another expression from where to exchange the subtree.
		  * @param pos Position where to start the exchange.
		  * @param crossover_length Length to exchange.
		  * @param son Expression where the result will be stored.
		  *
		  * @pre another Is not an empty expression.
		  *
		  * @return True if the exchange success.
		  */

		bool exchange_subtree(const Expression & another, const unsigned pos,
										  const unsigned crossover_length,
										  Expression & son) const;

		/**
		  * @brief Crossover between two trees representing expressions.
		  *
		  * @param another Another expression, the crossover will be the result of crossing this and another.
		  * @param son1 Expression to save the first son of the crossover.
		  * @param son2 Expression to save the second son of the crossover.
		  *
		  * @pre another Is not an empty expression.
		  */


		void tree_crossover(const Expression & another, Expression & son1, Expression & son2) const;

		/**
		  * @brief Set the expression as unevaluated.
		  *
		  */

		void no_longer_evaluated();

		/**
		  * @brief Assign operator: Copies an expression.
		  *
		  * @param another Expression to copy.
		  *
		  * @return Reference to this expression, allows Expression a = b = c;
		  */

		Expression & operator= (const Expression & another);

		/**
		  * @brief Obtain the subtree that represents an expression since certain position
		  *
		  * @param subtree Tree where to search
		  * @param pos Position from where to get the subtree in subtree.
		  *
		  * @return subtree expressed in preorder
		  */

		std::vector<Node> get_subtree(const std::vector<Node> & subtree, int pos) const;

		/**
		  * @brief Comput the depth of an expression
		  *
		  * @param start Start node position where to start the depth compute
		  *
		  * @return Depth of the expression since the given node.
		  */

		unsigned compute_depth(const unsigned start = 0) const;


		/**
		 * @brief Obtain a copy of the tree representing our expression
		 *
		 * @return Expression tree
		 */

		std::vector<Node> get_tree() const;

		/**
		 * @brief Check if two expressions have the same tree
		 *
		 * @param another Expression to compare
		 *
		 * @return True if both have the same tree.
		 */

		bool have_same_tree( const Expression & another) const;


		/**
		  * @brief Function to get an expression as an string
		  *
		  * @param stack Container with the expression in preorder..
		  * @param result Partial result with the current parsed string
		  * @param l_value Boolean: true points that we are reading a left part of an expression,
		  *  (the result go to the left side of the string)
		  *
		  * @return Result of parsing the stack as a string
		  */


		std::string get_string_expression(std::stack<Node> & stack,
													  std::string result,
													  const bool l_value) const;

		/**
		  * @brief Get the expression as string.
		  *
		  * @return String associated to the expression
		  */

		std::string string_expression() const;


		/**
		 *
		 *  @brief Mutation of Genetic Programming
		 *
		 *  @param num_vars Number of possible variables that the expression can use
		 *
		 */

		void mutate_GP(const int num_vars);


		/**
		 * @brief Operator to comparaise if two expressions are the same
		 *
		 * @param another Expression to compare
		 *
		 * @return True if both expressions are equals
		 */

		bool operator== (const Expression & another) const;

		/**
		 * @brief Operator to comparaise if two expressions are the different
		 *
		 * @param another Expression to compare
		 *
		 * @return True if both expressions are NOT equals
		 */

		bool operator!= (const Expression & another) const;

		/**
		  * @brief Output an expression in a output stream
		  *
		  * @param os Stream to send the Expression
		  * @param exp Expression to show
		  *
		  * @return Stream used in the call, allows << a << b << c.
		  */

		friend std::ostream & operator<< (std::ostream & os,
													 const Expression & exp);


		/**
		 *
		 * @brief Check if this Expression is better than another using its fitness
		 */

		 bool best_fitness (const Expression & another);

		 /**
 		 *
 		 * @brief Check if an Expression is considered less by another
 		 *
 		 */

		 bool operator < (const Expression & another);

};

} // namespace expressions_algs

#endif

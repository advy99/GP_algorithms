/**
  * \@file Node.hpp
  * @brief Node class header file
  *
  */

#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

#include "expressions_algs/aux_expressions_alg.hpp"

namespace expressions_algs {

/**
  * @brief Possible node types to be used
  */

enum class NodeType {NUMBER, VARIABLE, PLUS, MINUS, DOT, DIVISION};

/**
  *  @brief  Node Class
  *
  * An instance of the Node type will be a representation of the type of the node and a value of that node.
  *
  *
  * @author Antonio David Villegas Yeguas
  * @date Julio 2020
  */

class Node {
	private:
		/**
		  * @page repNode Representation of the node class
		  *
		  * @section faNode Abstraction function
		  *
		  * A valid object @e rep of class Node is given by a type of a node and its value.
		  *
		  *
		  * rep.node_type_
		  * rep.value_
		  * rep.numeric_value_
		  *
		  */

		/**
		  * @brief Node type
		  */
		NodeType node_type_;

		/**
		  * @brief Variable value associated to the node
		  */

		int value_;

		/**
		  * @brief Numerical value associated with the node
		  */

		double numeric_value_;

	public:

		/**
		 * @brief Constructor without arguments. We create an empty Node.
		 *
		 *
		 */

		Node();

		/**
		  * @brief Set the node type to a random operator among the possible operators.
		  *
		  * @pre node_type != NodeType::NUMBER && node_type != NodeType::VARIABLE
		  */


		void set_random_node_type();

		/**
		  * @brief Set the value of the node to a random term
		  *
		  * @param num_numbers Number of numbers to be taken into account when choosing value
		  * @param num_variables Number of variables to take into account when choosing the value.
		  *
		  * @pre node_type == NodeType::NUMBER || node_type == NodeType::VARIABLE
		  */

		void set_random_term(const int num_numbers, const int num_variables);



		/**
		  * @brief Set the value of the node to a random term.
		  *
		  * @param num_variables Number of variables to take into account when choosing the value.
		  *
		  * @pre node_type == NodeType::VARIABLE
		  */

		void set_random_term(const int num_variables);

		/**
		 * @brief Get the type of the node associated with the Node.
		 *
		 * @return Node type.
		 *
		 */

		NodeType get_node_type() const;

		/**
		 * @brief Get the value associated with the Node.
		 *
		 * @pre node_type == NodeType::VARIABLE
		 *
		 * @return value associated with the Node
		 */

		int get_value() const;

		/**
		 * @brief Obtain the numerical value associated with the Node.
		 *
		 * @pre node_type == NodeType::NUMBER
		 *
		 * @return numerical value associated with the Node
		 */

		double get_numeric_value() const;


		/**
		 * @brief Set the numerical value of the Node.
		 *
		 * @pre node_type == NodeType::NUMBER
		 *
		 * @param value Numerical value to be set in the Node.
		 *
		 */
		void set_numeric_value(const double value);


		/**
		 *
		 * @brief Set the type of the Node..
		 *
		 * @param type New Node type
		 *
		 *
		 */

		void set_node_type(const NodeType type);


		/**
		 * @brief Set the value of the 
		 *
		 * @pre node_type == NodeType::NUMBER
		 *
		 * @param val Value to be set in the Node.
		 *
		 */

		void set_value(const double val);


		/**
		 * @brief Comparison operator with another Node
		 *
		 * @param another Node to compare with
		 *
		 * @return True if both Node objects are the same, false otherwise
		 */

		bool operator==(const Node & another) const;

		/**
		 * @brief Comparison operator with another Node
		 *
		 * @param another Node to compare with
		 *
		 * @return True if both Node objects are different, false otherwise
		 */

		bool operator!=(const Node & another) const;

};


} // namespace expressions_algs

#endif

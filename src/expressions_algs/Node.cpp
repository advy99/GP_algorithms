#include "expressions_algs/Node.hpp"

namespace expressions_algs {

Node :: Node(){
	node_type_ = NodeType::NUMBER;
	value_ = 0;
	numeric_value_ = Random::get_float(-10.0, 10.0);
}

void Node :: set_random_term(const int num_numbers, const int num_variables){
	// si es un numero, escogemos un aleatorio entre todos los posibles valores
	if (node_type_ == NodeType::NUMBER){
		value_ = Random::get_int(num_numbers);
	} else {
		// si no, escogemos un aleatorio entre las posibles variables
		value_ = Random::get_int(num_variables);
	}
}


void Node :: set_random_term(const int num_variables){
	// si no, escogemos un aleatorio entre las posibles variables
	value_ = Random::get_int(num_variables);
}

void Node :: set_random_node_type(){
	// obtenemos un aleatorio
	float aleatorio = Random::get_float();

	// le damos la misma probabilidad a cada operador
	if (aleatorio < 1.0/4.0){
		node_type_ = NodeType::PLUS;
	} else if (aleatorio < 2.0/4.0) {
		node_type_ = NodeType::MINUS;
	} else if (aleatorio < 3.0/4.0) {
		node_type_ = NodeType::DOT;
	} else {
		node_type_ = NodeType::DIVISION;
	}
}


NodeType Node :: get_node_type() const {
	return node_type_;
}

int Node :: get_value() const {
	return value_;
}

double Node :: get_numeric_value() const {
	return numeric_value_;
}

void Node :: set_numeric_value(const double value) {
	numeric_value_ = value;
}


void Node :: set_node_type(const NodeType type){
	node_type_ = type;
}

void Node :: set_value(const double val){
	value_ = val;
}


bool Node :: operator==(const Node & another) const {

	// si no son el mismo type, directamente es false
	bool resultado = this->node_type_ == another.node_type_;

	// si es el mismo type y es NUMBER comprobamos el value
	if ( resultado && (this->node_type_ == NodeType::NUMBER ||
	 	  this->node_type_ == NodeType::VARIABLE) ) {
		resultado = this->value_ == another.value_;
	}
	// si el type del nodo es el mismo y es un operador, nos da
	// igual el value, es el mismo nodo

	return resultado;

}

bool Node :: operator!=(const Node & another) const {
	return !(*this == another);
}


} // namespace expressions_algs

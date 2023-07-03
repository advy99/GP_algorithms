#include "expressions_algs/GA_P_Expression.hpp"


namespace expressions_algs {

GA_P_Expression :: GA_P_Expression(const unsigned max_depth){

	// establecemos la profundidad a la maxima dada
	max_depth_ = max_depth;

	// inicilizamos la expresion vacia
	initialize_empty();


	// inicializamos el cromosoma de forma aleatoria
	initialize_chromosome(max_depth);
}

GA_P_Expression :: GA_P_Expression(const std::vector<Node> & subtree, const unsigned max_depth){

	// establecemos la profundidad maxima a la dada
	max_depth_ = max_depth;

	// inicializamos la expresion vacia
	initialize_empty();

	initialize_chromosome(max_depth);

	// obtenemos el subtree
	tree_ = get_subtree(subtree, 0);


}


GA_P_Expression :: GA_P_Expression(const unsigned max_length, const double prob_variable,
							const unsigned num_vars, const unsigned max_depth){

	max_depth_ = max_depth;

	initialize_empty();

	// inicializamos el cromosoma el cromosoma
	initialize_chromosome(max_depth);

	// generamos una expresion aleatoria
	generate_random_expression(max_length, prob_variable, num_vars);


}


void GA_P_Expression :: initialize_chromosome(const unsigned length){
	chromosome_.resize(length);
	// para cada elemento escogemos un numero aleatorio en [-10, 10]
	for (unsigned i = 0; i < chromosome_.size(); i++){
		chromosome_[i] = Random::get_float(-10.0f, 10.0f);
	}

}

void GA_P_Expression :: initialize_empty(){

	Expression::initialize_empty();

	chromosome_ = std::vector<double>();
}



void GA_P_Expression :: copy_data(const GA_P_Expression & otra) {

	Expression::copy_data(otra);

	chromosome_ = otra.chromosome_;
}


GA_P_Expression :: GA_P_Expression(const GA_P_Expression & otra) : Expression(otra){

	chromosome_ = otra.chromosome_;

}



GA_P_Expression & GA_P_Expression :: operator= (const GA_P_Expression & otra){
	// si no es ella misma
	if (this != &otra){

		// copiamos los data de la otra
		copy_data(otra);
	}

	// devolvemos el objeto actual
	return (*this);

}



bool GA_P_Expression :: generate_random_expression(const unsigned longitud_maxima,
														const double prob_variable,
														const unsigned num_variables){

	bool exito = Expression::generate_random_expression(longitud_maxima, prob_variable, num_variables);

	for (unsigned i = 0; i < get_tree_length(); i++) {
		if (tree_[i].get_node_type() == NodeType::NUMBER) {
			tree_[i].set_random_term(chromosome_.size(), num_variables);
		}
	}

	return exito;

}


// en reemplazo de evaluate_data, usa la del parent paro llama a esta para obtener un NUMBER
double GA_P_Expression :: get_number ( const Node & n) const {
	return chromosome_[n.get_value()];
}



double GA_P_Expression :: delta(const int generation, const int max_generaciones, const double value) {
	double aleatorio = Random::get_float();

	double sub = 1.0 - ((double)generation / (double)(max_generaciones));
	// TODO: parametrizar B
	double potencia = std::pow(sub, 5);
	double subtotal = std::pow(aleatorio, potencia);

	return (value * (1.0 - subtotal));
}


void GA_P_Expression :: mutate_ga(const int generation, const int max_generaciones) {

	int pos_mutacion = Random::get_int(chromosome_.size());

	if ( Random::get_float() < 0.5) {
		chromosome_[pos_mutacion] += delta(generation, max_generaciones, 1.0 - chromosome_[pos_mutacion]);
	} else {
		chromosome_[pos_mutacion] -= delta(generation, max_generaciones, chromosome_[pos_mutacion]);
	}
}


bool GA_P_Expression :: same_chromosome ( const GA_P_Expression & otra) const {

	// comprobamos si el arbol es igual
	bool resultado = chromosome_.size() == otra.get_chromosome_length();

	// si el arbol coincide, comparamos el cromosoma
	if ( resultado ) {
		for ( unsigned i = 0; i < get_chromosome_length(); i++) {
			resultado = resultado && aux::compare_floats(chromosome_[i], otra.chromosome_[i], 0.005);
		}
	}

	return resultado;

}


std::vector<double> GA_P_Expression :: get_chromosome () const {
	return chromosome_;
}


void GA_P_Expression :: blx_alpha_crossover(const GA_P_Expression & otra, GA_P_Expression & son1, GA_P_Expression & son2, const double alfa) const{

	if ( otra.chromosome_.size() != chromosome_.size() ) {
		std::cerr << "Cruzando dos cromosomas de distinta length" << std::endl;
	}

	std::vector<double> cromosoma_actual;
	cromosoma_actual.resize(chromosome_.size());

	std::vector<double> cromosoma_otro;
	cromosoma_otro.resize(otra.chromosome_.size());

	double punto_parent, punto_madre, seccion;

	for ( unsigned i = 0; i < chromosome_.size(); i++){
		punto_madre = chromosome_[i];
		punto_parent = otra.chromosome_[i];

		if ( punto_madre > punto_parent ) {
			double intercamio = punto_madre;
			punto_madre = punto_parent;
			punto_parent = intercamio;
		}

		seccion = punto_parent - punto_madre;

		punto_madre = punto_madre - seccion * alfa;
		punto_parent = punto_parent + seccion * alfa;

		if ( punto_madre < 0.0) {
			punto_madre = 0.0;
		}

		if ( punto_parent > 1.0) {
			punto_parent = 1.0;
		}

		cromosoma_actual[i] = punto_madre + Random::get_float() *
														(punto_parent - punto_madre);

		cromosoma_otro[i] = punto_madre + Random::get_float() *
														(punto_parent - punto_madre);
	}


	son1.assign_chromosome(cromosoma_actual);
	son2.assign_chromosome(cromosoma_otro);


}


unsigned GA_P_Expression :: get_chromosome_length() const{
	return chromosome_.size();
}



void GA_P_Expression :: assign_chromosome(const std::vector<double> & new_chromosome){
	chromosome_ = new_chromosome;
}

bool GA_P_Expression :: same_niche(const GA_P_Expression & otra) const {

	bool resultado = chromosome_.size() == otra.get_tree_length();

	unsigned i = 0;
	while (resultado && i < chromosome_.size()) {
		resultado = tree_[i].get_node_type() == otra.get_tree()[i].get_node_type();
		i++;
	}

	return resultado;
}


bool GA_P_Expression :: operator == (const GA_P_Expression & otra) const {
	return same_chromosome(otra) && have_same_tree(otra);
}

}

#include "expressions_algs/Expression.hpp"


namespace expressions_algs {

Expression :: Expression(const unsigned max_depth){

	// establecemos la profundidad a la maxima dada
	max_depth_ = max_depth;

	// inicilizamos la expresion vacia
	initialize_empty();

}

Expression :: Expression(const std::vector<Node> & subtree, const unsigned max_depth){

	// establecemos la profundidad maxima a la dada
	max_depth_ = max_depth;

	// inicializamos la expresion vacia
	initialize_empty();

	// obtenemos el subtree
	(*this) = get_subtree(subtree, 0);

}

Expression :: Expression(const unsigned max_length, const double prob_variable,
							const unsigned num_vars, const unsigned max_depth){

	max_depth_ = max_depth;

	num_variables_ = num_vars;

	initialize_empty();

	// generamos una expresion aleatoria
	generate_random_expression(max_length, prob_variable, num_vars);
}


Expression :: Expression ( const std::string & nombre_archivo, const unsigned max_length, const unsigned num_variables) {

	max_depth_ = max_length;
	num_variables_ = num_variables;

	std::ifstream is (nombre_archivo.c_str());

	if ( !is.is_open() ) {
		std::cerr << "ERROR: No se ha podido abrir " << nombre_archivo << std::endl;
	} else {
		std::string linea_expresion = "";

		std::getline(is, linea_expresion);

		std::vector<Node> pila_expresion;

		pila_expresion = get_expression(linea_expresion);

		if ( pila_expresion.size() > max_depth_) {
			std::cerr << "ERROR: Expression más grande del límite dado." << std::endl;
		} else {
			tree_ = pila_expresion;
		}

	}

}

std::vector<Node> Expression :: get_expression(const std::string & linea_expresion) {

	std::istringstream buffer_exp (linea_expresion);

	std::string part;


	Node operador;
	NodeType tipo_operador = NodeType::NUMBER;

	std::vector<Node> parte_izquierda;
	std::vector<Node> parte_derecha;

	buffer_exp >> part;
	do {

		if ( part == "(" ) {
			std::string sub_string = get_parenthesis_string(buffer_exp);
			parte_izquierda = get_expression(sub_string);
		} else {
			if (part[0] == 'x') {
				// si es una variable
				Node var;
				var.set_node_type(NodeType::VARIABLE);
				// elimino la x
				part.erase(part.begin());
				var.set_value(std::atoi(part.c_str()) );
				parte_izquierda.push_back(var);
			} else {
				// si es otra cosa: un numero
				Node num;
				num.set_node_type(NodeType::NUMBER);
				num.set_numeric_value(std::atof(part.c_str()) );
				parte_izquierda.push_back(num);
			}
		}

		if ( !buffer_exp.eof() ) {
			buffer_exp >> part;

			// si he leido una parte, ahora leo el operador
			if (part == "+") {
				tipo_operador = NodeType::PLUS;
			} else if ( part == "-" ) {
				tipo_operador = NodeType::MINUS;
			} else if ( part == "*" ) {
				// si es un operador
				tipo_operador = NodeType::DOT;
			} else if ( part == "/" ) {
				// si es un operador
				tipo_operador = NodeType::DIVISION;
			}

			buffer_exp >> part;

			if ( part == "(" ) {
				std::string sub_string = get_parenthesis_string(buffer_exp);
				parte_derecha = get_expression(sub_string);
			} else {
				if (part[0] == 'x') {
					// si es una variable
					Node var;
					var.set_node_type(NodeType::VARIABLE);
					// elimino la x
					part.erase(part.begin());
					var.set_value(std::atoi(part.c_str()));
					parte_derecha.push_back(var);
				} else {
					// si es otra cosa: un numero
					Node num;
					num.set_node_type(NodeType::NUMBER);
					num.set_numeric_value(std::atof(part.c_str()));
					parte_derecha.push_back(num);
				}
			}

		}

		buffer_exp >> part;
	} while (!buffer_exp.eof());


	std::vector<Node> resultado;

	if (tipo_operador != NodeType::NUMBER) {
		// metemos el operador
		operador.set_node_type(tipo_operador);
		resultado.push_back(operador);

		// metemos la parte izquierda
		resultado.insert( resultado.end(), parte_izquierda.begin(), parte_izquierda.end() );

		// metemos la parte derecha
		resultado.insert( resultado.end(), parte_derecha.begin(), parte_derecha.end() );
	} else {
		resultado = parte_izquierda;
	}

	return resultado;

}

std::string Expression :: get_parenthesis_string(std::istringstream & buffer_exp) {
	// si es el comienzo de una parte
	int contador_parentesis = 1;

	std::string sub_string = "";
	std::string part = "";

	while ( contador_parentesis != 0) {
		buffer_exp >> part;
		if (part == ")") {
			contador_parentesis--;
		} else if (part == "(") {
			contador_parentesis++;
		}

		if ( contador_parentesis != 0) {
			sub_string += part + " ";
		}
	}

	return sub_string;
}


std::vector<Node> Expression :: get_subtree(const std::vector<Node> & arbol, int posicion) const{
	std::vector<Node> sol;

	if ( arbol.size() > 0) {
		// al principio comenzamos con un nodo
		unsigned ramas_libres = 1;

		// mientras tenga ramas que visitar
		while(ramas_libres > 0){
			// si no es ni un numero ni una variable
			if (arbol[posicion].get_node_type() != NodeType::NUMBER &&
				arbol[posicion].get_node_type() != NodeType::VARIABLE){
				// es un operador, tiene dos ramas
				ramas_libres += 2;
			}
			// en todo caso, he visitado ese nodo, y el tamaño se incrementa en uno
			ramas_libres--;
			sol.push_back(arbol[posicion]);
			posicion++;

		}

	}


	// y la devolvemos
	return sol;
}



void Expression :: initialize_empty(){
	// una expresion vacia no tiene arbol
	tree_ = std::vector<Node>();
	num_variables_ = 0;
	no_longer_evaluated();
}



void Expression :: copy_data(const Expression & otra){
	// copiamos todos los valores
	fitness_            = otra.fitness_;
	is_evaluated_           = otra.is_evaluated_;
	max_depth_ = otra.max_depth_;
	num_variables_   = otra.num_variables_;
	tree_              = otra.tree_;

}


Expression :: Expression(const Expression & otra){
	// al initialize vacia mantenemos la prfuncidad que queramos
	max_depth_ = otra.max_depth_;

	// inicializamos vacia para poner punteros a nullptr
	initialize_empty();

	// copiamos la otra con el operador = ya implementado
	(*this) = otra;
}

Expression & Expression :: operator= (const Expression & otra){
	// si no es ella misma
	if (this != &otra){
		// copiamos los data de la otra
		copy_data(otra);
	}

	// devolvemos el objeto actual
	return (*this);

}

bool Expression :: generate_random_expression(const unsigned longitud_maxima,
														const double prob_variable,
														const unsigned num_variables){

	max_depth_ = longitud_maxima;
	// si no tenemos espacio, redimensionamos
	tree_ = std::vector<Node>();
	tree_.resize(longitud_maxima);

	// comenzamos con una rama libre
	int ramas_libres = 1;
	unsigned i = 0;

	// mientras tengamos length y tengamos ramas sueltas
	while (i < longitud_maxima && ramas_libres > 0){
		// obtenemos la probabilidad de que sea un operador
		float prob_operador = static_cast<float>(ramas_libres*ramas_libres+1) /
									 static_cast<float>(longitud_maxima-i);

		// si es un operador, lo generamos
		if (Random::get_float() > prob_operador){
			tree_[i].set_random_node_type();
			// tenemos una rama más libre, la actual que sería el
			// termino de la izquierda y una más para el termino de la derecha
			ramas_libres++;

		} else {
			// si es un simbolo terminal, generamos un aleatorio
			// para ver si es variable o numero
			if (Random::get_float() < prob_variable){
				tree_[i].set_node_type(NodeType::VARIABLE);
				tree_[i].set_random_term(num_variables);
			} else {
				tree_[i].set_node_type(NodeType::NUMBER);
				tree_[i].set_numeric_value(Random::get_float(-10.0, 10.0));
			}


			// al ser un terminal, esta rama ya no esta libre y quitamos una
			ramas_libres--;
		}

		// hemos rellenado un hueco del arbol
		i++;
	}



	// si lo hemos rellenado sin dejar ramas libres, esta bien
	bool exito = ramas_libres == 0;

	// si hemos dejado ramas, sacamos un error por la salida de error
	if (!exito){ 
		std::cerr << "Generación incorrecta de expresión aleatoria." << std::endl;
	}

	// la length del arbol es i, y la expresion no esta evaluada
	tree_.resize(i);

	no_longer_evaluated();

	return exito;

}

double Expression :: get_number ( const Node & n) const {
	return n.get_numeric_value();
}

double Expression :: evaluate_data(std::stack<Node> & pila,
										const std::vector<double> & dato) const {

	double resultado = 0.0;

	// si la pila esta vacia, devolvemos el value
	if (pila.empty()){
		resultado = 0.0;

	} else if (pila.top().get_node_type() == NodeType::NUMBER){
		// si el tope de la pila es un nodo de type Numero, miramos su value en la
		// posicion del cromosoma correspondiente
		resultado = get_number(pila.top());

		// eliminamos de la pila, y lo devolvemos
		pila.pop();

	} else if (pila.top().get_node_type() == NodeType::VARIABLE){
		// si es una variable, la consultamos en el dato dado, eliminamos el nodo
		// de la pila y devolvemos el value del dato
		resultado = dato[pila.top().get_value()];
		pila.pop();

	} else {
		// si es un operador, guardamos la operacion
		NodeType operacion = pila.top().get_node_type();

		// la eliminamos de la pila
		pila.pop();

		// consultamos el value de la rama izquierda y la derecha
		double valor_izda = evaluate_data(pila, dato);
		double valor_dcha = evaluate_data(pila, dato);

		// aplicamos el operador con ambas ramas y devolvemos el resultado
		if (operacion == NodeType::PLUS){
			resultado = valor_izda + valor_dcha;

		} else if (operacion == NodeType::MINUS){
			resultado = valor_izda - valor_dcha;

		} else if (operacion == NodeType::DOT){
			resultado = valor_izda * valor_dcha;

		} else if (operacion == NodeType::DIVISION){
			if (!aux::compare_floats(valor_dcha, 0.0) ){
				resultado = valor_izda / valor_dcha;
			} else {
				resultado = 1.0f;
			}
		}

	}

	return resultado;

}

double Expression :: evaluate_data(const std::vector<double> & dato) const {
	double resultado;

	// pila donde almacenaremos la expresion
	std::stack<Node> pila;

	//volcamos la expresion en la pila
	for (int i = (int)get_tree_length() - 1; i >= 0; i--){
		pila.push(tree_[i]);
	}

	// la evaluamos para el dato i
	resultado = evaluate_data(pila, dato);

	return resultado;

}


void Expression :: evaluate_expression(const std::vector<std::vector<double>> &data,
											  const std::vector<double> & labels,
										  	  aux::eval_function_t f_evaluacion,
										  	  const bool evaluar){

	// almacenamos como resultado el value de fitness
	double resultado = fitness_;

	std::vector<double> valores_predecidos;
	valores_predecidos.resize(labels.size());

	// si no esta evaluada y el arbol contiene una expresion
	if ( (!is_evaluated_ || evaluar) && tree_.size() > 0){

		// para cada dato
		for (unsigned i = 0; i < data.size(); i++){

			// la evaluamos para el dato i
			valores_predecidos[i] = evaluate_data(data[i]);
		}

		// hacemos la media de los cuadrados
		resultado = f_evaluacion(valores_predecidos, labels);


	}
	// actualizamos el fitness y que esta evaluada y devolvemos el resultado
	fitness_ = resultado;
	is_evaluated_ = true;

}

bool Expression :: is_evaluated() const{
	return is_evaluated_;
}

double Expression :: get_fitness() const{
	return fitness_;
}

unsigned Expression :: get_tree_length() const{
	return tree_.size();
}



bool Expression :: exchange_subtree(const Expression & otra, const unsigned pos,
													const unsigned cruce_padre,
												   Expression & hijo) const {

	//Expression madre_cortada((tree_ + pos), max_depth_);

	std::vector<Node> madre_cortada = get_subtree(tree_, pos);

	//Expression padre_cortado((otra.tree_ + cruce_padre), otra.max_depth_);

	std::vector<Node> padre_cortado = get_subtree(otra.get_tree(), cruce_padre);

	// sumamos, la parte de la mom, la length de la parte del dad, y lo que nos queda de mom tras el cruce
	unsigned nueva_longitud = pos + padre_cortado.size() + (get_tree_length() - madre_cortada.size() - pos);


	bool podido_cruzar = nueva_longitud <= max_depth_;


	if ( podido_cruzar) {
		// cruce
		std::vector<Node> arbol_hijo;
		arbol_hijo.resize(nueva_longitud);

		for ( unsigned i = 0; i < pos; i++) {
			arbol_hijo[i] = tree_[i];
		}

		for ( unsigned i = 0; i < padre_cortado.size(); i++) {
			arbol_hijo[i + pos] = padre_cortado[i];
		}

		unsigned index_hijo = 0;

		// i en este caso comienza en el punto donde acaba el arbol que hemos intercambiado
		for ( unsigned i = pos + madre_cortada.size(); i < get_tree_length(); i++) {
			// nos ponemos donde habiamos dejado de copiar el dad
			index_hijo = pos + padre_cortado.size() + (i - (pos + madre_cortada.size()) ) ;
			arbol_hijo[index_hijo] = tree_[i];
		}

		//hijo.assign_chromosome(cromosoma, longitud_cromosoma);
		hijo.assign_tree(arbol_hijo);
	}

	return podido_cruzar;

}



void Expression :: tree_crossover(const Expression & otra, Expression & son1, Expression & son2) const {

	int punto_cruce_madre;
	int punto_cruce_padre;

	bool cruce_mal;

	// cruzamos mientras se cruce mal
	do {

		punto_cruce_madre = Random::get_int(get_tree_length());
		punto_cruce_padre = Random::get_int(otra.get_tree_length());

		cruce_mal = !(exchange_subtree(otra, punto_cruce_madre, punto_cruce_padre, son1));

		if ( !cruce_mal ) {
			cruce_mal = !(otra.exchange_subtree((*this), punto_cruce_padre, punto_cruce_madre, son2));
		}

	} while (cruce_mal);


}


void Expression :: assign_tree (const std::vector<Node> & nuevo_arbol) {

	tree_ = nuevo_arbol;

}


void Expression :: no_longer_evaluated(){
	// ponemos la flag a false y establecemos el fitness a NaN
	is_evaluated_ = false;
	fitness_ = std::numeric_limits<double>::infinity();
}


unsigned Expression :: count_levels(std::stack<Node> & pila, unsigned nivel) const{
	// si la pila esta vacia, devolvemos el nivel actual
	if (pila.empty()){
		return nivel;
	} else if (pila.top().get_node_type() == NodeType::NUMBER ||
				  pila.top().get_node_type() == NodeType::VARIABLE) {
		// si en el tope hay un simbolo terminal, eso cuenta como un nivel,
		// eliminamos el nodo de la pila, y devolvemos ese value del nivel
		nivel++;
		pila.pop();
		return nivel;
	} else {
		// es un operador
		nivel++;
		// eliminamos el nodo del operador
		pila.pop();
		unsigned niveles_izda = nivel;
		unsigned niveles_dcha = nivel;

		// miramos el numero de niveles a la derecha y a la izquierda
		niveles_izda = count_levels(pila, nivel);
		niveles_dcha = count_levels(pila, nivel);

		// nos quedamos con el nivel de la rama con mayor profundidad
		nivel = niveles_izda > niveles_dcha ? niveles_izda : niveles_dcha;

		return nivel;
	}
}

unsigned Expression :: compute_depth(const unsigned comienzo) const {

	unsigned profundidad = 0;
	std::stack<Node> pila;

	//volcamos la expresion en la pila
	for (int i = static_cast<int>(get_tree_length() - 1); i >= static_cast<int>(comienzo); i--){
		pila.push(tree_[i]);
	}
	// contamos los niveles de toda la pila
	profundidad = count_levels(pila, profundidad);


	return profundidad;

}




std::string Expression :: get_string_expression(std::stack<Node> & pila,
															 std::string resultado,
															 const bool izda) const{
	// si la pila esta vacia, devolvemos el resultado
	if (pila.empty()){
		return resultado;
	} else if (pila.top().get_node_type() == NodeType::NUMBER){
		// si es un numero, lo consultamos en el cromosoma
		// dependiendo de si estamos mirando el nodo de la izquierda o de
		// la derecha ponemos primero el numero y lo que llevamos
		// o lo que llevamos y el numero
		if (izda){
			// resultado = std::to_string(cromosoma[pila.top().get_value()]) +
			resultado = std::to_string(get_number(pila.top()) ) +
							" " + resultado;
		} else {
			resultado = resultado + " " +
							std::to_string(get_number(pila.top()));
		}

		// eliminamos el nodo de la pila y devolvemos el resultado
		pila.pop();
		return resultado;

	} else if (pila.top().get_node_type() == NodeType::VARIABLE){
		// si es una variable, ponemos xN
		// de nuevo, dependiendo de si miramos el nodo de la izquierda o el
		// de la derecha lo ponemos aun ladou a another
		if (izda){
			resultado = "x" + std::to_string(pila.top().get_value()) +
							" " + resultado;
		} else {
			resultado = resultado + " x" +
							std::to_string(pila.top().get_value());
		}

		// eliminamos el nodo y devolvemos el resultado
		pila.pop();
		return resultado;
	} else {

		// sies un operador, obtenemos el value
		std::string value;
		if (pila.top().get_node_type() == NodeType::PLUS){
			value = "+";
		} else if (pila.top().get_node_type() == NodeType::MINUS){
			value = "-";
		} else if (pila.top().get_node_type() == NodeType::DOT){
			value = "*";
		} else if (pila.top().get_node_type() == NodeType::DIVISION){
			value = "/";
		}

		// eliminamos su nodo
		pila.pop();
		std::string izquierda;
		std::string derecha;

		// miramos la expresion que tienen a la izquierda y a la derecha
		izquierda = get_string_expression(pila, "", true) ;
		derecha = get_string_expression(pila, "", false);

		// la concatenamos, lo que tiene a la izquierda, el operador, y lo de la
		// derecha, todo entre parentesis
		std::string total = "( " + izquierda + " " + value + " " + derecha + " )";

		// y lo añadimos al resultado, dependiendo de si estamos a la izqda o no
		if (izda){
			resultado = total + resultado;
		} else {
			resultado = resultado + total;
		}

		return resultado;

	}



}



std::vector<Node> Expression:: get_tree () const {
	return tree_;
}


std::string Expression :: string_expression() const {
	std::string resultado = "";

	std::stack<Node> pila;

	// volcamos la pila
	for (int i = (int)get_tree_length() - 1; i >= 0; i--){
		pila.push(tree_[i]);
	}

	// obtenemos el string de toda la pila
	resultado = get_string_expression(pila, resultado, true);

	return resultado;
}

std::ostream & operator<< (std::ostream & os, const Expression & exp){
	// obtenemos el string y lo sacamos por el flujo
	std::string exp_string = exp.string_expression();

	os << exp_string;

	return os;
}



void Expression :: mutate_GP (const int num_vars) {

	num_variables_ = num_vars;
	int posicion = Random::get_int(tree_.size());

	float aleatorio = Random::get_float();

	if ( aleatorio < 0.5) {
		// primera opcion, cambiar un termino por another
		NodeType type = tree_[posicion].get_node_type();

		if ( type == NodeType::NUMBER || type == NodeType::VARIABLE){
			if ( Random::get_float() < 0.5) {
				tree_[posicion].set_node_type(NodeType::VARIABLE);
				tree_[posicion].set_random_term(num_vars);
			} else {
				tree_[posicion].set_node_type(NodeType::NUMBER);
				tree_[posicion].set_numeric_value(Random::get_float(-10.0, 10.0));
			}

		} else {
			tree_[posicion].set_random_node_type();
		}
	} else {
		// generamos un arbol aleatorio en la posicion

		bool cruce_mal;
		Expression hijo = (*this);

		do {

			Expression exp_aleatorio(max_depth_, 0.3, num_vars, max_depth_);

			cruce_mal = !(exchange_subtree(exp_aleatorio, posicion, 0, hijo));

		} while (cruce_mal);

		assign_tree(hijo.tree_);

	}

}


bool Expression :: have_same_tree( const Expression & otra) const {
	bool resultado = tree_.size() == otra.tree_.size();

	for (unsigned i = 0; i < tree_.size() && resultado; i++){
		resultado = tree_[i] == otra.tree_[i];
	}

	return resultado;
}

bool Expression :: operator == ( const Expression & otra) const {
	return have_same_tree(otra);
}

bool Expression :: operator != ( const Expression & otra) const {
	return !(*this == otra);
}

bool Expression :: best_fitness (const Expression & otra) {
	return fitness_ < otra.get_fitness();
}

bool Expression :: operator < (const Expression & otra) {
	return best_fitness(otra);
}


} // namespace expressions_algs

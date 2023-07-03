namespace expressions_algs {

template <class T>
Population<T> :: Population(){
	// una poblacion vacia no tiene nada
	expressions_     = std::vector<T>();
	mejor_individuo_ = -1;
}

template <class T>
Population<T> :: Population(const unsigned tam, const unsigned lon_expre,
							const double prob_var, const unsigned num_vars,
							const unsigned prof_expre){
	// liberamos memoria para initialize a vacio
	expressions_ = std::vector<T>();


	// reservamos memoria para tam individuos
	expressions_.resize(tam);
	// inicializamos todas las expresiones de la poblacion

	for (unsigned i = 0; i < tam; i++){
		expressions_[i] = T(lon_expre, prob_var, num_vars, prof_expre);
	}

}

template <class T>
Population<T> :: Population ( const Population & otra) {
	expressions_ = std::vector<T>();

	(*this) = otra;
}

template <class T>
Population<T> :: ~Population(){
}

template <class T>
void Population<T> :: copy_data(const Population & otra){
	// copiamos los atributos
	mejor_individuo_ = otra.mejor_individuo_;

	expressions_ = otra.expressions_;
}

template <class T>
void Population<T> :: evaluate_population(const std::vector<std::vector<double> > & data,
												  const std::vector<double> & labels,
											  	  aux::eval_function_t f_evaluacion){
	// establecemos el mejor individuo al primero
	mejor_individuo_ = 0;

	if (!expressions_[0].is_evaluated()) {
		expressions_[0].evaluate_expression(data, labels, f_evaluacion);
	}

	// evaluamos el resto de individuos
	#pragma omp parallel for
	for ( unsigned i = 1; i < expressions_.size(); i++){
		if (!expressions_[i].is_evaluated()){
			expressions_[i].evaluate_expression(data, labels, f_evaluacion);
		}

		#pragma omp critical
		{
			if (expressions_[i].get_fitness() < expressions_[mejor_individuo_].get_fitness()){
				mejor_individuo_ = i;
			}
		}

	}
}

template <class T>
double Population<T> :: fitness_sum() const {
	double suma = 0.0;

	for (unsigned i = 0; i < expressions_.size(); i++){
		suma += expressions_[i].get_fitness();
	}

	return suma;
}

template <class T>
unsigned Population<T> :: select_individual() const {

	std::vector<double> probabilidad;

	probabilidad.resize(expressions_.size());

	double fitness_poblacion = fitness_sum();

	probabilidad[0] = expressions_[0].get_fitness() / fitness_poblacion;

	for (unsigned i = 1; i < expressions_.size(); i++){
		probabilidad[i] = probabilidad[i-1] +
								(expressions_[i].get_fitness() / fitness_poblacion);
	}
	// evitamos errores de redondeo
	probabilidad[expressions_.size() - 1] = 1.0;

	double aleatorio = Random::get_float();

	unsigned index = 0;

	while (aleatorio > probabilidad[index] && index < expressions_.size()){
		index++;
	}

	return index;


}

template <class T>
T Population<T> :: get_best_individual() const {
	return expressions_[mejor_individuo_];
}

template <class T>
unsigned Population<T> :: get_best_individual_index() const {
	return mejor_individuo_;
}

template <class T>
unsigned Population<T> :: get_population_size() const {
	return expressions_.size();
}

template <class T>
T & Population<T> :: operator[] (const unsigned index) {
	return expressions_[index];
}

template <class T>
const T & Population<T> :: operator[] (const unsigned index) const {
	return expressions_[index];
}

template <class T>
Population<T> & Population<T> :: operator= (const Population & otra) {


	if (this != &otra){
		// copiamos los data de la otra
		copy_data(otra);
	}

	return (*this);

}

template <class T>
void Population<T> :: set_individual(const unsigned index, const T & n_individuo) {
	expressions_[index] = n_individuo;
}

template <class T>
void Population<T> :: insert(const T & nuevo_elemento) {
	expressions_.push_back(nuevo_elemento);

	if ( expressions_.size() == 1) {
		mejor_individuo_ = 0;
	} else if (expressions_[mejor_individuo_].get_fitness() > nuevo_elemento.get_fitness()) {
		mejor_individuo_ = expressions_.size() - 1;
	}
}

template <class T>
void Population<T> :: set_best_individual(const int nuevo_mejor) {
	mejor_individuo_ = nuevo_mejor;
}

template <class T>
void Population<T> :: search_best_individual() {
	mejor_individuo_ = -1;

	if ( expressions_.size() > 0) {
		mejor_individuo_ = 0;

		for ( unsigned i = 0; i < expressions_.size(); i++) {
			if (expressions_[i].get_fitness() < expressions_[mejor_individuo_].get_fitness()) {
				mejor_individuo_ = i;
			}
		}
	}

}


template <class T>
void Population<T> :: remove(const unsigned posicion) {
	expressions_.erase(expressions_.begin() + posicion);
	search_best_individual();
}

template <class T>
void Population<T> :: ordenar() {

	std::sort(expressions_.begin(), expressions_.end());
	mejor_individuo_ = 0;

}


} // namespace expressions_algs

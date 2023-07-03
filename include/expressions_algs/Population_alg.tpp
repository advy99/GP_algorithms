
namespace expressions_algs {

template <class T>
Population_alg<T> :: Population_alg() {

}

template <class T>
void Population_alg<T> :: initialize(const unsigned long seed, const unsigned population_size, const unsigned depth, const double prob_var) {

	// inicializamos la semilla
	Random::set_seed(seed);

	expressions_depth_ = depth;
	probabilidad_variable_ = prob_var;
	generate_population(population_size, depth, prob_var, true);

}


template <class T>
void Population_alg<T> :: load_data(const std::vector< std::vector<double> > & caracteristicas, const std::vector<double> & labels ) {
	data_ = caracteristicas;
	output_data_ = labels;
}

template <class T>
void Population_alg<T> :: read_data(const std::string data_file,
							const char comment_char, const char delimiter){
	// abrimos el fichero de lectura
	auto resultado = preprocess::read_data<double>(data_file, comment_char, delimiter);

	data_ = resultado.first;
	output_data_ = resultado.second;

}


template <class T>
int Population_alg<T> :: get_num_data() const {
	return data_.size();
}

template <class T>
void Population_alg<T> :: generate_population(const unsigned population_size, const unsigned profundidad_exp,
									 const double prob_var, const bool sustituir_actual) {
	if ( sustituir_actual ) {
		population_ = Population<T>(population_size, profundidad_exp, prob_var,
									 	 get_num_variables(), get_expressions_max_depth());
	}


}



template <class T>
int Population_alg<T> :: get_num_variables() const {
	return data_[0].size();
}

template <class T>
std::vector<std::vector<double> > Population_alg<T> :: get_all_data() const {
	return data_;
}

template <class T>
std::vector<double> Population_alg<T> :: get_data(const unsigned i) const {
	return data_[i];
}

template <class T>
std::vector<double> Population_alg<T> :: get_all_output_data() const {
	return output_data_;
}


template <class T>
double Population_alg<T> :: get_output_data(const unsigned index) const {
	return output_data_[index];
}

template <class T>
void Population_alg<T> :: initialize_empty() {
	expressions_depth_ = 0;
	data_.clear();
	output_data_.clear();
}


template <class T>
Population<T> Population_alg<T> :: tournament_selection(const unsigned tam_torneo) const {
	// partimos de una poblacion con el mismo tamaño que la actual
	Population<T> resultado;

	std::vector<int> participantes_torneo;
	participantes_torneo.resize(population_.get_population_size());

	for ( unsigned i = 0; i < population_.get_population_size(); i++ ) {
		participantes_torneo[i] = i;
	}

	int mejor_torneo = 0;
	
	// escojo una nueva poblacion del mismo tamaño
	for ( unsigned i = 0; i < population_.get_population_size(); i++) {


		std::shuffle(participantes_torneo.begin(), participantes_torneo.end(), Random::get_generator());

		mejor_torneo = participantes_torneo[0];

		// solo miro hasta tam_torneo, que son los escogidos aleatoriamente
		for ( unsigned i = 1; i < tam_torneo; i++) {
			if ( population_[mejor_torneo].get_fitness() > population_[participantes_torneo[i]].get_fitness()) {
				mejor_torneo = participantes_torneo[i];
			}
		}


		// el ganador del torneo i es el mejor del torneo
		resultado.insert(population_[mejor_torneo]);
	}

	return resultado;
}

template <class T>
T Population_alg<T> :: get_best_individual() const {
	return population_.get_best_individual();
}

template <class T>
unsigned Population_alg<T> :: get_expressions_max_depth() const {
	return expressions_depth_;
}



template <class T>
void Population_alg<T> :: apply_elitism(const T & mejor_ind_anterior) {
	// elitismo
	bool mejor_encontrado = false;
	unsigned i = 0;

	while (i < population_.get_population_size() && !mejor_encontrado) {
		mejor_encontrado = population_[i] == mejor_ind_anterior;
		i++;
	}

	// si no esta el mejor, aplico elitismo
	if ( !mejor_encontrado ){
		population_[population_.get_population_size() - 1] = mejor_ind_anterior;

		if (population_.get_best_individual().get_fitness() > mejor_ind_anterior.get_fitness()) {
			population_.set_best_individual(population_.get_population_size() - 1);
		}

	}

}

template <class T>
std::pair<bool, bool> Population_alg<T> :: apply_GP_mutations(T & son1, T & son2, const double prob_mutacion) {
	std::pair<bool, bool> resultado = std::make_pair(false, false);

	if ( Random::get_float() < prob_mutacion ) {
		// mutacion GP en el primer hijo
		son1.mutate_GP(get_num_variables());
		resultado.first = true;
	}

	if ( Random::get_float() < prob_mutacion ) {
		// mutacion GP en el segundo hijo
		son2.mutate_GP(get_num_variables());
		resultado.second = true;
	}

	return resultado;
}


template <class T>
double Population_alg<T> :: predict(const std::vector<double> & dato) const {
	double resultado = population_.get_best_individual().evaluate_data(dato);

	return resultado;
}

template <class T>
std::vector<double> Population_alg<T> :: predict(const std::vector<std::vector<double> > & data) const {

	std::vector<double> resultado;

	resultado.resize(data.size());

	#pragma omp parallel for
	for ( unsigned i = 0; i < data.size(); i++) {
		resultado[i] = predict(data[i]);
	}

	return resultado;
}


template <class T>
std::pair<T, std::vector<std::vector<double> > > Population_alg<T> :: fit_cv_files( std::pair<matriz<double>, std::vector<double> > & datos_train,  std::pair<matriz<double>, std::vector<double> > &  datos_test, 
														const Parameters & parameters){

	std::vector<std::vector<double >> errores;
	errores.resize(parameters.get_num_evaluation_functions() + 1);

	for (unsigned i = 0; i < errores.size(); i++){
		errores[i].resize(1);
	}

	T mejor_expresion;


	//auto train_datos_aleatorios = preprocess::random_data_reorder(datos_train.first, datos_train.second);
	//load_data(train_datos_aleatorios.first, train_datos_aleatorios.second);
	load_data(datos_train.first, datos_train.second);

	// generamos una nueva población en cada iteración, para asegurarnos que cada fold es independiente
	generate_population(population_.get_population_size(), expressions_depth_, probabilidad_variable_, true);

	// ajustamos para estos nuevos valores
	fit(parameters);

	// predecimos test para mirar el error
	//auto test_datos_aleatorios = preprocess::random_data_reorder(datos_test.first, datos_test.second);
	//auto predicciones = predict(test_datos_aleatorios.first);
	auto predicciones = predict(datos_test.first);

	// rellenamos el error de la función de evaluación con la que entrenamos
	errores[0][0] = parameters.get_evaluation_functions()(predicciones, datos_test.second);

	// calculamos todos los otros errores
	for (unsigned j = 0; j < parameters.get_num_evaluation_functions(); j++) {
		errores[j + 1][0]= parameters.get_error_function(j)(predicciones, datos_test.second);
	}

	mejor_expresion = get_best_individual();
	

	return std::make_pair(mejor_expresion, errores);
}

template <class T>
std::pair<T, std::vector<std::vector<double> > > Population_alg<T> :: perform_k_cross_validation(const unsigned numero_val_cruzada,
				 									 const Parameters & parameters) {

	const std::vector<std::vector<double> > datos_originales = data_;
	const std::vector<double> etiquetas_originales = output_data_;

	auto resultado_datos_aleatorios = preprocess::random_data_reorder(data_, output_data_);

	const int NUM_DATOS_TEST_ITERACION = data_.size() / numero_val_cruzada;

	std::vector<std::vector<double >> errores;
	errores.resize(parameters.get_num_evaluation_functions() + 1);

	for (unsigned i = 0; i < errores.size(); i++){
		errores[i].resize(numero_val_cruzada);
	}

	double error_mejor = std::numeric_limits<double>::infinity();
	T mejor_expresion;

	// para cada iteracion de la validación cruzada;
	for ( unsigned i = 0; i < numero_val_cruzada; i++) {
		// tenemos que hacer la separacion en train/test para esta iteracion
		auto train_test_separado = preprocess::split_train_test<double>(resultado_datos_aleatorios.first,
																				resultado_datos_aleatorios.second,
																				1.0/numero_val_cruzada,
																				NUM_DATOS_TEST_ITERACION * i);

		load_data(train_test_separado.first.first, train_test_separado.first.second);

		// generamos una nueva población en cada iteración, para asegurarnos que cada fold es independiente
		generate_population(population_.get_population_size(), expressions_depth_, probabilidad_variable_, true);

		// ajustamos para estos nuevos valores
		fit(parameters);

		// predecimos test para mirar el error
		auto predicciones = predict(train_test_separado.second.first);

		// rellenamos el error de la función de evaluación con la que entrenamos
		errores[0][i] = parameters.get_evaluation_functions()(predicciones, train_test_separado.second.second);

		// calculamos todos los otros errores
		for (unsigned j = 0; j < parameters.get_num_evaluation_functions(); j++) {
			errores[j + 1][i] = parameters.get_error_function(j)(predicciones, train_test_separado.second.second);
		}

		if (errores[0][i] < error_mejor) {
			mejor_expresion = get_best_individual();
		}

	}

	// reestablecer data originales
	load_data(datos_originales, etiquetas_originales);

	return std::make_pair(mejor_expresion, errores);

}



} // namespace expressions_algs

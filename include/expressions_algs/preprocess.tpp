namespace expressions_algs :: preprocess {


template <class T>
std::pair<matriz<T>, std::vector<T> > read_data(const std::string & data_file,
												const char comment_char, const char delimiter){
	// abrimos el fichero de lectura
	std::ifstream file(data_file);

	matriz<T> data;
	std::vector<T> output_datos;

	// mostramos un error si no podemos abrirlo
	if (!file){
		std::cerr << "Error al abrir " << data_file << std::endl;
	} else {
		// leemos una linea
		std::string linea;
		std::stringstream ss;

		std::getline(file, linea);

		// mientras no encuentre el EOF
		while (!file.eof()){
			// si hemos leido una linea y no es un comentario, la procesamos
			if (linea.size() > 0 && linea[0] != comment_char &&
				 !isblank(linea[0]) ) {
				// leemos por linea
				std::vector<T> datos_linea;
				ss.str(linea);

				std::string str_valor;

				std::getline(ss, str_valor, delimiter);

				// mientras queden valores en la linea
				while (!ss.eof()){

					T dato_a_insert;

					if constexpr ( std::is_same<T, double>::value ) {
						dato_a_insert = strtod(str_valor.c_str(), nullptr);
					} else {
						dato_a_insert = str_valor;
					}

					// los metemos en ese dato
					datos_linea.push_back(dato_a_insert);
					std::getline(ss, str_valor, delimiter);

				}

				T dato_a_insert;

				if constexpr ( std::is_same<T, double>::value ) {
					dato_a_insert = strtod(str_valor.c_str(), nullptr);
				} else {
					dato_a_insert = str_valor;
				}

				// el ultimo dato no tiene separador, lo incluimos
				output_datos.push_back(dato_a_insert);

				// introducimos el dato en la matriz final de data
 				data.push_back(datos_linea);


				// reseteamos el lector de strgin para leer la siguiente linea
				ss.str("");
				ss.clear();

			}

			std::getline(file, linea);
		}



	}


	return std::make_pair(data, output_datos);
}


template <class T>
std::pair<std::pair<matriz<T>, std::vector<T> >, std::pair<matriz<T>, std::vector<T> > >
	split_train_test(matriz<T> data, std::vector<T> labels,
							 const double PORCENTAJE_TEST, const int COMIENZO) {


	matriz<T> datos_test;
	std::vector<T> etiquetas_test;

	const unsigned NUM_DATOS_TEST = data.size() * PORCENTAJE_TEST;

	datos_test.resize(NUM_DATOS_TEST);
	etiquetas_test.resize(NUM_DATOS_TEST);

	bool aleatorio = COMIENZO == -1;
	int index = COMIENZO;

	for ( unsigned i = 0; i < NUM_DATOS_TEST; i++) {

		if (aleatorio ) {
			index = Random::get_int(0, data.size());
		}

		datos_test[i] = data[index];
		etiquetas_test[i] = labels[index];

		auto it_datos = data.begin() + index;
		auto it_etiquetas = labels.begin() + index;

		data.erase(it_datos);
		labels.erase(it_etiquetas);

	}

	return std::make_pair(std::make_pair(data, labels), std::make_pair(datos_test, etiquetas_test));

}

template <class T>
void write_data(const std::string & salida, const matriz<T> & data, const std::vector<T> & labels, const char DELIMITADOR) {
	std::ofstream salida_datos(salida);

	for ( unsigned i = 0; i < data.size(); i++) {
		for ( unsigned j = 0; j < data[i].size(); j++) {
			salida_datos << data[i][j] << DELIMITADOR;
		}

		salida_datos << labels[i] << std::endl;
	}

}



template <class T>
std::pair<matriz<T>, std::vector<T> > random_data_reorder (matriz<T> data,
	 																				  std::vector<T> labels) {
	matriz<T> datos_reordenados;
	std::vector<T> etiquetas_reordenados;

	datos_reordenados.resize(data.size());
	etiquetas_reordenados.resize(labels.size());

	const unsigned num_elementos = data.size();

	for ( unsigned i = 0; i < num_elementos; i++) {
		int index = Random::get_int(0, data.size());

		datos_reordenados[i] = data[index];
		etiquetas_reordenados[i] = labels[index];

		auto it_datos = data.begin() + index;
		auto it_etiquetas = labels.begin() + index;

		data.erase(it_datos);
		labels.erase(it_etiquetas);

	}

	return std::make_pair(datos_reordenados, etiquetas_reordenados);

}

}

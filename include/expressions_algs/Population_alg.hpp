/**
  * \@file Population_alg.hpp
  * @brief Fichero cabecera de la clase Population_alg
  *
  */


#ifndef POPULATION_ALG_H_INCLUDED
#define POPULATION_ALG_H_INCLUDED

#include "expressions_algs/aux_expressions_alg.hpp"
#include "expressions_algs/Population.hpp"
#include "expressions_algs/Parameters.hpp"


/**
 * @brief Clases, definiciones y estructuras necesarias para el Population_alg
 *
 */

namespace expressions_algs {

/**
  *  @brief Clase Population_alg
  *
  *  Una instancia del type Population_alg representará un estimador para los data
  *  dados, utilizando una poblacion del type dado.
  *
  * @tparam T Tipo de la población que utilizará el algoritmo
  *
  *
  * @author Antonio David Villegas Yeguas
  * @date Abril 2021
  */

template <class T>
class Population_alg {
	protected:

		/**
		 *  @brief Constructor vacio
		 *
		 */

		Population_alg ();

		/**
		 *  @brief Método para initialize el algoritmo de poblacion
		 *
		 *  @param seed Semilla aleatoria a utilizar
		 *  @param population_size Tamaño de la poblacion del algoritmo
		 *  @param depth Profundidad máxima si la poblacion es de Expressiones
		 *  @param prob_var Probabilidad de que en la poblacion dada un Node sea una variable
		 */


		void initialize(const unsigned long seed, const unsigned population_size, const unsigned depth, const double prob_var);


		/**
		  * @brief Datos con los que fit el algoritmo
		  *
		  */
		std::vector<std::vector<double> > data_;

		/**
		  * @brief Etiquetas para comprobar el error de la estimación.
		  */
		std::vector<double> output_data_;


		/**
		  * @brief Population de expresiones con el que aplicaremos el algoritmo
		  *
		  */
		Population<T> population_;


		/**
		  * @brief Profundidad máxima de las expresiones si el algoritmo es de expresiones
		  *
		  */

		unsigned expressions_depth_;

		/**
		  * @brief Probabilidad de que un nodo sea una variable
		  *
		  */

		double probabilidad_variable_;



		/**
		  * @brief Inicializar Population_alg vacio
		  *
		 */

		void initialize_empty();

		/**
		 *  @brief Selección de una nueva población por torneo a partir de
		 * la poblacion actual
		 *
		 * @param tam_torneo Tamaño del torneo
		 *
		 * @return Devuelve la poblacion con los T ganadores del torneo.
		 */

		Population<T> tournament_selection(const unsigned tam_torneo) const;


		/**
		 *  @brief Aplicar una mutación en la poblacion si esta es de expresiones
		 *
		 * @param son1 Primer hijo al que aplicar la mutacion
		 * @param son2 Segundo hijo al que aplicar la mutacion
		 *
		 * @param prob_mutacion Probabilidad con la que aplicar a cada hijo una mutacion
		 *
		 * @return Devuelve una pareja de booleanos, verdadero si se ha aplicado la mutacion, falso si no, uno para cada hijo
		 */

		std::pair<bool, bool> apply_GP_mutations(T & son1, T & son2,
			 													 const double prob_mutacion);

	public:

		/**
		 *  @brief Método para leer los data con los que entrenar de un fichero
		 *
		 * @param data_file Ruta al fichero donde leer los data
		 * @param comment_char Caracter utilizado para comentarios en el fichero de data
		 * @param delimiter Caracter delimiter entre los data
		 *
		 *
		 */

		void read_data(const std::string data_file,
							const char comment_char, const char delimiter = ',');

		/**
		  * @brief Cargar vectores de data y labels en GA_P
		  *
		  * @param caracteristicas Matriz de caracteristicas de cada dato
		  *
		  * @param labels Vector de labels, asociadas a cada fila de la matriz de data
		  *
		  *
		 */

		void load_data(const std::vector< std::vector<double> > & caracteristicas, const std::vector<double> & labels );

		/**
		  * @brief Obtener el numero de variables de los data
		  *
		  * @return Numero de variables del problema
		  */

		int get_num_variables() const ;

		/**
		  * @brief Obtener el numero de data
		  *
		  * @return Numero de data
		  */

		int get_num_data() const;

		/**
		  * @brief Generar la población en base a los data cargados
		  *
		  * @param population_size Tamaño de la poblacion a generar
		  *
		  * @param profundidad_exp Profundidad máxima de las expresiones de la población
		  *
		  * @param prob_var Probabilidad de que un nodo terminal sea una variable
		  *
		  * @param sustituir_actual Booleano para sustituir la población por una nueva, en caso de que ya exista una población
		  *
		  * @pre data.size != 0
		  *
		  */

		void generate_population(const unsigned population_size, const unsigned profundidad_exp, const double prob_var, const bool sustituir_actual = false);



		/**
		  * @brief Obtener los data
		  *
		  * @return Datos.
		  */
		std::vector<std::vector<double> > get_all_data() const;


		/**
		  * @brief Obtener el dato de la fila index
		  *
		  * @param index Indice del dato a obtener
		  *
		  * @pre index >= 0 && index < data.size
		  *
		  * @return Dato de la columna index.
		  */
		std::vector<double > get_data(const unsigned index) const;


		/**
		  * @brief Obtener las labels asociadas a los data
		  *
		  * @return Etiquetas asociadas a los data.
		  */

		std::vector<double> get_all_output_data() const;


		/**
		  * @brief Obtener la etiqueta asociada al dato index
		  *
		  * @param index Indice de la etiqueta a obtener
		  *
		  * @pre index >= 0 && index < output_datos.size
		  *
		  * @return Etiqueta asociada al dato index.
		  */

		double get_output_data(const unsigned index) const ;

		/**
		  * @brief Obtener el mejor individuo de la poblacion.
		  *
		  * @return Mejor individuo de la población
		  */

		T get_best_individual() const;

		/**
		 * @brief Obtener la profundidad máxima de las expresiones
		 *
		 * @return Profundidad máxima de las expresiones
		 */

		unsigned get_expressions_max_depth() const;


		/**
		 *  @brief Aplicar el elitismo a la poblacion actual
		 *
		 * @param mejor_individuo_anterior Mejor individuo con el que comparar la Population actual
		 *
		 *
		 */

		void apply_elitism(const T & mejor_individuo_anterior);

		/**
		 *  @brief Predecir un dato tras entrenar el algoritmo
		 *
		 * @param dato Dato a predict
		 *
		 * @pre Se ha entrenado el algoritmo
		 *
		 * @return Valor predecido por el algoritmo
		 */

		double predict(const std::vector<double> & dato) const ;

		/**
		 *  @brief Predecir un conjunto de data tras entrenar el algoritmo
		 *
		 * @param data Datos a predict
		 *
		 * @pre Se ha entrenado el algoritmo
		 *
		 * @return Valores predecido por el algoritmo
		 */

		std::vector<double> predict(const std::vector<std::vector<double> > & data) const;

		/**
		 *  @brief Ajustar el algoritmo con unos parameters dados
		 *
		 * @param parameters Parameters con los que fit el algoritmo
		 *
		 */

		virtual void fit(const Parameters & parameters) = 0;

		/**
		 *  @brief Ajustar el algoritmo con unos parameters dados utilizando validación simple
		 *
		 * @param datos_train Fichero previamente leido correspondiente al train del cv
		 * @param datos_test  Fichero previamente leido correspondiente a la validacion del cv
		 * @param parameters Parameters con los que fit el algoritmo
		 *
		 * @return Errores obtenido de los folds de validación cruzada
		 */

		std::pair<T, std::vector<std::vector<double> > >  fit_cv_files( std::pair<matriz<double>, std::vector<double> > & datos_train,  std::pair<matriz<double>, std::vector<double> > & datos_test, 
		 														const Parameters & parameters);
		/**
		 *  @brief Ajustar el algoritmo con unos parameters dados utilizando validación cruzada
		 *
		 * @param numero_val_cruzada Número de segmentos en los que dividir el conjunto de data para aplicar validación cruzada
		 * @param parameters Parameters con los que fit el algoritmo
		 *
		 * @return Errores obtenido de los folds de validación cruzada
		 */
		std::pair<T, std::vector<std::vector<double> > > perform_k_cross_validation(const unsigned numero_val_cruzada, const Parameters & parameters);

};



} // namespace expressions_algs


#include "expressions_algs/Population_alg.tpp"


#endif

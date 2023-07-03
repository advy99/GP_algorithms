/**
  * \@file Population.hpp
  * @brief Fichero cabecera de la clase Population
  *
  */

#ifndef POPULATION_H_INCLUDED
#define POPULATION_H_INCLUDED

#include "expressions_algs/Expression.hpp"
#include "expressions_algs/GA_P_Expression.hpp"

namespace expressions_algs {


/**
  *  @brief Clase Population
  *
  *
  *  @tparam T Tipo de dato que utilizará Population
  *
  *  Una instancia del type Population representará un conjunto de individuos
  *  de type T, también podrá indicar cual es el mejor
  *  individuo de la población.
  *
  * @author Antonio David Villegas Yeguas
  * @date Julio 2020
  */

template <class T>
class Population{
	private:

		/**
		  * @page repPopulation Representación de la clase Population
		  *
		  * @section invPopulation Representation invariant
		  *
		  * The invariant is; population_size > 0
		  *
		  * @section faPopulation Abstraction function
		  *
		  * A valid object @e rep of class Population representa la poblacion
		  *
		  * rep.poblacion
		  *
		  * Así como su mejor individuo en
		  *
		  * rep.mejor_individuo
		  *
		  * Podemos saber el tamaño de la poblacion con
		  *
		  * rep.population_size
		  *
		  */

		/**
		  * @brief Expressiones que conforman los individuos de la poblacion.
		  *
		  */

		std::vector<T> expressions_;


		/**
		  * @brief Mejor individuo de la población
		  */
		int mejor_individuo_;


		/**
		  * @brief Copiar data de una poblacion dada a la poblacion.
		  *
		  * @param otra Población de donde copiar los data.
		  *
		  */

		void copy_data(const Population & otra);


		/**
		 * @brief Obtener la suma del fitness de todos los individuos
		 *
		 * @return Sumatoria del fitness de todos los individuos
		 */

		double fitness_sum() const;

	public:

		/**
		  * @brief Constructor con sin parameters, generamos una poblacion vacia.
		  *
		  */

		Population();

		/**
		  * @brief Constructor de copia.
		  *
		  * @param otra Population a copiar
		  *
		  */

		Population(const Population & otra);




		/**
		  * @brief Constructor con dos parámetros. Generar una población
		  * con tam elementos aleatorios.
		  *
		  * @param tam Número de individuos con el que generar la población.
		  * @param lon_expre Longitud máxima de las expresiones a generar
		  * @param prob_var Probabilidad de que un nodo de la expresión sea
		  * una variable.
		  * @param num_vars Número de variables que utilizarán las expresiones
		  * de la población
		  * @param prof_expre Profundidad máxima de las expresiones de
		  * la población
		  *
		  */


		Population(const unsigned tam, const unsigned lon_expre,
					 const double prob_var, const unsigned num_vars,
					 const unsigned prof_expre);

		/**
		  * @brief Destructor de la Población
		  *
		  *
		  */

		~Population();

		/**
		  * @brief Obtener el mejor individuo de la poblacion.
		  *
		  * @return Mejor individuo de la población
		  */

		T get_best_individual() const;

		/**
		  * @brief Obtener el index del mejor individuo de la poblacion.
		  *
		  * @return Indice del mejor individuo de la población
		  */

		unsigned get_best_individual_index() const;

		/**
		  * @brief Obtener el tamaño de la poblacion.
		  *
		  * @return Tamaño de la población
		  */

		unsigned get_population_size() const;

		/**
		  * @brief Obtener el individuo index de la poblacion.
		  *
		  * @param index Posición del individuo en la población.
		  *
		  * @return Individuo en la posicion index de la población.
		  */

		T & operator [] (const unsigned index);

		/**
		  * @brief Obtener el individuo index de la poblacion.
		  *
		  * @param index Posición del individuo en la población.
		  *
		  * @return Individuo en la posicion index de la población.
		  */

		const T & operator [] (const unsigned index) const;


		/**
		  * @brief Modificar el individuo en la posición index de la poblacion
		  * por another individuo.
		  *
		  * @param index Posición del individuo a sustituir dentro de
		  * la población.
		  * @param n_individuo Nuevo individuo a introducir en la población.
		  *
		  */

		void set_individual(const unsigned index, const T & n_individuo);

		/**
		  * @brief Evaluar todos los elementos de la población.
		  *
		  * @param data Datos con los que se evaluará la población
		  * @param labels Valores correspondientes a los data dados
		  * @param funcion_evaluacion Funcion de evaluación a utilizar
		  *
		  * @pre data.size == labels.size
		  *
		  * @post El mejor individuo de la población se vera actualizado.
		  *
		  */

		void evaluate_population(const std::vector<std::vector<double> > & data,
									 const std::vector<double> & labels,
								 	 aux::eval_function_t funcion_evaluacion);

		/**
		 * @brief Seleccionar un individuo de la población
		 *
		 * @return Indice del individuo
		 */

		unsigned select_individual() const;


		/**
		  * @brief Operador de asignación de una poblacion. Asignamos una
		  * poblacion a otra.
		  *
		  * @param otra Poblacion a copiar.
		  *
		  * @return Referencia a la poblacion.
		  */

		Population & operator= (const Population & otra);

		/**
		  * @brief Cambiar el tamaño de la Population por el dado.
		  *
		  * @param nuevo_tam Nuevo tamaño de la poblacion.
		  *
		  */

		void resize(const unsigned nuevo_tam);

		/**
		  * @brief Insertar un elemento en la Population.
		  *
		  * @param nuevo_elemento Elemento a insert.
		  *
		  */


		void insert(const T & nuevo_elemento);

		/**
		  * @brief Eliminar un elemento en la Population.
		  *
		  * @param posicion Posicion a remove.
		  *
		  */

		void remove(const unsigned posicion);

		/**
		  * @brief Ordenar la Population de expresiones por fitness.
		  *
		  */

		void ordenar();

		/**
		 * @brief Cambiar el individuo que se considera mejor
		 *
		 * @param nuevo_mejor Indice del nuevo mejor individuo
		 *
		 */

		void set_best_individual(const int nuevo_mejor);


		/**
		 * @brief Buscar el mejor individuo en la población
		 *
		 *
		 */
		void search_best_individual();

};

} // namespace expressions_algs

#include "expressions_algs/Population.tpp"



#endif

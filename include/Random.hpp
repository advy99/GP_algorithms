/**
  * \@file random.hpp
  * @brief Fichero cabecera de la clase Random
  *
  */

#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

/**
  *  @brief Clase Random
  *
  * Una instancia del type Random será un generador modular de números aleatorios.
  *
  *
  * @author Antonio David Villegas Yeguas
  * @date Julio 2020
  */

#include <random>


class Random{
	private:

		/**
		  * @page repRandom Representación de la clase Random
		  *
		  * @section invRandom Representation invariant
		  *
		  * The invariant is; SEED ha de estar inicializado
		  *
		  *
		  */

		static std::mt19937 generator_;

		/**
		  * @brief Constructor sin parámetros que iniciliza la semilla a un value aleatorio
		  */

		Random() = delete;



	public:

		/**
		 * @brief Inicializar la semilla
		 *
		 * @param seed Nueva semilla para el generador
		 */

		static void set_seed(const unsigned long seed);



		/**
		  * @brief Destructor de la clase Random
		  *
		  */

		~Random();

		/**
		  * @brief Constructor de copia. ELIMINADO AL SER UNA CLASE SINGLETON
		  *
		  */


		Random(const Random & another) = delete;

		/**
		  * @brief Operador =. ELIMINADO AL SER UNA CLASE SINGLETON
		  *
		  */
		Random & operator = (const Random & another) = delete;


		/**
		  * @brief Generar un value real aleatorio entre [0, 1[
		  *
		  * @return Número aleatorio entre [0, 1[
		  */

		static float get_float();


		/**
		  * @brief Generar un value real aleatorio entre dos valores dados
		  *
		  * @param LOW Límite inferior para el value a generar
		  * @param HIGH Límite superior para el value a generar
		  *
		  * @return Número real aleatorio entre [LOW, HIGH[
		  */

		static float get_float(const float LOW, const float HIGH);

		/**
		  * @brief Generar un value real aleatorio entre 0 y un value dado
		  *
		  * @param HIGH Límite superior para el value a generar
		  *
		  * @return Número real aleatorio entre [0, HIGH[
		  */

		static float get_float(const float HIGH);


		/**
		  * @brief Generar un value entero aleatorio entre dos valores dados
		  *
		  * @param LOW Límite inferior para el value a generar
		  * @param HIGH Límite superior para el value a generar
		  *
		  * @return Número entero aleatorio entre [LOW, HIGH[
		  */

		static int get_int(const int LOW, const int HIGH);

		/**
		  * @brief Generar un value entero aleatorio entre 0 y un value dado
		  *
		  * @param HIGH Límite superior para el value a generar
		  *
		  * @return Número entero aleatorio entre [0, HIGH[
		  */

		static int get_int(const int HIGH);


		/**
		  * @brief Obtener el generador de aleatorios que se está utilizando
		  *
		  *
		  * @return Generador de aleatorios que se está utilizando
		  */

		static std::mt19937 get_generator();

};

#endif

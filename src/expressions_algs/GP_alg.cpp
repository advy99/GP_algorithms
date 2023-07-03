#include "expressions_algs/GP_alg.hpp"

namespace expressions_algs {



GP_alg :: GP_alg(const std::vector<std::vector<double> > & data, const std::vector<double> & labels,
									const unsigned long seed, const unsigned population_size, const unsigned depth, const double prob_var){
	initialize_empty();
	load_data(data, labels);
	initialize(seed, population_size, depth, prob_var);

}



GP_alg :: GP_alg(const std::string data_file, const char comment_char,
		  const unsigned population_size, const double prob_var,
		  const unsigned long seed,
		  const char delimiter, const unsigned depth){

	initialize_empty();
	read_data(data_file, comment_char, delimiter);

	initialize(seed, population_size, depth, prob_var);

}


GP_alg :: ~GP_alg() {

}


void GP_alg :: fit(const Parameters & parameters) {

	const int NUM_GENERACIONES = parameters.get_num_evaluations() / static_cast<double>(population_.get_population_size());

	int generation = 0;
	int parent, mom;
	bool modificado_hijo1;
	bool modificado_hijo2;

	// evaluo la poblacion al inicio
	population_.evaluate_population(data_, output_data_, parameters.get_evaluation_functions());

	Expression mejor_individuo = population_.get_best_individual();

	Expression son1, son2;

	while ( generation < NUM_GENERACIONES) {

		// seleccionamos la poblacion a cruzar
		population_ = tournament_selection(parameters.get_tournament_size());

		// aplicamos los operadores geneticos
		for ( unsigned i = 0; i < population_.get_population_size(); i += 2){

			mom = i;
			parent = i + 1;

			son1 = population_[mom];
			son2 = population_[parent];

			modificado_hijo1 = modificado_hijo2 = false;

			// cruce de la parte GP
			if ( Random::get_float() < parameters.get_pg_crossover_probability() ) {
				// cruce de programacion genetica, se intercambian arboles

				population_[mom].tree_crossover(population_[parent], son1, son2);
				modificado_hijo1 = modificado_hijo2 = true;
			}

			// si no hay cruce, los hijos ya estaban con el value de los parents

			auto resultado_mut_gp = apply_GP_mutations(son1, son2, parameters.get_pg_mutation_probability());

			modificado_hijo1 = modificado_hijo1 || resultado_mut_gp.first;
			modificado_hijo2 = modificado_hijo2 || resultado_mut_gp.second;

			if ( modificado_hijo1 ) {
				population_[mom] = son1;
				population_[mom].no_longer_evaluated();
			}

			if ( modificado_hijo2) {
				population_[parent] = son2;
				population_[parent].no_longer_evaluated();
			}

		}


		apply_elitism(mejor_individuo);


		// evaluamos
		population_.evaluate_population(data_, output_data_, parameters.get_evaluation_functions());

		mejor_individuo = population_.get_best_individual();

		if ( parameters.get_show_evaluation() ) {
			// mostramos el mejor individuo
			std::cout << generation << "\t" << mejor_individuo.get_fitness() << std::endl;
		}

		generation++;
	}

}


}

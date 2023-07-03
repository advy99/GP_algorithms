#include "expressions_algs/GA_P_alg.hpp"

namespace expressions_algs {


GA_P_alg :: GA_P_alg(const std::vector<std::vector<double> > & data, const std::vector<double> & labels,
										 const unsigned long seed, const unsigned population_size, const unsigned depth, const double prob_var) {
	initialize_empty();
	load_data(data, labels);
	initialize(seed, population_size, depth, prob_var);

}



GA_P_alg :: GA_P_alg(const std::string data_file, const char comment_char,
			  const unsigned population_size, const double prob_var,
			  const unsigned long seed, const char delimiter, const unsigned depth){


	// lemos los data del fichero de entrada
	initialize_empty();
	read_data(data_file, comment_char, delimiter);

	initialize(seed, population_size, depth, prob_var);

}

void GA_P_alg :: fit(const Parameters & parameters) {


	// en cada generation hacemos dos evaluaciones, así que si queremos cierto
	// numero de evaluaciones, las generaciones serán la mitad
	const int NUM_GENERACIONES = parameters.get_num_evaluations() / static_cast<double>(population_.get_population_size());

	int generation = 0;
	int parent, mom;
	bool modificado_hijo1;
	bool modificado_hijo2;

	// evaluo la poblacion al inicio
	population_.evaluate_population(data_, output_data_, parameters.get_evaluation_functions());
	// population_.ordenar();



	GA_P_Expression mejor_individuo = population_.get_best_individual();

	GA_P_Expression son1, son2;

	bool cruce_intra_nicho_posible = true;

	while ( generation < NUM_GENERACIONES) {

		// seleccionamos la poblacion
		population_ = tournament_selection(parameters.get_tournament_size());

		std::vector<bool> cruzados;
		cruzados.resize(population_.get_population_size(), false);
		int primero_sin_cruzar = 0;

		for ( unsigned i = 0; i < population_.get_population_size(); i += 2){

			mom = primero_sin_cruzar;
			do {
				++primero_sin_cruzar;
			} while (cruzados[primero_sin_cruzar]);
			cruzados[mom] = true;

			cruce_intra_nicho_posible = false;
			modificado_hijo1 = modificado_hijo2 = false;

			if ( parameters.get_ga_inter_niche_crossover_probability() < Random::get_float() ){
				// cruce intra-nicho

				parent = inter_niche_selection(mom, cruzados);

				cruce_intra_nicho_posible = parent != -1;

				// aplicamos el cruce
				if ( cruce_intra_nicho_posible) {
					// ya he escogido a ese parent
					cruzados[parent] = true;
					if (parent == primero_sin_cruzar) {
						do {
							++primero_sin_cruzar;
						} while (cruzados[primero_sin_cruzar]);

					}

					son1 = population_[mom];
					son2 = population_[parent];

					// cruce de la parte GA
					if ( Random::get_float() < parameters.get_ga_crossover_probability() ) {
						// cruce del cromosoma utilizando BLX_alfa
						population_[mom].blx_alpha_crossover(population_[parent], son1, son2);
						modificado_hijo1 = modificado_hijo2 = true;
					}


					if ( Random::get_float() < parameters.get_ga_mutation_probability() ) {
						// mutacion GP en el primer hijo
						son1.mutate_ga(generation, NUM_GENERACIONES);
						modificado_hijo1 = true;
					}

					if ( Random::get_float() < parameters.get_ga_mutation_probability() ) {
						// mutacion GP en el segundo hijo
						son2.mutate_ga(generation, NUM_GENERACIONES);
						modificado_hijo2 = true;
					}

				}

			}

			// si se escoge hacer cruce inter-nicho, o si no es posible aplicarlo
			if ( !cruce_intra_nicho_posible ) {
				// cruce inter-nicho
				parent = primero_sin_cruzar;
				do {
					++primero_sin_cruzar;
				} while (cruzados[primero_sin_cruzar]);
				cruzados[parent] = true;

				son1 = population_[mom];
				son2 = population_[parent];

				// cruce de la parte GP
				if ( Random::get_float() <  parameters.get_pg_crossover_probability() ) {
					// cruce de programacion genetica, se intercambian arboles

					population_[mom].tree_crossover(population_[parent], son1, son2);
					modificado_hijo1 = modificado_hijo2 = true;
				}

				// cruce de la parte GA
				if ( Random::get_float() < parameters.get_ga_crossover_probability() ) {
					// cruce del cromosoma utilizando BLX_alfa
					population_[mom].blx_alpha_crossover(population_[parent], son1, son2);
					modificado_hijo1 = modificado_hijo2 = true;
				}

				// si no hay cruce, los hijos ya estaban con el value de los parents
				if ( Random::get_float() < parameters.get_ga_mutation_probability() ) {
					// mutacion GP en el primer hijo
					son1.mutate_ga(generation, NUM_GENERACIONES);
					modificado_hijo1 = true;
				}

				if ( Random::get_float() < parameters.get_ga_mutation_probability() ) {
					// mutacion GP en el segundo hijo
					son2.mutate_ga(generation, NUM_GENERACIONES);
					modificado_hijo2 = true;
				}


				auto resultado_mut_gp = apply_GP_mutations(son1, son2, parameters.get_pg_mutation_probability());

				modificado_hijo1 = modificado_hijo1 || resultado_mut_gp.first;
				modificado_hijo2 = modificado_hijo2 || resultado_mut_gp.second;

			}


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
		population_.evaluate_population(data_, output_data_, parameters.get_evaluation_functions());
		population_.ordenar();

		mejor_individuo = population_[0];

		if ( parameters.get_show_evaluation() ) {
			// mostramos el mejor individuo
			std::cout << generation << "\t" << mejor_individuo.get_fitness() << std::endl;
		}

		generation++;

	}

}

int GA_P_alg :: inter_niche_selection(const int mom, const std::vector<bool> & escogidos) const{

	int parent = -1;


	unsigned i = 0;

	do {

		parent = i;

		// si no estan en el mismo nicho, o si ya lo hemos escogido antes
		if ( !population_[mom].same_niche(population_[parent]) || escogidos[parent] ) {
			parent = -1;
		}

		i++;

	} while ( parent == -1 && i < escogidos.size() );

	return parent;
}


} // namespace expressions_algs

#include <iostream>
#include "expressions_algs/GA_P_alg.hpp"
#include "expressions_algs/GP_alg.hpp"

#include <ctime>
#include "Random.hpp"
#include <chrono>

#ifdef _OPENMP
	#include <omp.h>
#endif


int main(int argc, char ** argv){

	if ( argc < 13 || argc > 15 ) {
		std::cerr << "Error en el número de parámetros\n"
					 << "\t Uso: " << argv[0] << " <data_file> <population_size> <prob_variable> <profundidad_max> \n"
					 << "\t\t\t" << " <num_evaluaciones> <prob_cruce_gp> <prob_cruce_ga> <prob_mutacion_gp> <prob_mutacion_ga> <prob_cruce_intranicho> <tam_torneo> [num_trabajos] [semilla] "
					 << std::endl;
		exit(-1);
	}

	int semilla;

	if ( argc == 15 ){
		semilla = atoi(argv[14]);
	} else {
		semilla = std::time(nullptr);
	}

	int tam_pob = atoi(argv[3]);
	double prob_variable = atof(argv[4]);
	int prof_max_expr = atoi(argv[5]);
	int evaluaciones = atoi(argv[6]);
	double prob_cruce_gp = atof(argv[7]);
	double prob_cruce_ga = atof(argv[8]);
	double prob_muta_gp = atof(argv[9]);
	double prob_muta_ga = atof(argv[10]);
	double prob_cruce_intra = atof(argv[11]);
	int tam_torneo = atoi(argv[12]);

	int num_trabajos = atoi(argv[13]);

	expressions_algs::Parameters parametros_ejecucion(evaluaciones, expressions_algs::aux::cuadratic_mean_error, prob_cruce_gp,
		 																	  prob_cruce_ga, prob_muta_gp,
																			  prob_muta_ga, prob_cruce_intra,
																			  tam_torneo, false);

	parametros_ejecucion.add_error_function(expressions_algs::aux::root_cuadratic_mean_error);
	parametros_ejecucion.add_error_function(expressions_algs::aux::mean_absolute_error);

	// si utilizamos openMP, establecemos el número de trabajos
	#ifdef _OPENMP
		omp_set_num_threads(num_trabajos);
	#endif

	int semilla_original = semilla;
	Random::set_seed(semilla);
	auto data = expressions_algs::preprocess::read_data<double>(std::string(argv[1]), '@', ',');
	data = expressions_algs::preprocess::random_data_reorder(data.first, data.second);
	auto train_test_split = expressions_algs::preprocess::split_train_test(data.first, data.second);

	expressions_algs::GA_P_alg myGAP (train_test_split.first.first, train_test_split.first.second, semilla, tam_pob, prof_max_expr, prob_variable);


	double error_medio_ecm_gap = 0.0;
	double error_medio_recm_gap = 0.0;
	double error_medio_mae_gap = 0.0;


	std::vector<double> error_it;
	expressions_algs::GA_P_Expression mejor_expresion_gap;

	// ajustamos GAP midiendo tiempo
	auto tiempo_inicio = std::chrono::high_resolution_clock::now();

	const unsigned num_cv = 2;
	const unsigned num_it = 5;

	// hacemos 5x2 cv
	for (unsigned i = 0; i < num_it; i++) {
		auto resultado_cv = myGAP.perform_k_cross_validation(num_cv, parametros_ejecucion);

		if ( resultado_cv.first.get_fitness() < mejor_expresion_gap.get_fitness()) {
			mejor_expresion_gap = resultado_cv.first;
		}

		for ( unsigned j = 0; j < num_cv; j++) {
			error_medio_ecm_gap += resultado_cv.second[0][j];
			error_medio_recm_gap += resultado_cv.second[1][j];
			error_medio_mae_gap += resultado_cv.second[2][j];
		}

	}

	error_medio_ecm_gap /= num_cv * num_it * 1.0;
	error_medio_recm_gap /= num_cv * num_it * 1.0;
	error_medio_mae_gap /= num_cv * num_it * 1.0;

	auto tiempo_fin = std::chrono::high_resolution_clock::now();


	std::chrono::duration<double> t_ejecucion = std::chrono::duration_cast<std::chrono::microseconds>(tiempo_fin - tiempo_inicio);

	// mostramos el resultado
	std::cout << semilla_original << "\t"
				 << error_medio_ecm_gap << "\t"
				 << error_medio_recm_gap << "\t"
				 << error_medio_mae_gap << "\t"
				 << mejor_expresion_gap << "\t"
				 << t_ejecucion.count() << "\t GAP" << std::endl;

	
	tiempo_inicio = std::chrono::high_resolution_clock::now();
	//Evaluamos la mejor expresion sobre el conjunto de validacion final 
	mejor_expresion_gap.evaluate_expression(train_test_split.second.first, train_test_split.second.second, expressions_algs::aux::cuadratic_mean_error, true);
	double error_val_gap_ecm = mejor_expresion_gap.get_fitness();

	mejor_expresion_gap.evaluate_expression(train_test_split.second.first, train_test_split.second.second, expressions_algs::aux::root_cuadratic_mean_error, true);
	double error_val_gap_recm = mejor_expresion_gap.get_fitness();

	mejor_expresion_gap.evaluate_expression(train_test_split.second.first, train_test_split.second.second, expressions_algs::aux::mean_absolute_error, true);
	double error_val_gap_mae = mejor_expresion_gap.get_fitness();
	
	tiempo_fin = std::chrono::high_resolution_clock::now();
	t_ejecucion = std::chrono::duration_cast<std::chrono::microseconds>(tiempo_fin - tiempo_inicio);

	std::cout << semilla_original << "\t"
				 << error_val_gap_ecm << "\t"
				 << error_val_gap_recm << "\t"
				 << error_val_gap_mae << "\t"
				 << mejor_expresion_gap << "\t"
				 << t_ejecucion.count() << "\t GAP VAL" << std::endl;

	// ahora con PG
	Random::set_seed(semilla_original);
	// hacemos lo mismo pero con PG
	expressions_algs::GP_alg myPG (train_test_split.first.first, train_test_split.first.second, semilla, tam_pob, prof_max_expr, prob_variable);

	double error_medio_ecm_gp = 0.0;
	double error_medio_recm_gp = 0.0;
	double error_medio_mae_gp = 0.0;

	expressions_algs::Expression mejor_expresion_pg;


	tiempo_inicio = std::chrono::high_resolution_clock::now();

	// hacemos 5x2 cv
	for (unsigned i = 0; i < num_it; i++) {
		auto resultado_cv = myPG.perform_k_cross_validation(num_cv, parametros_ejecucion);


		if ( resultado_cv.first.get_fitness() < mejor_expresion_pg.get_fitness()) {
			mejor_expresion_pg = resultado_cv.first;
		}

		for ( unsigned j = 0; j < num_cv; j++) {
			error_medio_ecm_gp += resultado_cv.second[0][j];
			error_medio_recm_gp += resultado_cv.second[1][j];
			error_medio_mae_gp += resultado_cv.second[2][j];
		}
	}

	error_medio_ecm_gp /= num_cv * num_it * 1.0;
	error_medio_recm_gp /= num_cv * num_it * 1.0;
	error_medio_mae_gp /= num_cv * num_it * 1.0;

	tiempo_fin = std::chrono::high_resolution_clock::now();

	t_ejecucion = std::chrono::duration_cast<std::chrono::microseconds>(tiempo_fin - tiempo_inicio);

	std::cout << semilla_original << "\t"
				 << error_medio_ecm_gp << "\t"
				 << error_medio_recm_gp << "\t"
				 << error_medio_mae_gp << "\t"
				 << mejor_expresion_pg << "\t"
				 << t_ejecucion.count() << "\t PG" << std::endl;


	tiempo_inicio = std::chrono::high_resolution_clock::now();
	//Y sobre pg
	mejor_expresion_pg.evaluate_expression(train_test_split.second.first, train_test_split.second.second, expressions_algs::aux::cuadratic_mean_error, true);
	double error_val_pg_ecm = mejor_expresion_pg.get_fitness();

	mejor_expresion_pg.evaluate_expression(train_test_split.second.first, train_test_split.second.second, expressions_algs::aux::root_cuadratic_mean_error, true);
	double error_val_pg_recm = mejor_expresion_pg.get_fitness();

	mejor_expresion_pg.evaluate_expression(train_test_split.second.first, train_test_split.second.second, expressions_algs::aux::mean_absolute_error, true);
	double error_val_pg_mae = mejor_expresion_pg.get_fitness();
	
	tiempo_fin = std::chrono::high_resolution_clock::now();
	t_ejecucion = std::chrono::duration_cast<std::chrono::microseconds>(tiempo_fin - tiempo_inicio);

	std::cout << semilla_original << "\t"
				 << error_val_pg_ecm << "\t"
				 << error_val_pg_recm << "\t"
				 << error_val_pg_mae << "\t"
				 << mejor_expresion_pg<< "\t"
				 << t_ejecucion.count() << "\t PG VAL" << std::endl;


	return 0;

}

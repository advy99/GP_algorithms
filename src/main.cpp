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

	if ( argc < 13 || argc > 16 ) {
		std::cerr << "ERROR: Wrong number of params\n"
					 << "\t Use: " << argv[0] << " <train_data_file> <test_data_file> <val_data_file> <population_size> <variable_prob> <max_depth> \n"
					 << "\t\t\t" << " <num_evaluations> <prob_pg_crossover> <prob_ga_crossover> <prob_gp_mutation> <prob_ga_mutation> <prob_inter_niche_crossover> <tournament_size> [num_jobs] [seed] "
					 << std::endl;
		exit(-1);
	}

	int seed;

	if ( argc == 16 ){
		seed = atoi(argv[15]);
	} else {
		seed = std::time(nullptr);
	} 

	int population_size = atoi(argv[4]);
	double prob_variable = atof(argv[5]);
	int max_expression_depth = atoi(argv[6]);
	int evaluaciones = atoi(argv[7]);
	double gp_crossover_prob = atof(argv[8]);
	double ga_crossover_prob = atof(argv[9]);
	double gp_mutation_prob = atof(argv[10]);
	double ga_mutation_prob = atof(argv[11]);
	double inter_niche_crossover_prob = atof(argv[12]);
	int tournament_size = atoi(argv[13]);

	int num_jobs = atoi(argv[14]);

	expressions_algs::Parameters execution_params(evaluaciones, expressions_algs::aux::cuadratic_mean_error, gp_crossover_prob,
		 																	  ga_crossover_prob, gp_mutation_prob,
																			  ga_mutation_prob, inter_niche_crossover_prob,
																			  tournament_size, false);

	execution_params.add_error_function(expressions_algs::aux::root_cuadratic_mean_error);
	execution_params.add_error_function(expressions_algs::aux::mean_absolute_error);

	// si utilizamos openMP, establecemos el número de trabajos
	#ifdef _OPENMP
		omp_set_num_threads(num_jobs);
	#endif

	int original_seed = seed;
	Random::set_seed(seed);


	double mean_error_ecm_gap = 0.0;
	double mean_error_recm_gap = 0.0;
	double mean_error_mae_gap = 0.0;

 
	expressions_algs::GA_P_Expression best_gap_expression;
	auto best_error=100000.0;

	// ajustamos GAP midiendo tiempo
	auto start_time = std::chrono::high_resolution_clock::now();

	const unsigned num_cv = 1;
	const unsigned num_it = 5; 
 

	// No hacemos 5x2 cv, solo 5-fold cv 
	for (unsigned i = 0; i < num_it; i++) {
		auto start_time_it = std::chrono::high_resolution_clock::now();

		std::string t =std::to_string(i);
		std::string train_file=std::string(argv[1]);
		std::string::size_type n = 0;
		const std::string s = "0";
 		n = train_file.find( s, n );
		if(n!= std::string::npos)				train_file.replace( n, s.size(), t );
		
		std::string test_file=std::string(argv[2]); 
 		n = test_file.find( s, n );
		if(n!= std::string::npos)				test_file.replace( n, s.size(), t );
		
		auto data = expressions_algs::preprocess::read_data<double>(train_file, '@', ',');
		auto data_test = expressions_algs::preprocess::read_data<double>(test_file, '@', ',');

		expressions_algs::GA_P_alg myGAP (data.first, data.second, seed, population_size, max_expression_depth, prob_variable);
		
		auto cv_result=myGAP.fit_cv_files(data,data_test,execution_params);



		auto end_time = std::chrono::high_resolution_clock::now();


		std::chrono::duration<double> execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time_it);

		std::cout << original_seed << "\t"
					<< cv_result.second[0][0] << "\t"
					<< cv_result.second[1][0] << "\t"
					<< cv_result.second[2][0] << "\t"
					<< cv_result.first << "\t"
					<< execution_time.count() << "\t GAP" << "\t" << "cv: " << i << std::endl;


		//if ( cv_result.first.get_fitness() < best_gap_expression.get_fitness()) {
		//	best_gap_expression = cv_result.first;
		//}
		if(cv_result.second[0][0] < best_error){
			best_gap_expression = cv_result.first;
			best_error=cv_result.second[0][0];
		}

		for ( unsigned j = 0; j < num_cv; j++) {
			mean_error_ecm_gap += cv_result.second[0][0];
			mean_error_recm_gap += cv_result.second[1][0];
			mean_error_mae_gap += cv_result.second[2][0];
			}
	}

	mean_error_ecm_gap /= num_cv * num_it * 1.0;
	mean_error_recm_gap /= num_cv * num_it * 1.0;
	mean_error_mae_gap /= num_cv * num_it * 1.0;

	auto end_time = std::chrono::high_resolution_clock::now();


	std::chrono::duration<double> execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	std::cout << original_seed << "\t"
				 << mean_error_ecm_gap << "\t"
				 << mean_error_recm_gap << "\t"
				 << mean_error_mae_gap << "\t"
				 << best_gap_expression << "\t"
				 << execution_time.count() << "\t GAP AVG" << std::endl;


	std::string fichero_ValidacionFinal=std::string(argv[3]);
	auto datos_val = expressions_algs::preprocess::read_data<double>(fichero_ValidacionFinal, '@', ',');
	
	start_time = std::chrono::high_resolution_clock::now();
	// evaluate the best expression over the final validation data
	best_gap_expression.evaluate_expression(datos_val.first, datos_val.second, expressions_algs::aux::cuadratic_mean_error, true);
	double error_val_gap_ecm = best_gap_expression.get_fitness();

	best_gap_expression.evaluate_expression(datos_val.first, datos_val.second, expressions_algs::aux::root_cuadratic_mean_error, true);
	double error_val_gap_recm = best_gap_expression.get_fitness();

	best_gap_expression.evaluate_expression(datos_val.first, datos_val.second, expressions_algs::aux::mean_absolute_error, true);
	double error_val_gap_mae = best_gap_expression.get_fitness();
	
	end_time = std::chrono::high_resolution_clock::now();
	execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	std::cout << original_seed << "\t"
				 << error_val_gap_ecm << "\t"
				 << error_val_gap_recm << "\t"
				 << error_val_gap_mae << "\t"
				 << best_gap_expression << "\t"
				 << execution_time.count() << "\t GAP VAL" << std::endl;

	// now repeat with GP
	Random::set_seed(original_seed);

	double mean_error_ecm_gp = 0.0;
	double mean_error_recm_gp = 0.0;
	double mean_error_mae_gp = 0.0;

	expressions_algs::Expression best_pg_expression;


	start_time = std::chrono::high_resolution_clock::now();
	best_error=100000.0;
	for (unsigned i = 0; i < num_it; i++) {

		auto start_time_it = std::chrono::high_resolution_clock::now();
		std::string t =std::to_string(i);
		std::string train_file=std::string(argv[1]);
		std::string::size_type n = 0;
		const std::string s = "0";
 		n = train_file.find( s, n );
		if(n!= std::string::npos)				train_file.replace( n, s.size(), t );
		std::string test_file=std::string(argv[2]); 
 		n = test_file.find( s, n );

		if(n!= std::string::npos)				test_file.replace( n, s.size(), t );
		
		auto data = expressions_algs::preprocess::read_data<double>(train_file, '@', ',');

		auto data_test = expressions_algs::preprocess::read_data<double>(test_file, '@', ',');
	

		expressions_algs::GP_alg myPG (data.first, data.second, seed, population_size, max_expression_depth, prob_variable);
		
		auto cv_result=myPG.fit_cv_files(data,data_test,execution_params);


	
		if(cv_result.second[0][0] < best_error){
			best_pg_expression = cv_result.first;
			best_error=cv_result.second[0][0];
		}
		for ( unsigned j = 0; j < num_cv; j++) {
			mean_error_ecm_gp += cv_result.second[0][0];
			mean_error_recm_gp += cv_result.second[1][0];
			mean_error_mae_gp += cv_result.second[2][0];
		}

		auto end_time = std::chrono::high_resolution_clock::now();


		std::chrono::duration<double> execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time_it);

		std::cout << original_seed << "\t"
					<< cv_result.second[0][0] << "\t"
					<< cv_result.second[1][0] << "\t"
					<< cv_result.second[2][0] << "\t"
					<< cv_result.first << "\t"
					<< execution_time.count() << "\t GP" << "\t" << "cv: " << i << std::endl;

	}


	mean_error_ecm_gp /= num_cv * num_it * 1.0;
	mean_error_recm_gp /= num_cv * num_it * 1.0;
	mean_error_mae_gp /= num_cv * num_it * 1.0;

	end_time = std::chrono::high_resolution_clock::now();
 	
	execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	std::cout << original_seed << "\t"
				 << mean_error_ecm_gp << "\t"
				 << mean_error_recm_gp << "\t"
				 << mean_error_mae_gp << "\t"
				 << best_pg_expression << "\t"
				 << execution_time.count() << "\t GP AVG" << std::endl;


	start_time = std::chrono::high_resolution_clock::now();
	//Y sobre pg
	best_pg_expression.evaluate_expression(datos_val.first, datos_val.second, expressions_algs::aux::cuadratic_mean_error, true);
	double error_val_pg_ecm = best_pg_expression.get_fitness();

	best_pg_expression.evaluate_expression(datos_val.first, datos_val.second, expressions_algs::aux::root_cuadratic_mean_error, true);
	double error_val_pg_recm = best_pg_expression.get_fitness();

	best_pg_expression.evaluate_expression(datos_val.first, datos_val.second, expressions_algs::aux::mean_absolute_error, true);
	double error_val_pg_mae = best_pg_expression.get_fitness();
	
	end_time = std::chrono::high_resolution_clock::now();
	execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	std::cout << original_seed << "\t"
				 << error_val_pg_ecm << "\t"
				 << error_val_pg_recm << "\t"
				 << error_val_pg_mae << "\t"
				 << best_pg_expression<< "\t"
				 << execution_time.count() << "\t GP VAL" << std::endl;


	return 0;

}

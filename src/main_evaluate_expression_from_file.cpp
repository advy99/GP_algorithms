#include <iostream>
#include "expressions_algs/GA_P_alg.hpp"
#include "expressions_algs/GP_alg.hpp"

#include <ctime>
#include "Random.hpp"
#include <chrono>
#include <ranges>
#ifdef _OPENMP
	#include <omp.h>
#endif


int main(int argc, char ** argv){

	if ( argc < 5 || argc > 6 ) {
		std::cerr << "ERROR: Bad numbers of parameters\n"
					 << "\t Use: " << argv[0] << " <data_file> <expression_file> <max_depth> <num_variables> [seed] \n"
					 << std::endl;
		exit(-1);
	}

	std::string file_exp = std::string(argv[2]);
	unsigned max_depth = atoi(argv[3]);
	int num_vars = atoi(argv[4]);
	int seed;

	if ( argc == 6 ){
		seed = atoi(argv[5]);
	} else {
		seed = std::time(nullptr);
	}


	int num_trabajos = 2;
	// si utilizamos openMP, establecemos el número de trabajos
	#ifdef _OPENMP
		omp_set_num_threads(num_trabajos);
	#endif

	int original_seed = seed;
	Random::set_seed(seed);
	auto data = expressions_algs::preprocess::read_data<double>(std::string(argv[1]), '@', ',');
	//data = expressions_algs::preprocess::random_data_reorder(data.first, data.second);
	//auto train_test_split = expressions_algs::preprocess::split_train_test(data.first, data.second);	
 
	expressions_algs::Expression expression(file_exp, max_depth, num_vars);

	expression.evaluate_expression(data.first, data.second, expressions_algs::aux::cuadratic_mean_error, true);
	double error_medio_ecm = expression.get_fitness();

	expression.evaluate_expression(data.first, data.second, expressions_algs::aux::root_cuadratic_mean_error, true);
	double error_medio_recm = expression.get_fitness();

	expression.evaluate_expression(data.first, data.second, expressions_algs::aux::mean_absolute_error, true);
	double error_medio_mae = expression.get_fitness();

	// mostramos el resultado
	std::cout << original_seed << "\t"
				 << error_medio_ecm << "\t"
				 << error_medio_recm << "\t"
				 << error_medio_mae << std::endl;

	return 0;

}

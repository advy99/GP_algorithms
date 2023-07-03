#include "expressions_algs/Parameters.hpp"


namespace expressions_algs {


Parameters :: Parameters(const int N_EVALS,
								 aux::eval_function_t f_eval,
								  const double PROB_CROSSOVER_GP,
								  const double PROB_CROSSOVER_GA,
								  const double PROB_MUTA_GP,
								  const double PROB_MUTA_GA,
								  const double INTERNICHE_CROSSOVER_PROB,
								  const int TOURNAMENT_SIZE,
								  const bool SHOW_EVOLUTION)
	:num_evaluations_(N_EVALS), gp_crossover_probability_(PROB_CROSSOVER_GP),
	 ga_crossover_probability_(PROB_CROSSOVER_GA), gp_mutation_probability_(PROB_MUTA_GP),
	 ga_mutation_probability_(PROB_MUTA_GA), probabilidad_cruce_intranicho_(INTERNICHE_CROSSOVER_PROB),
	 tournament_size_(TOURNAMENT_SIZE), show_evolution_(SHOW_EVOLUTION), eval_f_(f_eval)
	  {}



Parameters :: Parameters(const int N_EVALS,
								 aux::eval_function_t f_eval,
								  const double PROB_CROSSOVER_GP,
								  const double PROB_MUTA_GP,
								  const int TOURNAMENT_SIZE,
								  const bool SHOW_EVOLUTION)
	:Parameters(N_EVALS, f_eval, PROB_CROSSOVER_GP, -1, PROB_MUTA_GP, -1, TOURNAMENT_SIZE, SHOW_EVOLUTION)
	  {}



int Parameters :: get_num_evaluations() const {
	return num_evaluations_;
}
double Parameters :: get_pg_crossover_probability() const {
	return gp_crossover_probability_;
}
double Parameters :: get_ga_crossover_probability() const {
	return ga_crossover_probability_;
}

double Parameters :: get_pg_mutation_probability() const {
	return gp_mutation_probability_;
}

double Parameters :: get_ga_mutation_probability() const {
	return ga_mutation_probability_;
}

double Parameters :: get_ga_inter_niche_crossover_probability() const {
	return probabilidad_cruce_intranicho_;
}

int Parameters :: get_tournament_size() const {
	return tournament_size_;
}

bool Parameters :: get_show_evaluation() const {
	return show_evolution_;
}

aux::eval_function_t Parameters :: get_evaluation_functions() const {
	return eval_f_;
}

void Parameters :: add_error_function(const aux::eval_function_t & func) {
	eval_error_functions_.push_back(func);
}

unsigned Parameters :: get_num_evaluation_functions() const {
	return eval_error_functions_.size();
}

aux::eval_function_t Parameters :: get_error_function(const unsigned i) const {
	return eval_error_functions_[i];
}

}

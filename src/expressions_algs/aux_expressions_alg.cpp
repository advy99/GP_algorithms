#include "expressions_algs/aux_expressions_alg.hpp"

namespace expressions_algs :: aux {

bool compare_floats(const double a, const double b, const double epsilon) {
	return fabs(a - b) < epsilon;
}

double cuadratic_mean_error(const std::vector<double> & predicted_values,
										const std::vector<double> & real_values) {

	double result = 0.0;

	for ( unsigned i = 0; i < real_values.size(); i++ ) {
		result += std::pow( predicted_values[i] - real_values[i] , 2.0);
	}

	result = result / static_cast<double>(real_values.size());

	return result;

}

double root_cuadratic_mean_error(const std::vector<double> & predicted_values,
											  const std::vector<double> & real_values) {

	double result = cuadratic_mean_error(predicted_values, real_values);
	// result = RMSE
	result = std::sqrt(result);

	return result;

}

double mean_absolute_error(const std::vector<double> & predicted_values,
									const std::vector<double> & real_values) {

	double result = 0.0;
	for ( unsigned i = 0; i < real_values.size(); i++ ) {
		result += std::abs(predicted_values[i] - real_values[i]);
	}

	result = result / static_cast<double>(real_values.size());

	return result;
}


} // namespace expressions_algs

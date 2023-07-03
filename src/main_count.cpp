
#include <iostream>

#include "expressions_algs/preprocess.hpp"


template <class T>
void count_class (const std::vector<T> & labels, const std::string & output) {
	std::vector<int> elements_per_phase(10, 0);

	for ( auto it = labels.begin(); it != labels.end(); ++it) {
		if ( expressions_algs::aux::compare_floats((*it), 19.0) ) {
			elements_per_phase[0]++;
		} else if ( expressions_algs::aux::compare_floats((*it), 20.5)) {
			elements_per_phase[1]++;
		} else if ( expressions_algs::aux::compare_floats((*it), 23.0)) {
			elements_per_phase[2]++;
		} else if ( expressions_algs::aux::compare_floats((*it), 25.5)) {
			elements_per_phase[3]++;
		} else if ( expressions_algs::aux::compare_floats((*it), 28.5)) {
			elements_per_phase[4]++;
		} else if ( expressions_algs::aux::compare_floats((*it), 32.5)) {
			elements_per_phase[5]++;
		} else if ( expressions_algs::aux::compare_floats((*it), 37.0)) {
			elements_per_phase[6]++;
		} else if ( expressions_algs::aux::compare_floats((*it), 42.0)) {
			elements_per_phase[7]++;
		} else if ( expressions_algs::aux::compare_floats((*it), 47.0)) {
			elements_per_phase[8]++;
		} else {
			elements_per_phase[9]++;
		}
	}

	std::ofstream count_output;
	count_output.open(output);

	for ( unsigned i = 0; i < elements_per_phase.size(); ++i) {
		count_output << i + 1 << "\t" << elements_per_phase[i] << std::endl;
	}

	count_output.close();

}

int main(int argc, char ** argv) {

	if ( argc < 3 ) {
		std::cerr << "ERROR: Bad number of arguments, expected file path and output." << std::endl;
		std::cerr << "\t Use: " << argv[0] << " <file> <output_file> [comment_char] [separator_char]" << std::endl;
		exit(-1);
	}

	std::string file = argv[1];
	std::string output = argv[2];
	char comment = '@';
	char separator = ',';

	if (argc >= 4) {
		comment = argv[3][0];
	}

	if (argc == 5) {
		separator = argv[4][0];
	}

	auto data = expressions_algs::preprocess::read_data<double>(file, comment, separator);

	count_class(data.second, output);


}

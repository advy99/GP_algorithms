#include <iostream>

#include "expressions_algs/preprocess.hpp"

std::pair<matriz<double>, std::vector<double> > regression_preprocess(const matriz<std::string> & data, const std::vector<std::string> labels) {
	matriz<double> result_data;
	std::vector<double> result_labels;

	result_data.resize(data.size());
	for(unsigned i = 0; i < data.size(); i++) {
		result_data[i].resize(data[i].size());

		for ( unsigned j = 0; j < data[i].size(); j++) {
			if ( data[i][j] == "NoGrooves" || data[i][j] == "Absence" || data[i][j] == "NotDefined"
				  || data[i][j] == "Absent" ) {
				result_data[i][j] = 1;
			} else if (  data[i][j] == "GroovesRest" || data[i][j] == "Medium" || data[i][j] == "Defined"
						||	 (data[i][j] == "Present" && j != 7 ) || data[i][j] == "InProcess" || data[i][j] == "PartiallyFormed"  ) {
				result_data[i][j] = 2;
			} else if (  data[i][j] == "GroovesShallow" || data[i][j] == "Much" || data[i][j] == "Present" || data[i][j] == "FormedWithoutRarefactions"  ) {
				result_data[i][j] = 3;
			} else if (  data[i][j] == "RidgesAndGrooves" || data[i][j] == "FormedWitFewRarefactions"  ) {
				result_data[i][j] = 4;
			} else if (  data[i][j] == "RidgesFormation" || data[i][j] == "FormedWithLotRecessesAndProtrusions" ) {
				result_data[i][j] = 5;
			} else if (  data[i][j] == "RegularPorosity" ) {
				result_data[i][j] = 6;
			}
		}

	}

	result_labels.resize(labels.size());
	for ( unsigned i = 0; i < labels.size() ; ++i) {
		result_labels[i] = atof(labels[i].c_str());
	}

	return std::make_pair(result_data, result_labels);

}




int main(int argc, char ** argv) {

	if ( argc < 3 ) {
		std::cerr << "ERROR: Wrong number of parameters." << std::endl;
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

	auto result = expressions_algs::preprocess::read_data<std::string>(file, comment, separator);

	matriz<std::string> data = result.first;
	std::vector<std::string> labels = result.second;

	auto result_phases = regression_preprocess(data, labels);

	expressions_algs::preprocess::write_data(output, result_phases.first, result_phases.second, separator);


}

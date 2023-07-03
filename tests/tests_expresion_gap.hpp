#ifndef TESTS_EXPRESION_GAP
#define TESTS_EXPRESION_GAP

#include <gtest/gtest.h>
#include "expressions_algs/Expression.hpp"
#include "expressions_algs/GA_P_Expression.hpp"

TEST (GA_P_Expression, IgualElMismo) {
	expressions_algs::GA_P_Expression exp;

	EXPECT_EQ(exp, exp);
}

TEST (GA_P_Expression, IgualElMismoConDatos) {
	expressions_algs::GA_P_Expression exp;

	exp.generate_random_expression(10, 0.3, 4);

	EXPECT_EQ(exp, exp);
}


TEST ( GA_P_Expression, ConstructorPorDefecto) {
	expressions_algs::GA_P_Expression exp1;
	expressions_algs::GA_P_Expression exp2;

	// el cromosoma es aleatorio, así que por asegurarnos que esta igual
	exp1.assign_chromosome(exp2.get_chromosome());

	EXPECT_EQ(exp1, exp2);
}

TEST ( GA_P_Expression, ConstructorCopia) {
	expressions_algs::GA_P_Expression exp1;

	exp1.generate_random_expression(10, 0.3, 3);

	expressions_algs::GA_P_Expression exp2(exp1);

	EXPECT_EQ(exp1, exp2);
}

TEST ( GA_P_Expression, OperadorAsignacion) {
	expressions_algs::GA_P_Expression exp1;

	exp1.generate_random_expression(10, 0.3, 3);

	expressions_algs::GA_P_Expression exp2;
	exp2 = exp1;

	EXPECT_EQ(exp1, exp2);
}

// TEST ( Expression, TotalmenteIguales) {
// 	expressions_algs::Expression exp1;
//
// 	exp1.generate_random_expression(10, 0.3, 3);
//
// 	expressions_algs::Expression exp2;
// 	exp2 = exp1;
//
// 	EXPECT_TRUE(exp1.totalmenteIguales(exp2));
// }


TEST ( GA_P_Expression, AsignarCromosoma) {
	expressions_algs::GA_P_Expression exp1;

	exp1.generate_random_expression(10, 0.3, 3);

	expressions_algs::GA_P_Expression exp2;

	exp2.assign_tree(exp1.get_tree());
	exp2.assign_chromosome(exp1.get_chromosome());

	EXPECT_EQ(exp1, exp2);
}

TEST (GA_P_Expression, MismaCadena) {
	expressions_algs::GA_P_Expression exp1;

	exp1.generate_random_expression(10, 0.3, 3);

	expressions_algs::GA_P_Expression exp2(exp1);

	EXPECT_EQ(exp1.string_expression(), exp2.string_expression());

}

TEST (GA_P_Expression, EvaluarDato) {

	expressions_algs::GA_P_Expression exp1;

	std::vector<expressions_algs::Node> arbol_tmp;
	arbol_tmp.resize(5);

	arbol_tmp[0].set_node_type(expressions_algs::NodeType::PLUS);

	arbol_tmp[1].set_node_type(expressions_algs::NodeType::VARIABLE);
	arbol_tmp[1].set_value(0);

	arbol_tmp[2].set_node_type(expressions_algs::NodeType::DOT);

	arbol_tmp[3].set_node_type(expressions_algs::NodeType::VARIABLE);
	arbol_tmp[3].set_value(1);
	arbol_tmp[4].set_node_type(expressions_algs::NodeType::NUMBER);
	arbol_tmp[4].set_value(1);

	std::vector<double> cromosoma;
	cromosoma.resize(5);

	for ( int i = 0; i < 5; i++) {
		cromosoma[i] = 2.2;
	}

	exp1.assign_tree(arbol_tmp);
	exp1.assign_chromosome(cromosoma);

	std::vector<double> dato = {2.5, 3.2};

	double resultado = exp1.evaluate_data(dato);

	EXPECT_TRUE( expressions_algs::aux::compare_floats(resultado, 9.54 ));

}


TEST (GA_P_Expression, ObtenerSurarbol) {

	expressions_algs::GA_P_Expression exp1;

	std::vector<expressions_algs::Node> arbol_tmp;
	arbol_tmp.resize(5);

	arbol_tmp[0].set_node_type(expressions_algs::NodeType::PLUS);

	arbol_tmp[1].set_node_type(expressions_algs::NodeType::VARIABLE);
	arbol_tmp[1].set_value(0);

	arbol_tmp[2].set_node_type(expressions_algs::NodeType::DOT);

	arbol_tmp[3].set_node_type(expressions_algs::NodeType::VARIABLE);
	arbol_tmp[3].set_value(1);
	arbol_tmp[4].set_node_type(expressions_algs::NodeType::VARIABLE);
	arbol_tmp[4].set_value(2);

	exp1.assign_tree(arbol_tmp);

	std::vector<expressions_algs::Node> subtree = exp1.get_subtree(exp1.get_tree(), 2);

	std::vector<expressions_algs::Node> subarbol_real;

	for ( unsigned i = 2; i < arbol_tmp.size(); i++) {
		subarbol_real.push_back(arbol_tmp[i]);
	}

	EXPECT_TRUE( subtree == subarbol_real );

}

#endif

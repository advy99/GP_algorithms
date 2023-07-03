#ifndef TESTS_EXPRESION
#define TESTS_EXPRESION

#include <gtest/gtest.h>
#include "expressions_algs/Expression.hpp"
#include "expressions_algs/GA_P_Expression.hpp"

TEST (Expression, IgualElMismo) {
	expressions_algs::Expression exp;

	EXPECT_EQ(exp, exp);
}

TEST (Expression, IgualElMismoConDatos) {
	expressions_algs::Expression exp;

	exp.generate_random_expression(20, 0.3, 4);

	EXPECT_EQ(exp, exp);
}


TEST ( Expression, ConstructorPorDefecto) {
	expressions_algs::Expression exp1;
	expressions_algs::Expression exp2;

	EXPECT_EQ(exp1, exp2);
}

TEST ( Expression, ConstructorCopia) {
	expressions_algs::Expression exp1;

	exp1.generate_random_expression(10, 0.3, 3);

	expressions_algs::Expression exp2(exp1);

	EXPECT_EQ(exp1, exp2);
}

TEST ( Expression, OperadorAsignacion) {
	expressions_algs::Expression exp1;

	exp1.generate_random_expression(10, 0.3, 3);

	expressions_algs::Expression exp2;
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


TEST ( Expression, AsignarArbol) {
	expressions_algs::Expression exp1;

	exp1.generate_random_expression(10, 0.3, 3);

	expressions_algs::Expression exp2;

	exp2.assign_tree(exp1.get_tree());

	EXPECT_EQ(exp1, exp2);
}

TEST ( Expression, AsignarCromosoma) {
	expressions_algs::Expression exp1;

	exp1.generate_random_expression(10, 0.3, 3);

	expressions_algs::Expression exp2;

	exp2.assign_tree(exp1.get_tree());

	EXPECT_EQ(exp1, exp2);
}

TEST (Expression, MismaCadena) {
	expressions_algs::Expression exp1;

	exp1.generate_random_expression(10, 0.3, 3);

	expressions_algs::Expression exp2(exp1);

	EXPECT_EQ(exp1.string_expression(), exp2.string_expression());

}

TEST (Expression, EvaluarDato) {

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


TEST (Expression, ObtenerSurarbol) {

	expressions_algs::Expression exp1;

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

TEST (Expression, CruzarArbol) {

	expressions_algs::Expression exp1;

	std::vector<expressions_algs::Node> arbol_tmp;
	arbol_tmp.resize(5);

	arbol_tmp[0].set_node_type(expressions_algs::NodeType::PLUS);

	arbol_tmp[1].set_node_type(expressions_algs::NodeType::VARIABLE);
	arbol_tmp[1].set_value(0);

	arbol_tmp[2].set_node_type(expressions_algs::NodeType::DOT);

	arbol_tmp[3].set_node_type(expressions_algs::NodeType::VARIABLE);
	arbol_tmp[3].set_value(1);
	arbol_tmp[4].set_node_type(expressions_algs::NodeType::NUMBER);
	arbol_tmp[4].set_numeric_value(1.2);

	std::vector<double> cromosoma;
	cromosoma.resize(5);

	for ( int i = 0; i < 5; i++) {
		cromosoma[i] = 2.2;
	}

	exp1.assign_tree(arbol_tmp);

	std::vector<expressions_algs::Node> arbol_tmp2;
	arbol_tmp2.resize(3);

	arbol_tmp2[0].set_node_type(expressions_algs::NodeType::PLUS);

	arbol_tmp2[1].set_node_type(expressions_algs::NodeType::VARIABLE);
	arbol_tmp2[1].set_value(0);

	arbol_tmp2[2].set_node_type(expressions_algs::NodeType::NUMBER);
	arbol_tmp2[2].set_numeric_value(1.45);

	expressions_algs::Expression exp2;
	exp2.assign_tree(arbol_tmp2);

	expressions_algs::Expression hijo = exp1;
	expressions_algs::Expression son2 = exp1;


	exp1.exchange_subtree(exp2, 4, 0, hijo);
	exp2.exchange_subtree(exp1, 0, 4, son2);

	EXPECT_TRUE( hijo.get_tree_length() == 7 && son2.get_tree_length() == 1);

}

TEST ( Expression, LeerFichero) {

	expressions_algs::Expression exp1(std::string("tests/expresion_prueba.txt"), 20, 9);

	std::string solucion = exp1.string_expression();

	EXPECT_EQ(solucion, std::string("( ( -0.188219  / ( x3  +  -1.088276 ) ) + ( ( -6.747766  -  9.194880 ) * ( -4.898098  * ( ( x3  /  9.770432 ) *  x0 ) ) ) )"));
}

#endif

#ifndef TESTS_NODO
#define TESTS_NODO

#include <gtest/gtest.h>
#include "expressions_algs/Node.hpp"


TEST (Node, IgualElMismo) {
	expressions_algs::Node nodo;

	EXPECT_EQ(nodo, nodo);
}

TEST (Node, ConstructorCopia) {
	expressions_algs::Node nodo;
	expressions_algs::Node another(nodo);

	EXPECT_EQ(nodo, another);
}

TEST (Node, ConstructorDefecto) {
	expressions_algs::Node nodo;
	expressions_algs::Node another;

	EXPECT_EQ(nodo, another);
}

TEST (Node, Asignacion) {
	expressions_algs::Node nodo;
	nodo.set_node_type(expressions_algs::NodeType::NUMBER);

	expressions_algs::Node another;

	another = nodo;

	EXPECT_EQ(nodo, another);
}

TEST (Node, DistintosTipos) {
	expressions_algs::Node nodo;
	nodo.set_node_type(expressions_algs::NodeType::NUMBER);

	expressions_algs::Node another;
	nodo.set_node_type(expressions_algs::NodeType::PLUS);


	EXPECT_NE(nodo, another);
}


TEST (Node, MismoTipoValorDistinto) {
	expressions_algs::Node nodo;
	nodo.set_node_type(expressions_algs::NodeType::PLUS);

	expressions_algs::Node another;
	another.set_node_type(expressions_algs::NodeType::PLUS);

	another.set_value(-23);

	EXPECT_EQ(nodo, another);
}

TEST (Node, DistintoTipoValorIgual) {
	expressions_algs::Node nodo;
	nodo.set_node_type(expressions_algs::NodeType::NUMBER);
	nodo.set_value(1);

	expressions_algs::Node another;
	another.set_node_type(expressions_algs::NodeType::VARIABLE);

	another.set_value(1);

	EXPECT_NE(nodo, another);
}

#endif

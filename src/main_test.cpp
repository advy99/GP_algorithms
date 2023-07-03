#include "tests/tests_nodo.hpp"
#include "tests/tests_expresion.hpp"
#include "tests/tests_expresion_gap.hpp"

#include <gtest/gtest.h>

#include <string>

int main(int argc, char ** argv) {

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

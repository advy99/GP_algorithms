# variables
CXX      := g++
HOME     := .
BIN      = $(HOME)/bin
INC	   = $(HOME)/include
SRC      = $(HOME)/src
OBJ      = $(HOME)/obj
DATA	   = $(HOME)/data
DOC      = $(HOME)/doc
CHARTS = $(HOME)/charts

SRC_ALG_POB = $(SRC)/expressions_algs
INC_ALG_POB = $(INC)/expressions_algs


# Default parameters
MESSAGE := "Compiling\ with\ C++17,\ max\ optimization\ level\ and\ all\ warnings\ enabled"
OLEVEL ?= 3
GPROF ?= 0
OPENMP ?= 1

ifeq ($(DEBUG), 1)
# target for debug (change flags and message)
OLEVEL = g -g
OPENMP = 0
MESSAGE := "Compiling\ with\ C++17,\ debug\ optimization\ level\ and\ symbols\ and\ all\ warnings\ enabled"
endif

ifeq ($(GPROF), 1)
F_GPROF = -pg
else
F_GPROF =
endif

ifeq ($(OPENMP), 1)
F_OPENMP = -fopenmp
else
F_OPENMP =
endif

O_LEVEL := -O$(strip $(OLEVEL))

CXXFLAGS = -std=c++17 $(O_LEVEL) $(F_OPENMP) $(F_GPROF) -Wall -Wextra -Wfloat-equal -Wpedantic


# main target
TARGET = $(BIN)/main
OBJECTS = $(OBJ)/main.o

# target for the lib
OBJECTS_ALGS_POB = $(OBJ)/Parameters.o $(OBJ)/Node.o $(OBJ)/Expression.o $(OBJ)/GP_alg.o $(OBJ)/GA_P_Expression.o $(OBJ)/GA_P_alg.o $(OBJ)/Random.o $(OBJ)/aux_expressions_alg.o
HEADERS_ALGS_POB = $(wildcard include/expressions_algs/*.hpp)

ALGS_POB_COMMON_HEADERS = $(INC)/Random.hpp $(INC_ALG_POB)/aux_expressions_alg.hpp

# target for test
TARGET_TEST = $(BIN)/main_test
OBJECTS_TEST = $(OBJ)/main_test.o

TARGET_PREPROCESS = $(BIN)/main_preprocess
OBJECTS_PREPROCESS = $(OBJ)/main_preprocess.o

# counter of number of objects compiled
N := $(shell echo $(TARGET) $(OBJECTS) $(OBJECTS_ALGS_POB) $(TARGET_TEST) $(OBJECTS_TEST) $(TARGET_PREPROCESS) $(OBJECTS_PREPROCESS) $(BIN)/main_count $(OBJ)/main_count.o $(BIN)/main_evaluate_expression_from_file $(OBJ)/main_evaluate_expression_from_file.o | wc -w )
X := 0
SUMA = $(eval X=$(shell echo $$(($(X)+1))))

# GoogleTest --> https://github.com/google/googletest
gtest      = /usr/include/gtest/
gtestlibs  = /usr/lib/libgtest.so
gtestflags = -I$(gtest) $(gtestlibs)

# targets
.PHONY: all make-folders debug START END doc clean-doc mrproper help tests exec-tests

all: make-folders START exec-tests $(TARGET) $(TARGET_PREPROCESS) $(BIN)/main_count $(BIN)/main_evaluate_expression_from_file doc END


# targer for only test compilation
tests: clean make-folders START $(TARGET_TEST) exec-tests END

# rule to compile and exec the tests
exec-tests: $(TARGET_TEST)
	@$(TARGET_TEST) --gtest_color=yes || (printf "\e[31mERROR: Tests failed. Aborting compilation\n"; false)
	@printf "\n\e[32mAll tests passed.\n\n"



$(TARGET_TEST): $(OBJECTS_ALGS_POB) $(HEADERS_ALGS_POB)  $(OBJECTS_TEST)
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreating binary $(TARGET_TEST) from $(OBJECTS_TEST)\n"
	@$(CXX) $(OBJECTS_ALGS_POB) $(OBJECTS_TEST) -o $(TARGET_TEST) $(F_OPENMP) $(gtestflags) -I$(INC)
	@printf "\n\e[36m$(TARGET_TEST) generated successfully.\e[0m\n\n"

$(TARGET_PREPROCESS): $(OBJECTS_ALGS_POB) $(HEADERS_ALGS_POB)  $(OBJECTS_PREPROCESS)
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreating binary $(TARGET_PREPROCESS) from $(OBJECTS_PREPROCESS)\n"
	@$(CXX) $(OBJECTS_ALGS_POB) $(OBJECTS_PREPROCESS) -o $(TARGET_PREPROCESS) $(F_OPENMP) $(gtestflags) -I$(INC)
	@printf "\n\e[36m$(TARGET_PREPROCESS) generated successfully.\e[0m\n\n"

$(BIN)/main_count : $(OBJECTS_ALGS_POB) $(HEADERS_ALGS_POB) $(OBJ)/main_count.o
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreating binary $(BIN)/main_count from $(OBJ)/main_count.o\n"
	@$(CXX) $(OBJECTS_ALGS_POB) $(OBJ)/main_count.o -o $(BIN)/main_count $(F_OPENMP) $(gtestflags) -I$(INC)
	@printf "\n\e[36m$(BIN)/main_count generated successfully.\e[0m\n\n"

$(BIN)/main_evaluate_expression_from_file : $(OBJECTS_ALGS_POB) $(HEADERS_ALGS_POB) $(OBJ)/main_evaluate_expression_from_file.o
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreating binary $(BIN)/main_evaluate_expression_from_file from $(OBJ)/main_evaluate_expression_from_file.o\n"
	@$(CXX) $(OBJECTS_ALGS_POB) $(OBJ)/main_evaluate_expression_from_file.o -o $(BIN)/main_evaluate_expression_from_file $(F_OPENMP) $(gtestflags) -I$(INC)
	@printf "\n\e[36m$(BIN)/main_evaluate_expression_from_file generated successfully.\e[0m\n\n"

# generic function to compile an obj
define compile_obj
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreating $(2) from $(1)\n"
	@$(CXX) -c $(CXXFLAGS) $(1) -I$(INC) -I. -o $(2)
endef



# start message and compiling of objects
START:
	@printf "\n\e[36mStarting compilating of $(BIN)/GA_P_alg\n\n"
	@printf "\e[94mCompiler: $(CXX)\n"
	@printf "\e[94mCompiler flags: $(CXXFLAGS)\n\n"
	@printf "\e[94m$(MESSAGE)\n\n"

$(TARGET): $(OBJECTS) $(OBJECTS_ALGS_POB) $(HEADERS_ALGS_POB)
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreating binary $(TARGET) from $(OBJECTS)\n"
	@$(CXX) $(OBJECTS) $(OBJECTS_ALGS_POB) $(CXXFLAGS) -o $@ -I$(INC) $(F_OPENMP) $(F_GPROF)
	@printf "\n\e[36m$(TARGET) generated successfully.\n\n"


$(OBJ)/Parameters.o: $(SRC_ALG_POB)/Parameters.cpp $(INC_ALG_POB)/Parameters.hpp $(ALGS_POB_COMMON_HEADERS)
	$(call compile_obj,$<,$@)

$(OBJ)/Node.o: $(SRC_ALG_POB)/Node.cpp $(INC_ALG_POB)/Node.hpp $(ALGS_POB_COMMON_HEADERS)
	$(call compile_obj,$<,$@)

$(OBJ)/Expression.o: $(SRC_ALG_POB)/Expression.cpp $(INC_ALG_POB)/Expression.hpp $(INC_ALG_POB)/Node.hpp $(ALGS_POB_COMMON_HEADERS)
	$(call compile_obj,$<,$@)

$(OBJ)/GA_P_Expression.o: $(SRC_ALG_POB)/GA_P_Expression.cpp $(INC_ALG_POB)/GA_P_Expression.hpp $(INC_ALG_POB)/Expression.hpp $(INC_ALG_POB)/Node.hpp $(ALGS_POB_COMMON_HEADERS)
	$(call compile_obj,$<,$@)

$(OBJ)/GA_P_alg.o: $(SRC_ALG_POB)/GA_P_alg.cpp $(INC_ALG_POB)/GA_P_alg.hpp $(INC_ALG_POB)/GA_P_Expression.hpp $(INC_ALG_POB)/Expression.hpp $(INC_ALG_POB)/Node.hpp $(ALGS_POB_COMMON_HEADERS)
	$(call compile_obj,$<,$@)

$(OBJ)/GP_alg.o: $(SRC_ALG_POB)/GP_alg.cpp $(INC_ALG_POB)/GP_alg.hpp $(INC_ALG_POB)/Expression.hpp $(INC_ALG_POB)/Node.hpp $(ALGS_POB_COMMON_HEADERS)
	$(call compile_obj,$<,$@)

$(OBJ)/aux_expressions_alg.o: $(SRC_ALG_POB)/aux_expressions_alg.cpp $(INC_ALG_POB)/aux_expressions_alg.hpp
	$(call compile_obj,$<,$@)

$(OBJ)/Random.o: $(SRC)/Random.cpp $(INC)/Random.hpp
	$(call compile_obj,$<,$@)



$(OBJ)/main_test.o: $(SRC)/main_test.cpp $(INC_ALG_POB)/GA_P_alg.hpp $(INC)/Random.hpp
	$(call compile_obj,$<,$@)

$(OBJ)/main.o: $(SRC)/main.cpp $(INC_ALG_POB)/GA_P_alg.hpp $(INC)/Random.hpp
	$(call compile_obj,$<,$@)

$(OBJ)/main_preprocess.o: $(SRC)/main_preprocess.cpp $(INC_ALG_POB)/preprocess.hpp
	$(call compile_obj,$<,$@)

$(OBJ)/main_count.o: $(SRC)/main_count.cpp $(INC_ALG_POB)/preprocess.hpp
	$(call compile_obj,$<,$@)

$(OBJ)/main_evaluate_expression_from_file.o: $(SRC)/main_evaluate_expression_from_file.cpp
	$(call compile_obj,$<,$@)


END:
	@printf "\n\e[36mCompilation finished sucessfully\n"

# cleaning targets
clean:
	@printf "\e[36mCleaning dir $(OBJ)\n"
	-@rm $(OBJ)/*.o 2> /dev/null || printf "\t\e[33m$(OBJ) is empty, nothing to clean\n"
	@printf "\e[36mCleaning dir $(BIN)\n"
	-@rm $(BIN)/* 2> /dev/null || printf "\t\e[33m$(BIN) is empty, nothing to clean\n"
	@printf "\e[36mAll binaries cleaned.\n"

clean-doc:
	@printf "\e[36mCleaning older documentation\n"
	-@rm $(DOC)/html -r 2> /dev/null || printf "\t\e[33mNo HTML documentation found.\n"
	-@rm $(DOC)/html -r 2> /dev/null || printf "\t\e[33mNo LaTeX documentation found.\n"

mrproper: clean clean-doc


#documentation targets
doc:
	@printf "\e[36mStarting project documentation compilation.\n\e[33m\n"
	@doxygen doc/doxys/Doxyfile
	@printf "\e[36mCompilation of the documentation has been completed. You can consult it at doc/html/index.html\n"

# targerts for generating dirs
make-folders:
	@printf "\e[36mCreating necessary folders\e[0m\n"
	-@mkdir $(OBJ) 2> /dev/null || printf "\t\e[33mFolder $(OBJ) already exists.\n"
	-@mkdir $(BIN) 2> /dev/null || printf "\t\e[33mFolder $(BIN) already exists.\n"
	-@mkdir $(CHARTS) 2> /dev/null || printf "\t\e[33mFolder $(CHARTS) already exists.\n"
	-@mkdir $(CHARTS)/data 2> /dev/null || printf "\t\e[33mFolder $(CHARTS)/data already exists.\n"


help:
	@printf "\e[33mMakefile usage:\n"
	@printf "\t\e[36mCompile with optimization: \t     \e[94mmake\n"
	@printf "\t\e[36mCompile and exec tests: \t     \e[94mmake \e[0mtests\n"
	@printf "\t\e[36mCompile documentation: \t     \e[94mmake \e[0mdoc\n"
	@printf "\t\e[36mClean binaries: \t     \e[94mmake \e[0mclean\n"
	@printf "\t\e[36mClean documentation: \t\t     \e[94mmake \e[0mclean-doc\n"
	@printf "\t\e[36mClean all: \t\t\t     \e[94mmake \e[0mmrproper\n\n"
	@printf "\e[33mOther available options:\n"
	@printf "\t\e[36mOptimization level(0, 1, 2, 3, g):   \e[94mmake \e[0mOLEVEL=3\n"
	@printf "\t\e[36mCompile with OpenMP(0, 1): \t     \e[94mmake \e[0mOPENMP=1\n"
	@printf "\t\e[36mEnable debug symbols(0, 1):   \e[94mmake \e[0mDEBUG=0\n"
	@printf "\t\e[36mCompile for GPROF(0, 1): \t     \e[94mmake \e[0mGPROF=0\n"

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


# flags de compilacion por defecto
MENSAJE := "Compilando\ usando\ C++17,\ sin\ optimización\ de\ máximo\ nivel\ y\ con\ todos\ los\ warnings\ activados"
OPTIMIZACION ?= 3
GPROF ?= 0
OPENMP ?= 1

ifeq ($(DEBUG), 1)
# target para debug (cambiamos flags y el mensaje)
OPTIMIZACION = g -g
OPENMP = 0
MENSAJE = "Compilando\ usando\ C++17,\ con\ optimización,\ con\ todos\ los\ warnings\ activados\ y\ con\ símbolos\ de\ depuración"
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

O_LEVEL := -O$(strip $(OPTIMIZACION))

CXXFLAGS = -std=c++17 $(O_LEVEL) $(F_OPENMP) $(F_GPROF) -Wall -Wextra -Wfloat-equal -Wpedantic


# objetivo principal
OBJETIVO = $(BIN)/main
OBJETOS = $(OBJ)/main.o

# objetivos de la biblioteca GA_P_alg
OBJETOS_ALGS_POB = $(OBJ)/Parameters.o $(OBJ)/Node.o $(OBJ)/Expression.o $(OBJ)/GP_alg.o $(OBJ)/GA_P_Expression.o $(OBJ)/GA_P_alg.o $(OBJ)/Random.o $(OBJ)/aux_expressions_alg.o
CABECERAS_ALGS_POB = $(wildcard include/expressions_algs/*.hpp)

ALGS_POB_INC_COMUNES = $(INC)/Random.hpp $(INC_ALG_POB)/aux_expressions_alg.hpp

# objetivos de los tests
OBJETIVO_TEST = $(BIN)/main_test
OBJETOS_TEST = $(OBJ)/main_test.o

OBJETIVO_PREPROCESADO = $(BIN)/main_preprocesar
OBJETOS_PREPROCESADO = $(OBJ)/main_preprocesar.o

# variables para el contador de reglas
N := $(shell echo $(OBJETIVO) $(OBJETOS) $(OBJETOS_ALGS_POB) $(OBJETIVO_TEST) $(OBJETOS_TEST) $(OBJETIVO_PREPROCESADO) $(OBJETOS_PREPROCESADO) $(BIN)/main_count $(OBJ)/main_count.o $(BIN)/main_evaluar_expresion_archivo $(OBJ)/main_evaluar_expresion_archivo.o | wc -w )
X := 0
SUMA = $(eval X=$(shell echo $$(($(X)+1))))

# variables de GoogleTest --> https://github.com/google/googletest
gtest      = /usr/include/gtest/
gtestlibs  = /usr/lib/libgtest.so
gtestflags = -I$(gtest) $(gtestlibs)

# targets
.PHONY: all make-folders debug INICIO FIN doc clean-doc mrproper help tests ejecutar-tests

# target por defecto
all: make-folders INICIO ejecutar-tests $(OBJETIVO) $(OBJETIVO_PREPROCESADO) $(BIN)/main_count $(BIN)/main_evaluar_expresion_archivo doc FIN


# target para compilar solo los tests
tests: clean make-folders INICIO $(OBJETIVO_TEST) ejecutar-tests FIN

# reglas para compilar y ejecutar los test
ejecutar-tests: $(OBJETIVO_TEST)
	@$(OBJETIVO_TEST) --gtest_color=yes || (printf "\e[31mERROR: No se ha conseguido pasar todos los tests, abortando la compilación\n"; false)
	@printf "\n\e[32mTests ejecutados correctamente\n\n"



$(OBJETIVO_TEST): $(OBJETOS_ALGS_POB) $(CABECERAS_ALGS_POB)  $(OBJETOS_TEST)
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreando el binario $(OBJETIVO_TEST) a partir de $(OBJETOS_TEST)\n"
	@$(CXX) $(OBJETOS_ALGS_POB) $(OBJETOS_TEST) -o $(OBJETIVO_TEST) $(F_OPENMP) $(gtestflags) -I$(INC)
	@printf "\n\e[36mCompilación de $(OBJETIVO_TEST) finalizada con exito.\e[0m\n\n"

$(OBJETIVO_PREPROCESADO): $(OBJETOS_ALGS_POB) $(CABECERAS_ALGS_POB)  $(OBJETOS_PREPROCESADO)
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreando el binario $(OBJETIVO_PREPROCESADO) a partir de $(OBJETOS_PREPROCESADO)\n"
	@$(CXX) $(OBJETOS_ALGS_POB) $(OBJETOS_PREPROCESADO) -o $(OBJETIVO_PREPROCESADO) $(F_OPENMP) $(gtestflags) -I$(INC)
	@printf "\n\e[36mCompilación de $(OBJETIVO_PREPROCESADO) finalizada con exito.\e[0m\n\n"

$(BIN)/main_count : $(OBJETOS_ALGS_POB) $(CABECERAS_ALGS_POB) $(OBJ)/main_count.o
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreando el binario $(BIN)/main_count a partir de $(OBJ)/main_count.o\n"
	@$(CXX) $(OBJETOS_ALGS_POB) $(OBJ)/main_count.o -o $(BIN)/main_count $(F_OPENMP) $(gtestflags) -I$(INC)
	@printf "\n\e[36mCompilación de $(BIN)/main_count finalizada con exito.\e[0m\n\n"

$(BIN)/main_evaluar_expresion_archivo : $(OBJETOS_ALGS_POB) $(CABECERAS_ALGS_POB) $(OBJ)/main_evaluar_expresion_archivo.o
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreando el binario $(BIN)/main_evaluar_expresion_archivo a partir de $(OBJ)/main_evaluar_expresion_archivo.o\n"
	@$(CXX) $(OBJETOS_ALGS_POB) $(OBJ)/main_evaluar_expresion_archivo.o -o $(BIN)/main_evaluar_expresion_archivo $(F_OPENMP) $(gtestflags) -I$(INC)
	@printf "\n\e[36mCompilación de $(BIN)/main_evaluar_expresion_archivo finalizada con exito.\e[0m\n\n"

# funcion para compilar un objeto
define compilar_objeto
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreando el objeto $(2) a partir de $(1)\n"
	@$(CXX) -c $(CXXFLAGS) $(1) -I$(INC) -I. -o $(2)
endef


# mensaje de inicio y compilacion del objetivo y objeros

INICIO:
	@printf "\n\e[36mComenzando compilación de $(BIN)/GA_P_alg\n\n"
	@printf "\e[94mCompilador: $(CXX)\n"
	@printf "\e[94mFlags del compilador: $(CXXFLAGS)\n\n"
	@printf "\e[94m$(MENSAJE)\n\n"

$(OBJETIVO): $(OBJETOS) $(OBJETOS_ALGS_POB) $(CABECERAS_ALGS_POB)
	@$(SUMA)
	@printf "\e[31m[$(X)/$(N)] \e[32mCreando el binario $(OBJETIVO) a partir de $(OBJETOS)\n"
	@$(CXX) $(OBJETOS) $(OBJETOS_ALGS_POB) $(CXXFLAGS) -o $@ -I$(INC) $(F_OPENMP) $(F_GPROF)
	@printf "\n\e[36mCompilación de $(OBJETIVO) finalizada con exito.\n\n"


$(OBJ)/Parameters.o: $(SRC_ALG_POB)/Parameters.cpp $(INC_ALG_POB)/Parameters.hpp $(ALGS_POB_INC_COMUNES)
	$(call compilar_objeto,$<,$@)

$(OBJ)/Node.o: $(SRC_ALG_POB)/Node.cpp $(INC_ALG_POB)/Node.hpp $(ALGS_POB_INC_COMUNES)
	$(call compilar_objeto,$<,$@)

$(OBJ)/Expression.o: $(SRC_ALG_POB)/Expression.cpp $(INC_ALG_POB)/Expression.hpp $(INC_ALG_POB)/Node.hpp $(ALGS_POB_INC_COMUNES)
	$(call compilar_objeto,$<,$@)

$(OBJ)/GA_P_Expression.o: $(SRC_ALG_POB)/GA_P_Expression.cpp $(INC_ALG_POB)/GA_P_Expression.hpp $(INC_ALG_POB)/Expression.hpp $(INC_ALG_POB)/Node.hpp $(ALGS_POB_INC_COMUNES)
	$(call compilar_objeto,$<,$@)

$(OBJ)/GA_P_alg.o: $(SRC_ALG_POB)/GA_P_alg.cpp $(INC_ALG_POB)/GA_P_alg.hpp $(INC_ALG_POB)/GA_P_Expression.hpp $(INC_ALG_POB)/Expression.hpp $(INC_ALG_POB)/Node.hpp $(ALGS_POB_INC_COMUNES)
	$(call compilar_objeto,$<,$@)

$(OBJ)/GP_alg.o: $(SRC_ALG_POB)/GP_alg.cpp $(INC_ALG_POB)/GP_alg.hpp $(INC_ALG_POB)/Expression.hpp $(INC_ALG_POB)/Node.hpp $(ALGS_POB_INC_COMUNES)
	$(call compilar_objeto,$<,$@)

$(OBJ)/aux_expressions_alg.o: $(SRC_ALG_POB)/aux_expressions_alg.cpp $(INC_ALG_POB)/aux_expressions_alg.hpp
	$(call compilar_objeto,$<,$@)

$(OBJ)/Random.o: $(SRC)/Random.cpp $(INC)/Random.hpp
	$(call compilar_objeto,$<,$@)



$(OBJ)/main_test.o: $(SRC)/main_test.cpp $(INC_ALG_POB)/GA_P_alg.hpp $(INC)/Random.hpp
	$(call compilar_objeto,$<,$@)

$(OBJ)/main.o: $(SRC)/main.cpp $(INC_ALG_POB)/GA_P_alg.hpp $(INC)/Random.hpp
	$(call compilar_objeto,$<,$@)

$(OBJ)/main_preprocesar.o: $(SRC)/main_preprocesar.cpp $(INC_ALG_POB)/preprocess.hpp
	$(call compilar_objeto,$<,$@)

$(OBJ)/main_count.o: $(SRC)/main_count.cpp $(INC_ALG_POB)/preprocess.hpp
	$(call compilar_objeto,$<,$@)

$(OBJ)/main_evaluar_expresion_archivo.o: $(SRC)/main_evaluar_expresion_archivo.cpp
	$(call compilar_objeto,$<,$@)


# mensaje de fin
FIN:
	@printf "\n\e[36mCompilación finalizada con éxito\n"

# targets de limpieza
clean:
	@printf "\e[36mLimpiando el directorio $(OBJ)\n"
	-@rm $(OBJ)/*.o 2> /dev/null || printf "\t\e[33mEl directorio $(OBJ) está vacio, nada que limpiar\n"
	@printf "\e[36mLimpiando el directorio $(BIN)\n"
	-@rm $(BIN)/* 2> /dev/null || printf "\t\e[33mEl directorio $(BIN) está vacio, nada que limpiar\n"
	@printf "\e[36mLimpieza completada\n"

clean-doc:
	@printf "\e[36mLimpiando la documentación\n"
	-@rm $(DOC)/html -r 2> /dev/null || printf "\t\e[33mNo existe documentación generada en HTML\n"
	-@rm $(DOC)/html -r 2> /dev/null || printf "\t\e[33mNo existe documentación generada en LaTeX\n"

mrproper: clean clean-doc


# target de documentacion
doc:
	@printf "\e[36mComenzando compilación de la documentación del proyecto\n\e[33m\n"
	@doxygen doc/doxys/Doxyfile
	@printf "\e[36mFinalizada compilación de la documentación. Puedes consultarla en doc/html/index.html\n"

# target para crear las carpetas necesarias
make-folders:
	@printf "\e[36mCreando carpetas necesarias\e[0m\n"
	-@mkdir $(OBJ) 2> /dev/null || printf "\t\e[33mYa existe la carpeta $(OBJ)\n"
	-@mkdir $(BIN) 2> /dev/null || printf "\t\e[33mYa existe la carpeta $(BIN)\n"
	-@mkdir $(CHARTS) 2> /dev/null || printf "\t\e[33mYa existe la carpeta $(CHARTS)\n"
	-@mkdir $(CHARTS)/data 2> /dev/null || printf "\t\e[33mYa existe la carpeta $(CHARTS)/data\n"


# target para mostrar ayuda
help:
	@printf "\e[33mUso del Makefile:\n"
	@printf "\t\e[36mCompilar con optimización: \t     \e[94mmake\n"
	@printf "\t\e[36mCompilar y ejecutar tests: \t     \e[94mmake \e[0mtests\n"
	@printf "\t\e[36mCompilar documentación: \t     \e[94mmake \e[0mdoc\n"
	@printf "\t\e[36mLimpiar binarios y objetos: \t     \e[94mmake \e[0mclean\n"
	@printf "\t\e[36mLimpiar documentación: \t\t     \e[94mmake \e[0mclean-doc\n"
	@printf "\t\e[36mLimpiar todo: \t\t\t     \e[94mmake \e[0mmrproper\n\n"
	@printf "\e[33mTambién tienes disponibles las siguientes variables:\n"
	@printf "\t\e[36mNivel optimizacion(0, 1, 2, 3, g):   \e[94mmake \e[0mOPTIMIZACION=3\n"
	@printf "\t\e[36mCompilar con OpenMP(0, 1): \t     \e[94mmake \e[0mOPENMP=1\n"
	@printf "\t\e[36mActivar símbolos depuracion(0, 1):   \e[94mmake \e[0mDEBUG=0\n"
	@printf "\t\e[36mCompilar para GPROF(0, 1): \t     \e[94mmake \e[0mGPROF=0\n"

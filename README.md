# expressions_algs

This repository contains the implementation of several Genetic Programming (classic GP, GA_P) algorithms in C++.

# Code structure

## Important parts of code 

There are several namespaces for each task:

- `expressions_algs::Population_alg`: Abstract class with the common elements of an algorithm with a population.
- `expressions_algs::Population`: Class that represents a set of individuals that make up a population.
- `expressions_algs::Expression`:  Class that represents a Genetic Programming expression.
- `expressions_algs::Node`: Class that represents a node of a Genetic Programming expression.
- `expressions_algs::GP_alg`: Class that implements the Genetic Programming algorithm.
- `expressions_algs::GA_P_Expression`: Class that implements a GA-P expression.
- `expressions_algs::GA_P_alg`: Class that implements the GA-P algorithm.

In each of these classes you will find more information about their components and operators.


# Documentation

You can generate the documentation running:

```sh
make doc
```

You can consult it by opening `docs/index.html` in your browser.


# Compiling the project

You can compile the project with:

```sh
make
```


You can consult all the options of the Makefile with the `help` target:

```sh
make help
```

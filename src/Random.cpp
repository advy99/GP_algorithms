#include <ctime>
#include "Random.hpp"
#include <iostream>

void Random :: set_seed(const unsigned long seed){
	generator_.seed(seed);
}

Random :: ~Random(){
}

float Random :: get_float(){
	std::uniform_real_distribution<> dis(0, 1.0);
	return dis(generator_);
}

float Random :: get_float(const float LOW, const float HIGH){
	std::uniform_real_distribution<> dis(LOW, HIGH);
	return dis(generator_);
}

float Random :: get_float(const float HIGH){
	return get_float(0, HIGH);
}


int Random :: get_int(const int LOW, const int HIGH){
	std::uniform_int_distribution<> dis(LOW, HIGH - 1);
	return dis(generator_);
}

int Random :: get_int(const int HIGH){
	return get_int(0, HIGH);
}

std::mt19937 Random :: get_generator() {
	return generator_;
}


std::mt19937 Random :: generator_;

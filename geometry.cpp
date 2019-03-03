#include <iostream>
#include <cstdlib>
#include <cmath>
#include "geometry.h"

using namespace std;

Position::Position(void) {
	X = 0;
	Y = 0;
}
Position::Position(size_t x, size_t y) {
	X = x;
	Y = y;
}
Position::~Position(void) {
	
}

void Position::Input(std::istream &is) {
	is >> X;
	is >> Y;
}

void Position::Print(void) {
	cout << "(" << X << ":" << Y << ")";
}

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "geometry.h"
#include "environment.h"

using namespace std;

Orientation GetRandomOrientation() {
	int tmp = rand() % 2;
	if (tmp == 0) {
		return Orientation::VERTICAL;
	} else {
		return Orientation::HORISONTAL;
	}
}

Position::Position(void) {
	X = 0;
	Y = 0;
}
Position::Position(int x, int y) {
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

void Position::SetRandom(void) {
	X = rand() % MAP_WIDTH;
	Y = rand() % MAP_HEIGHT;
}

int ManhattanDistance(Position pos1, Position pos2) {
	return abs(pos2.X-pos1.X)+abs(pos2.Y-pos1.Y);
}

#include <iostream>

const char MAP_WIDTH = 10;
const char MAP_HEIGHT = MAP_WIDTH;
const char MAP_ELEMENT_EMPTY = '.';
const char MAP_ELEMENT_SPACE = ' ';
const char MAP_ELEMENT_DAMAGED='X';
int NumCount(int num);

class Map {
	char Matrix[MAP_HEIGHT][MAP_WIDTH];
public:
	Map();
	void Print();
	void Shoot(int x, int y);
};

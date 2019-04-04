#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <iostream>
#include <cstdlib>

enum Orientation {
	VERTICAL,
	HORISONTAL,
	UNDEFINED
};

Orientation GetRandomOrientation();

//Класс для хранения и обработки координат
class Position {
public:
	int X;
	int Y;

	Position(void);
	Position(int x, int y);
	~Position(void);

	void Input(std::istream &is);
	void Print(void);
	void SetRandom(void);
};

int ManhattanDistance(Position pos1, Position pos2);


#endif /* GEOMETRY_H */
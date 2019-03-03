#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <iostream>
#include <cstdlib>

enum Orientation {
	VERTICAL,
	HORISONTAL
};

//Класс для хранения и обработки координат
class Position {
public:
	size_t X;
	size_t Y;

	Position(void);
	Position(size_t x, size_t y);
	~Position(void);

	void Input(std::istream &is);
	void Print(void);

};


#endif /* GEOMETRY_H */
#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>
#include "environment.h"
#include "geometry.h"
#include <vector>

int NumCount(int num);

class Map;

class Ship{
	int Length;
	Position Pos;
	Orientation Orient;
	Map *Battlefield;
	bool IsInit;
public:
	Ship(Map *battlefield, int length);
	bool Init(Position pos, Orientation orient);
	Position GetPosition();
	Orientation GetOrientation();
	int GetLength();
};

class Map {
	char Matrix[MAP_HEIGHT][MAP_WIDTH];
	std::vector <Ship> ShipsList;
	bool CheckPosition(Ship *ship);
	bool IsPosCorrect(Position pos);
public:
	Map();
	void PrintForEnemy();
	void PrintForMe();
	void Shoot(int x, int y);
	bool DrawShip(Ship *ship);
};

#endif

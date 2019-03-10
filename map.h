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
	int Damage;
public:
	Ship(Map *battlefield, int length);
	bool Init(Position pos, Orientation orient);
	Position GetPosition();
	Orientation GetOrientation();
	int GetLength();
	int GetDamage();
	bool IsDead();
	void DrawCircuit();
	bool Harm();
};

struct MapField {
	char Sign;
	Ship *Battleship;
};

class Map {
	MapField Matrix[MAP_HEIGHT][MAP_WIDTH];
	std::vector <Ship*> ShipsList[SHIPS_MAX_LENGTH];
	bool CheckPosition(Ship *ship);
	bool IsPosCorrect(Position pos);
public:
	Map();
	~Map();
	void PrintForEnemy();
	void PrintForMe();
	void Shoot(Position pos);
	bool DrawShip(Ship *ship);
	void RandomFill();
	friend Ship;
};

#endif

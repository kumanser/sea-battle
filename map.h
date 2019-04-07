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

	void ClearShipsList();
	void ClearMatrix();
public:
	Map();
	~Map();
	void Clear();
	void PrintForEnemy();
	void PrintForMe();
	void Shoot(Position pos);
	void Shoot(Position pos, int radius);
	bool DrawShip(Ship *ship);
	bool AddShip(Position pos, Orientation orient, int length);
	bool RemoveShip(Position pos);
	void RandomFill();
	void GetCharMap(char **char_map);
	MapField GetMapElement(int i, int j);
	MapField GetMapElement(Position pos);
	friend Ship;
};

class MapBasic {
public:
	char Matrix[MAP_HEIGHT][MAP_WIDTH];

	MapBasic();
	void Import(Map &map);
};

void SetEmptyCharMap(char **char_map);

#endif

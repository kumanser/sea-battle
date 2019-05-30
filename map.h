#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>
#include "environment.h"
#include "geometry.h"
#include <vector>

enum ShootResult {
	INCORRECT,
	SLIP,
	HURT,
	KILLED
};

ShootResult GetMaxShootResult(ShootResult a, ShootResult b);

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

int LinearizePos(Position pos);

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
	ShootResult Shoot(Position pos);
	ShootResult Shoot(Position pos, int radius);
	bool DrawShip(Ship *ship);
	bool AddShip(Position pos, Orientation orient, int length);
	bool RemoveShip(Position pos);
	void RandomFill();
	bool IsAllShipsDead();
	//void GetCharMap(char **char_map);
	MapField GetMapElement(int i, int j);
	MapField GetMapElement(Position pos);

	friend Ship;
};

class MapBasic {
public:
	char Matrix[MAP_HEIGHT][MAP_WIDTH];

	MapBasic();
	void Import(Map &map);
	void Print();
	std::string ConvertToArray();
};

void SetEmptyCharMap(char **char_map);

ShootResult GetShootResultByAns(char ans);
std::string SetShootResultAns(ShootResult);

#endif

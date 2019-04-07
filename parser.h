#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"

class InitElements {
	int ShipsNotUsed[SHIPS_MAX_LENGTH];

public:
	void SetDefault();
	void UseAllShips();
	bool UseShip(int length);
	void FreeShip(int length);
};


std::string GetParameter(std::string str, int num);
int LetterToCoordinate(char ch);
Position CoordinateParse(std::string cmd);
Orientation OrientationParse(std::string cmd);
void PrintHelp();
bool ParseCmdSet(Map &map, std::string cmd, InitElements &init_data);
bool ParseCmdDelete(Map &map, std::string cmd, InitElements &init_data);
void ParseCmdClear(Map &map, InitElements &init_data);
bool ParseCommandShipInit(Map &map, std::string cmd, InitElements &init_data);


#endif

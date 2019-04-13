#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"

struct ParserStepResult {
	bool GlobalContinue;
	bool LocalContinue;
	ShootResult ShootStatus;

	ParserStepResult();
};

class InitElements {
	int ShipsNotUsed[SHIPS_MAX_LENGTH];

public:
	void SetDefault();
	void UseAllShips();
	bool UseShip(int length);
	void FreeShip(int length);
	bool IsAllUsed();
};


std::string GetParameter(std::string str, int num);
int LetterToCoordinate(char ch);
Position CoordinateParse(std::string cmd);
Orientation OrientationParse(std::string cmd);
void PrintHelp();
bool ParseCmdSet(Map &map, std::string cmd, InitElements &init_data);
bool ParseCmdDelete(Map &map, std::string cmd, InitElements &init_data);
void ParseCmdClear(Map &map, InitElements &init_data);
ParserStepResult ParseCommandShipInit(Map &map, std::string cmd, InitElements &init_data);
ParserStepResult ParseCommandGameplay(Map &map_me, MapBasic &map_enemy, std::string cmd, int radius);


#endif

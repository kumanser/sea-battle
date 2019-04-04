#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"

int CharToInt(char ch);
int StrToInt(std::string str);
char ToLower(char ch);
int LetterToCoordinate(char ch);
Position CoordinateParse(std::string cmd);
void PrintHelp();
bool ParseCommandShipInit(Map *map, std::string cmd);

#endif

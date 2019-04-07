#ifndef INTERFACE_H
#define INTERFACE_H

#include "environment.h"
#include "geometry.h"
#include "map.h"
#include "parser.h"
#include <vector>

enum InterfaceStatus {
	CONNECT,
	INIT,
	GAMEPLAY
};

class Interface {
	InterfaceStatus Status;
	Map MapMe;
	//char MapEnemy[MAP_HEIGHT][MAP_WIDTH];
	MapBasic MapEnemy;
	InitElements InitData;
public:

	Interface();

	bool InitProcess();
};

#endif

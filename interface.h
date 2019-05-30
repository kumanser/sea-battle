#ifndef INTERFACE_H
#define INTERFACE_H

#include "environment.h"
#include "geometry.h"
#include "map.h"
#include "parser.h"
#include "connection.h"
#include <vector>

/*enum InterfaceStatus {
	CONNECT,
	INIT,
	GAMEPLAY
};*/


class Interface {
	//InterfaceStatus Status;
	Connection Network;
	Map MapMe;
	//char MapEnemy[MAP_HEIGHT][MAP_WIDTH];
	MapBasic MapEnemy;
	InitElements InitData;
	int CurrRadius;
	bool FirstClientRecv;

	void PrintCmdPrefix();
public:

	Interface();
	void Start();
	void InitDevice();
	bool InitConnection();
	bool InitProcess();
	bool GameplayProcess();
	bool ActiveMode();
	bool PassiveMode();
	ShootResult DrawResult(std::string ans);
};

#endif

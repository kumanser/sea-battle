#include <iostream>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"
#include "parser.h"
#include "interface.h"

using namespace std;

Interface::Interface() {
	CurrRadius = 0;
}

void Interface::PrintCmdPrefix() {
	cout << ">>> ";
}

bool Interface::InitProcess() {
	ParserStepResult res;
	while (res.LocalContinue) {
		string cmd;
		PrintCmdPrefix();
		getline(cin, cmd);
		res = ParseCommandShipInit(MapMe, cmd, InitData);
	}

	return res.GlobalContinue;
}

bool Interface::GameplayProcess() {
	ParserStepResult res;
	while (res.LocalContinue) {
		string cmd;
		PrintCmdPrefix();
		getline(cin, cmd);
		res = ParseCommandGameplay(MapMe, MapEnemy, cmd, CurrRadius);
	}

	return res.GlobalContinue;
}


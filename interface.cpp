#include <iostream>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"
#include "parser.h"
#include "interface.h"

using namespace std;

Interface::Interface() {
}

bool Interface::InitProcess() {
	while (true) {
		string cmd;
		cout << ">>> ";
		getline(cin, cmd);
		if (!ParseCommandShipInit(MapMe, cmd, InitData)) {
			break;
		}
	}
	return false;
}


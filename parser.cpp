#include <iostream>
#include <cstdlib>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"
#include "parser.h"
#include "converter.h"

using namespace std;

string GetParameter(string str, int num) {
	str = RemoveSpecial(str);
	int cnt = 0;
	int i;
	bool special = false;
	for (i = 0; cnt < num && i < str.size(); i++) {
		if (special) {
			special = false;
			continue;
		}
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
			cnt++;
		}
		if (str[i] == '\\') {
		   special = true;
		}
	}

	special = false;
	string res = "";
	for (; cnt == num && i < str.size(); i++) {
		if (special) {
			res += str[i];
			special = false;
			continue;
		}

		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
			break;
		}
		if (str[i] == '\\') {
		   special = true;
		   continue;
		}
		res += str[i];
	}
	return res;
}

int LetterToCoordinate(char ch) {
	ch = CharToLower(ch);
	return ch - 'a';
}

Position CoordinateParse(string cmd) {
	string num_str = cmd.substr(1);
	Position pos(StringToUNum(num_str) - 1, LetterToCoordinate(cmd[0]));
	return pos;
}

Orientation OrientationParse(string cmd) {
	if (cmd[0] == 'v') {
		return Orientation::VERTICAL;
	}
	if (cmd[0] == 'h') {
		return Orientation::HORISONTAL;
	}
	return Orientation::UNDEFINED;

}

void PrintHelp() {
	cout << "==================================================================================" << endl;
	cout << "|set <length (3)> <position (A2)> <orientation (v/h)> — поместить корабль на поле|" << endl;
	cout << "|    Пример: set 3 A2 v                                                          |" << endl;
	cout << "|set random — случайная расстановка кораблей                                     |" << endl;
	cout << "|delete <position (A2)> — удаление корабля, расположенного на данной клетке      |" << endl;
	cout << "|    Пример: delete A2                                                           |" << endl;
	cout << "|clear — очистить поле                                                           |" << endl;
	cout << "|exit — выйти                                                                    |" << endl;
	cout << "==================================================================================" << endl << endl;
}

bool ParseCmdSet(Map *map, string cmd) {
	if (GetParameter(cmd, 1) == "random") {
		map->RandomFill();
		return true;
	}
	int length = StringToUNum(GetParameter(cmd, 1));
	Position pos = CoordinateParse(GetParameter(cmd, 2));
	Orientation orient = OrientationParse(GetParameter(cmd, 3));
	return map->AddShip(pos, orient, length);
}

bool ParseCmdDelete(Map *map, string cmd) {
	return map->RemoveShip(CoordinateParse(GetParameter(cmd, 1)));
}

bool ParseCommandShipInit(Map *map, string cmd) {
	string cmd_name = GetParameter(cmd, 0);

	if (cmd_name == "help") {
		PrintHelp();
		return true;
	}

	if (cmd_name == "set") {
		ParseCmdSet(map, cmd);
		return true;
	}

	if (cmd_name == "delete") {
		ParseCmdDelete(map, cmd);
		return true;
	}

	if (cmd_name == "clear") {
		map->Clear();
		return true;
	}

	if (cmd_name == "exit") {
		return false;
	}



	/*switch (cmd_name) {
		case("help"):
			PrintHelp();
			break;
		case "set":
			ParseCmdSet(map, cmd);
			break;
		case "delete":
			ParseCmdDelete(map, cmd);
			break;
		case "clear":
			map->Clear();
			break;
		case "exit":
			return false;
	}*/
	cout << "Команда введена неверно" << endl;
	return true;
}

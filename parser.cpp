#include <iostream>
#include <cstdlib>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"
#include "parser.h"
#include "converter.h"

using namespace std;

void InitElements::SetDefault() {
	for(int i=0; i<SHIPS_MAX_LENGTH; i++){
		ShipsNotUsed[i]=SHIPS_MAX_LENGTH-i;
	}
}
void InitElements::UseAllShips() {
	for (int i=0; i<SHIPS_MAX_LENGTH; i++) {
		ShipsNotUsed[i] = 0;
	}
}

bool InitElements::UseShip(int length) {
	if(ShipsNotUsed[length-1] > 0){
		ShipsNotUsed[length-1] --;
		return true; 
	}
	return false;
}
void InitElements::FreeShip(int length) {
	ShipsNotUsed[length - 1] ++;
}

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
	Position pos(LetterToCoordinate(cmd[0]), StringToUNum(num_str) - 1);
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

bool ParseCmdSet(Map &map, string cmd, InitElements &init_data) {
	if (GetParameter(cmd, 1) == "random") {
		map.RandomFill();
		init_data.UseAllShips();
		return true;
	}
	int length = StringToUNum(GetParameter(cmd, 1));
	Position pos = CoordinateParse(GetParameter(cmd, 2));
	Orientation orient = OrientationParse(GetParameter(cmd, 3));

	if (!init_data.UseShip(length)) {
		return false;
	}

	if (!map.AddShip(pos, orient, length)) {
		init_data.FreeShip(length);
		return false;
	}
	return true;
	//return map.AddShip(pos, orient, length);
}

bool ParseCmdDelete(Map &map, string cmd, InitElements &init_data) {
	Position pos = CoordinateParse(GetParameter(cmd, 1));
	if (map.GetMapElement(pos).Battleship == NULL) {
		return false;
	}
	int length = map.GetMapElement(pos).Battleship->GetLength();
	if (map.RemoveShip(pos)) {
		init_data.FreeShip(length);
		return true;
	}
	return false;
}

void ParseCmdClear(Map &map, InitElements &init_data) {
	init_data.SetDefault();
	map.Clear();
}

bool ParseCommandShipInit(Map &map, string cmd, InitElements &init_data) {
	string cmd_name = GetParameter(cmd, 0);

	bool res;
	if (cmd_name == "help") {
		PrintHelp();
		return true;
	}

	if (cmd_name == "set") {
		res = ParseCmdSet(map, cmd, init_data);
		if (res) {
			map.PrintForMe();
		} else {
			cout << "Параметры корабля введены некорректно" << endl;
		}
		return true;
	}

	if (cmd_name == "delete") {
		res = ParseCmdDelete(map, cmd, init_data);
		if (res) {
			map.PrintForMe();
		} else {
			cout << "Неверная позиция" << endl;
		}
		return true;
	}

	if (cmd_name == "clear") {
		ParseCmdClear(map, init_data);
		map.PrintForMe();
		return true;
	}

	if (cmd_name == "exit") {
		cout << "До свидания! :)" << endl;
		return false;
	}

	if (cmd_name == "start") {
		cout << "Начинаем..." << endl;
		//Добавить доп. переменную
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

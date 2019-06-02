#include <iostream>
#include <cstdlib>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"
#include "parser.h"
#include "converter.h"

using namespace std;

ParserStepResult::ParserStepResult() {
	GlobalContinue = true;
	LocalContinue = true;
}

InitElements::InitElements() {
	SetDefault();
}

void InitElements::SetDefault() {
	for(int i=0; i<SHIPS_MAX_LENGTH; i++){
		ShipsNotUsed[i]=SHIPS_MAX_LENGTH-i;
		cout << i << ": " << ShipsNotUsed[i] << endl;
	}
}
void InitElements::UseAllShips() {
	for (int i=0; i<SHIPS_MAX_LENGTH; i++) {
		ShipsNotUsed[i] = 0;
	}
}

bool InitElements::UseShip(int length) {
	// cout << "length = " << length << endl;
	// cout << ShipsNotUsed[0] << endl;
	// cout << ShipsNotUsed[1] << endl;
	// cout << ShipsNotUsed[2] << endl;
	// cout << ShipsNotUsed[3] << endl;
	// cout << "~~~~~~" << endl;
	if(ShipsNotUsed[length-1] > 0){
		ShipsNotUsed[length-1] --;
		return true; 
	}
	return false;
}
void InitElements::FreeShip(int length) {
	ShipsNotUsed[length - 1] ++;
}

bool InitElements::IsAllUsed() {
	for (int i = 0; i < SHIPS_MAX_LENGTH; i++) {
		if (ShipsNotUsed[i] != 0) {
			return false;
		}
	}
	return true;
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
	if (ch < 'a' || ch > 'z') {
		return -1;
	}
	return ch - 'a';
}

Position CoordinateParse(string cmd) {
	if (cmd.empty()) {
		return Position(-1, -1);
	}
	string num_str = cmd.substr(1);
	int i = LetterToCoordinate(cmd[0]);
	if (i == -1) {
		return Position(-1, -1);
	}
	int j = StringToUNum(num_str) - 1;
	return Position(i, j);
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

const string ANGRY_ROMANOV = "Не, ну это катастрофа, бред какой-то, успокойтесь, молодой человек!";
const string STUPID_USER = "Команда введена неверно или данной команды не существует";

void PrintHelpInit() {
	cout << "==================================================================================" << endl;
	cout << "|start — начать игру                                                             |" << endl;
	cout << "|set <length (3)> <position (A2)> <orientation (v/h)> — поместить корабль на поле|" << endl;
	cout << "|    Пример: set 3 A2 v                                                          |" << endl;
	cout << "|set random — случайная расстановка кораблей                                     |" << endl;
	cout << "|delete <position (A2)> — удаление корабля, расположенного на данной клетке      |" << endl;
	cout << "|    Пример: delete A2                                                           |" << endl;
	cout << "|clear — очистить поле                                                           |" << endl;
	cout << "|show — показать поле                                                            |" << endl;
	cout << "|help — показать справку                                                         |" << endl;
	cout << "|exit — выйти                                                                    |" << endl;
	cout << "==================================================================================" << endl << endl;
}
void PrintHelpGameplay() {
	cout << "==================================================================================" << endl;
	cout << "|shoot <position (A2)> — произвести выстрел по заданной координате               |" << endl;
	cout << "|    Пример: shoot A2                                                            |" << endl;
	cout << "|show — показать поле                                                            |" << endl;
	cout << "|help — показать справку                                                         |" << endl;
	cout << "|exit — выйти                                                                    |" << endl;
	cout << "==================================================================================" << endl << endl;
}

bool ParseCmdSet(Map &map, string cmd, InitElements &init_data) {
	if (GetParameter(cmd, 1) == "random") {
	 	map.RandomFill();
		init_data.UseAllShips();
		return true;
	} 
	if (StringToUNum(GetParameter(cmd, 1)) <= 0 || StringToUNum(GetParameter(cmd, 1)) > 4) {
		return false;
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

ParserStepResult ParseCommandShipInit(Map &map, string cmd, InitElements &init_data) {
	string cmd_name = GetParameter(cmd, 0);

	ParserStepResult res;
	if (cmd_name == "help") {
		PrintHelpInit();
		return res;
	}

	if (cmd_name == "set") {
		if (ParseCmdSet(map, cmd, init_data)) {
			map.PrintForMe();
		} else {
			cout << "Параметры корабля введены некорректно" << endl;
		}
		return res;
	}

	if (cmd_name == "delete") {
		if (ParseCmdDelete(map, cmd, init_data)) {
			map.PrintForMe();
		} else {
			cout << "Неверная позиция" << endl;
		}
		return res;
	}
	if (cmd_name == "show") {
		map.PrintForMe();
		return res;
	}

	if (cmd_name == "clear") {
		ParseCmdClear(map, init_data);
		map.PrintForMe();
		return res;
	}

	if (cmd_name == "exit" || cmd_name.size() == 0) {
		if (cmd.size() == 0) {
			cout << "exit" << endl;
		}
		cout << "До свидания! :)" << endl;
		res.GlobalContinue = false;
		res.LocalContinue = false;
		return res;
	}
	if (cmd_name == "Romanoff") {
		cout << ANGRY_ROMANOV <<endl;
		cout << "Выйдите вон из аудитории!!!" << endl;
		res.GlobalContinue = false;
		res.LocalContinue = false;
		return res;
	}

	if (cmd_name == "start") {
		if (!init_data.IsAllUsed()) {
			cout << "Некоторые корабли не установлены" << endl;
			return res;
		}
		res.LocalContinue = false;
		return res;
	}

	cout << STUPID_USER << endl;
	return res;
}
ParserStepResult ParseCommandGameplay(Map &map_me, MapBasic &map_enemy, std::string cmd, int radius){
	string cmd_name = GetParameter(cmd, 0);

	ParserStepResult res;
	if (cmd_name == "help") {
		PrintHelpGameplay();
		return res;
	}

	if (cmd_name == "show") {
		map_me.PrintForMe();
		map_enemy.Print();
		return res;
	}

	if (cmd_name == "shoot") {
		string pos_str = GetParameter(cmd, 1);
		Position pos = CoordinateParse(pos_str);

		res.ShootStatus = map_me.Shoot(pos, radius);
		map_enemy.Import(map_me); 

		map_me.PrintForMe();
		map_enemy.Print();

		bool is_enemy_dead = map_me.IsAllShipsDead(); 

		if (is_enemy_dead) {
			cout << "Враг уничтожен. Игра окончена" << endl;
			res.LocalContinue = false;
		}
		return res;
	}

	if (cmd_name == "exit" || cmd_name.size() == 0) {
		cout << "До свидания! :)" << endl;
		res.GlobalContinue = false;
		res.LocalContinue = false;
		return res;
	}
	if (cmd_name == "Romanoff") {
		cout << ANGRY_ROMANOV <<endl;
		return res;
	}

	cout << STUPID_USER << endl;
	return res;
}
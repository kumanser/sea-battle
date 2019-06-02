#include <iostream>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"
#include "parser.h"
#include "connection.h"
#include "interface.h"
#include "converter.h"

using namespace std;

Interface::Interface() {
	//cout << "INTERFACE CONSTRUCTOR" << endl;
	CurrRadius = 0;
	FirstClientRecv = false;
	InitData.SetDefault();
}

void Interface::Start() {
	cout<<"Рады приветствовать вас в модифицированной игре 'Морской бой'"<<endl;
	cout<<"Прежде чем начать, присоединитесь к другому игроку или создайте игру"<<endl;
	InitDevice();
	InitConnection();

	if (!InitProcess()) {
		return;
	}
	GameplayProcess();
}

void Interface::PrintCmdPrefix() {
	cout << ">>> ";
}

void Interface::PrintMaps() {
	cout << "================================" << endl;
	cout << "Моё поле" << endl;
	MapMe.PrintForMe();
	cout << "Поле соперника" << endl;
	MapEnemy.Print();
}

void Interface::InitDevice() {
	cout << "Выберите нужный вариант:" << endl;
	cout << "(1) Создать игру" << endl;
	cout << "(2) Присоединиться к игре" << endl;

	while (true) {
		string cmd;
		PrintCmdPrefix();
		getline(cin, cmd);
		if (cmd == "1") {
			Network.SetMode(DeviceMode::SERVER);
			break;
		}
		if (cmd == "2") {
			Network.SetMode(DeviceMode::CLIENT);
			FirstClientRecv = true;
			break;
		}
		if (cmd != "1" || cmd != "2") {
			cout << "Параметр выбран неверно"<< endl;
			cout << "Введите '1' , чтобы создать игру." <<endl;
			cout << "Введите '2' , чтобы присоединиться к игре." <<endl;
		}
	}
}

bool Interface::InitConnection() {
	if (Network.GetMode() == DeviceMode::UNDEFINED_MODE) {
		return false;
	}

	while (true) {
		string port;
		string host;
		if (Network.GetMode() == DeviceMode::CLIENT) {
			cout << "Укажите адрес сервера [стандартное значение `localhost`]" << endl;
			PrintCmdPrefix();
			getline(cin,host);
			Network.SetHost(host);
			if (host != "localhost"){
				cout << "Адрес сервера указан неверно" << endl;
				continue;
			}
		}	
		cout << "Укажите порт [стандартное значение `4040`]" << endl;
		PrintCmdPrefix();
		getline(cin,port);
		Network.SetPort(port);
		if (port != "4040"){
			cout << "Порт указан неверно" << endl;
			continue;
		}

		if (Network.Connect()) {
			//cout << "Успешно" << endl;
			break;
		}
		cout << "Ошибка подключения. Попробуйте ещё раз" << endl;
	}

	cout << "Подключение к другому игроку... Ожидайте..." << endl;
	Network.Sync();

	return true;
}

bool Interface::InitProcess() {
	cout << "Установите корабли на поле, воспользовавшись командой set или set random" << endl;
	cout << "Чтобы начать игру, введите start" << endl;
	cout << "Более подробное описание данных команд можно найти в справке, введя 'help'" << endl;
	ParserStepResult res;
	bool first = true;
	while (res.LocalContinue) {
		string cmd;
		PrintCmdPrefix();
		getline(cin, cmd);
		if (cmd == "" && first) {
			getline(cin, cmd);
		}
		
		first = false;
		res = ParseCommandShipInit(MapMe, cmd, InitData);
	}
	if (res.GlobalContinue) {
		cout << "Ожидание другого игрока..." << endl;
		bool isResume = Network.Sync();
		if (isResume) {
			cout << "Начинаем..." << endl;
		} else {
			cout << "Другой игрок вышел из игры. До свидания :)" << endl;
			res.GlobalContinue = false;
		}
		
	} else {
		//cout << "Sending msg" << endl;
		Network.SendMessage(MSG_EXIT);
	}

	return res.GlobalContinue;
}

ShootResult Interface::DrawResult(string ans) {
	string res = GetParameter(ans, 0);
	string map = GetParameter(ans, 1);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			MapEnemy.Matrix[i][j] = map[LinearizePos(Position(i, j))];
		}
	}

	return GetShootResultByAns(res[0]);
}

bool Interface::ActiveMode() {
	while (true) {
		string cmd;
		PrintCmdPrefix();
		getline(cin, cmd);

		cmd = StringToLower(cmd);

		if (GetParameter(cmd, 0) == "shoot") {
			string posStr = GetParameter(cmd, 1);
			string sendCmd = posStr + " ";
			while (sendCmd.size() < MSG_GAMEPLAY_REQ_LEN - NumLength(CurrRadius)) {
				sendCmd += "0";
			}
			sendCmd += UNumToString(CurrRadius);
			
			Network.SendMessage(sendCmd);
			string ans = Network.ReceiveMessage(MSG_GAMEPLAY_ANS_LEN);

			ShootResult res = DrawResult(ans);
			PrintMaps();

			if (res == ShootResult::KILLED) {
				CurrRadius = SHOOT_RADIUS_INCREASED;
				continue;
			}
			if (res == ShootResult::HURT) {
				CurrRadius = SHOOT_RADIUS_DEFAULT;
				continue;
			}
			if (res == ShootResult::FINISHED) {
				cout << "Вы победили! Игра окончена, сессия автоматом!" << endl;
				return false;
			}

			break;
		} else {
			ParserStepResult res;
			res = ParseCommandGameplay(MapMe, MapEnemy, cmd, CurrRadius);
			if (!res.LocalContinue) {
				Network.SendMessage(MSG_EXIT);
				return false;
			}
		}
	}
	Network.SendMessage(MSG_CONFIRM);

	return true;
}

bool Interface::PassiveMode() {
	while (true) {
		string gotCmd = Network.ReceiveMessage(MSG_GAMEPLAY_REQ_LEN);
		if (gotCmd == MSG_EXIT){
			ParserStepResult res;
			res = ParseCommandGameplay(MapMe, MapEnemy, gotCmd, CurrRadius);
			cout << "Другой игрок вышел из игры. До свидания :)" << endl;
			res.GlobalContinue = false;
			res.LocalContinue = false;
			return false;
		}
		string posStr = GetParameter(gotCmd, 0);
		string radiusStr = GetParameter(gotCmd, 1);

		Position pos = CoordinateParse(posStr);
		unsigned int radius = StringToUNum(radiusStr);
		ShootResult res = MapMe.Shoot(pos, radius);
		string sendRes = SetShootResultAns(res);
		MapBasic sendMap;
		sendMap.Import(MapMe);
		string sendMapStr = sendMap.ConvertToArray();
		string sendMsg = sendRes + " " + sendMapStr;

		if (res != ShootResult::INCORRECT) {
			Network.SendMessage(sendMsg);
			PrintMaps();
		}

		if (res == ShootResult::FINISHED) {
			cout << "Вы проиграли, лалка! Игра окончена" << endl;
			return false;
		}
		

		if (res == ShootResult::SLIP/* || res == ShootResult::INCORRECT*/) {
			break;
		}
	}
	Network.Sync();
	Network.ReceiveMessage(MSG_CONFIRM.size());
	return true;
}

bool Interface::GameplayProcess() {
	ParserStepResult res;
	while (res.LocalContinue) {
		if (Network.GetMode() == DeviceMode::CLIENT) {
			if (!ActiveMode()) {
				break;
			}
			if (!PassiveMode()) {
				break;
			}
		} else if (Network.GetMode() == DeviceMode::SERVER) {
			if (!PassiveMode()) {
				break;
			}
			if (!ActiveMode()) {
				break;
			}
		}
	}

	return res.GlobalContinue;
}


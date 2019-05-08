#include <iostream>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"
#include "parser.h"
#include "connection.h"
#include "interface.h"

using namespace std;

Interface::Interface() {
	CurrRadius = 0;
}

void Interface::Start() {
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

void Interface::InitDevice() {
	cout << "Выберите нужный вариант:" << endl;
	cout << "(1) Создать игру" << endl;
	cout << "(2) Присоединиться к игре" << endl;
	
	int selection = 0;
	while (true) {
		PrintCmdPrefix();
		cin >> selection;
		if (selection == 1) {
			Network.SetMode(DeviceMode::SERVER);
			break;
		}
		if (selection == 2) {
			Network.SetMode(DeviceMode::CLIENT);
			break;
		}

		cout << "Параметр выбран неверно" << endl;
	}
}

bool Interface::InitConnection() {
	if (Network.GetMode() == DeviceMode::UNDEFINED_MODE) {
		return false;
	}

	while (true) {
		int port;
		string host;

		if (Network.GetMode() == DeviceMode::CLIENT) {
			cout << "Укажите адрес сервера [стандартное значение `localhost`]" << endl;
			PrintCmdPrefix();
			cin >> host; 
			Network.SetHost(host);
		}
		cout << "Укажите порт [стандартное значение `4040`]" << endl;
		PrintCmdPrefix();
		cin >> port;
		Network.SetPort(port);

		if (Network.Connect()) {
			//cout << "Успешно" << endl;
			break;
		}
		cout << "Ошибка подключения. Попробуйте ещё раз" << endl;
	}

	/*if (Network.GetMode() == DeviceMode::SERVER) {
		string test = Network.ReceiveMessage(19);
		cout << test << endl;
		Network.SendMessage("Success from server");
	} else if (Network.GetMode() == DeviceMode::CLIENT) {
		Network.SendMessage("Success from client");
		string test = Network.ReceiveMessage(19);
		cout << test << endl;
	}*/
	cout << "Подключение к другому игроку... " << endl;
	Network.Sync();
	cout << "Успешно" << endl;

	return true;
}

bool Interface::InitProcess() {
	cout << "Установите корабли на поле" << endl;
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
		Network.Sync();
		cout << "Начинаем..." << endl;
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


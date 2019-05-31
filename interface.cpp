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
	CurrRadius = 0;
	FirstClientRecv = false;
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
	MapMe.PrintForMe();
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
		}
		cout << "Укажите порт [стандартное значение `4040`]" << endl;
		PrintCmdPrefix();
		getline(cin,port);
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
	cout << "Подключение к другому игроку... Ожидайте..." << endl;
	Network.Sync();

	return true;
}

bool Interface::InitProcess() {
	cout << "Установите корабли на поле, воспользовавшись командой set или set random" << endl;
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

/*void Interface::DrawResult(Position pos, string str) {
	int size = 2 * CurrRadius + 1;
	Position origin(pos.X - CurrRadius, pos.Y - CurrRadius);
	int cnt = 0;
	//ShootResult res = ShootResult::INCORRECT;
	for(int i=0; i<size; i++){
		int start;
		int end;
		if (i<size/2){
			start = radius - i;
			end = radius + i;
		} else {
			start = i - radius;
			end = 3*radius - i;
		}
		for(int j=start; j<=end; j++){
			Position curr_pos(origin.X + i, origin.Y + j);
			if (MapMe.IsPosCorrect(curr_pos)) {
				MapEnemy.Matrix[curr_pos.X, curr_pos.Y] = str[cnt];
			}
			cnt++;
			//res = GetMaxShootResult(res, Shoot(curr_pos));
			//A[i][j] = SYMBOL;
		}
	}
}*/

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
		cout << "ACTIVE STEP" << endl;
		string cmd;
		PrintCmdPrefix();
		getline(cin, cmd);

		cmd = StringToLower(cmd);

		if (GetParameter(cmd, 0) == "shoot") {
			string posStr = GetParameter(cmd, 1);
			//string sendCmd = posStr + " " + UNumToString(CurrRadius);
			string sendCmd = posStr + " ";
			cout << "NumLength = " << NumLength(CurrRadius) << endl;
			while (sendCmd.size() < MSG_GAMEPLAY_REQ_LEN - NumLength(CurrRadius)) {
				sendCmd += "0";
			}
			sendCmd += UNumToString(CurrRadius);
			cout << "SendCmd: " << sendCmd << endl;

			//Network.Sync();
			//cout << "ACTIVE SEND" << endl;
			Network.SendMessage(sendCmd);
			//cout << "SENDED MESSAGE: " <<sendCmd<<endl;
			cout << "ACTIVE Receive..." << endl;
			string ans = Network.ReceiveMessage(MSG_GAMEPLAY_ANS_LEN);
			cout << "Received successful" << endl;


			ShootResult res = DrawResult(ans);
			PrintMaps();

			if (res == ShootResult::KILLED) {
				CurrRadius = SHOOT_RADIUS_INCREASED;
				continue;
			}
			if (res == ShootResult::HURT) {
				continue;
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
	cout << "=======================================" << endl;
	cout << "Active sync..." << endl;
	//Network.Sync();
	Network.SendMessage(MSG_CONFIRM);
	cout << "Active end" << endl;

	return true;
}

bool Interface::PassiveMode() {
	while (true) {
		cout << "PASSIVE STEP" << endl;
		string gotCmd = Network.ReceiveMessage(MSG_GAMEPLAY_REQ_LEN);
		cout << "RECEIVED MESSAGE: " << gotCmd <<endl;
		cout << "Received successful" << endl;

		string posStr = GetParameter(gotCmd, 0);
		string radiusStr = GetParameter(gotCmd, 1);
		cout << "POS: " << posStr << endl;

		Position pos = CoordinateParse(posStr);
		unsigned int radius = StringToUNum(radiusStr);
		cout << "RADIUS: " << radius << endl;
		ShootResult res = MapMe.Shoot(pos, radius);
		string sendRes = SetShootResultAns(res);
		MapBasic sendMap;
		sendMap.Import(MapMe);
		string sendMapStr = sendMap.ConvertToArray();
		string sendMsg = sendRes + " " + sendMapStr;

		// cout << "SendMsg:" << endl << sendMsg << endl;
		// cout << "SendRes = " << sendRes << endl;
		// cout << "SendMsg Len = " << sendMsg.size() << endl;

		if (res != ShootResult::INCORRECT) {
			cout << "PASSIVE Send" << endl;
			Network.SendMessage(sendMsg);
			//string Guy = Network.ReceiveMessage(MSG_GAMEPLAY_REQ_LEN);
			//cout << "IGNOR PASSIV KOSTYL Receive: govno = " << Guy <<endl;
		}
		
		//cout << "PNT2" << endl;

		cout << "PASSIVE STEP END" << endl;
		if (res == ShootResult::SLIP/* || res == ShootResult::INCORRECT*/) {
			cout << "PASSIVE BREAK" << endl;
			break;
		}
	}
	cout << "+++++++++++++++++++++++++++++++++" << endl;
	cout << "Passive sync..." << endl;
	//Network.Sync();
	Network.ReceiveMessage(MSG_CONFIRM.size());
	cout << "Passive end" << endl;
	return true;
}

bool Interface::GameplayProcess() {
	ParserStepResult res;
	while (res.LocalContinue) {
		/*string cmd;
		PrintCmdPrefix();
		getline(cin, cmd);
		res = ParseCommandGameplay(MapMe, MapEnemy, cmd, CurrRadius);*/
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


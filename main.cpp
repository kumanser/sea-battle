#include <iostream>
#include <ctime>
#include "interface.h"
using namespace std;
int main()
{
	srand(time(NULL));
	cout<<"Рады приветствовать вас в игре 'Морской бой'"<<endl;
	cout<<"Чтобы получить список доступных команд, введите 'help'"<<endl;
	cout<<"Прежде чем начать игру, установите корабли на поле"<<endl;
	//Map map;
	//Position pos(2, 3);
	//map.AddShip(Position(2, 3), Orientation::VERTICAL, 3);
	//map.AddShip(Position(3, 3), Orientation::HORISONTAL, 3);
	//map.AddShip(Position(0, 0), Orientation::HORISONTAL, 2);
	//map.RandomFill();
	//map.Shoot(0, 0);
	//map.Shoot(0, 1);
	//map.Shoot(1, 1);
	/*cout << "For Me" << endl;
	map.PrintForMe();
	cout << "For Enemy" << endl;
	map.PrintForEnemy();

	cout << "Input position for shoot" << endl;
	//Position pos;
	string cmd1, cmd2;
	//cin >> cmd1;
	getline(cin, cmd1);
	ParseCommandShipInit(&map, cmd1);
	//pos.Input(cin);
	//map.Shoot(CoordinateParse(cmd1), 2);
	cout << "For Me" << endl;
	map.PrintForMe();
	cout << "For Enemy" << endl;
	map.PrintForEnemy();
	getline(cin, cmd2);
	ParseCommandShipInit(&map, cmd2);
	//pos.Input(cin);
	//map.Shoot(CoordinateParse(cmd2), 2);

	cout << "For Me" << endl;
	map.PrintForMe();
	cout << "For Enemy" << endl;
	map.PrintForEnemy();*/

	Interface interface;
	interface.Start();

	return 0;
}
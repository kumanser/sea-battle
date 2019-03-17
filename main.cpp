#include <iostream>
#include <ctime>
#include "map.h"
using namespace std;
int main()
{
	srand(time(NULL));
	Map map;
	//Position pos(2, 3);
	map.AddShip(Position(2, 3), Orientation::VERTICAL, 3);
	map.AddShip(Position(3, 3), Orientation::HORISONTAL, 3);
	map.AddShip(Position(0, 0), Orientation::HORISONTAL, 2);
	//map.RandomFill();
	//map.Shoot(0, 0);
	//map.Shoot(0, 1);
	//map.Shoot(1, 1);
	cout << "For Me" << endl;
	map.PrintForMe();
	cout << "For Enemy" << endl;
	map.PrintForEnemy();

	cout << "Input position for shoot" << endl;
	Position pos;
	pos.Input(cin);
	map.Shoot(pos);
	pos.Input(cin);
	map.Shoot(pos);

	cout << "For Me" << endl;
	map.PrintForMe();
	cout << "For Enemy" << endl;
	map.PrintForEnemy();
	return 0;
}
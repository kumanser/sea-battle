#include <iostream>
#include "map.h"
#include "geometry.h"

using namespace std;

int NumCount(int num){
	int cnt=0;
	while(num != 0){
		num/=10;
		cnt++;
	}
	return cnt;
}

Ship::Ship(Map *battlefield, int length){
	Battlefield = battlefield;
	Length = length;
	IsInit = false;
}

bool Ship::Init(Position pos, Orientation orient){
	Pos=pos;
	Orient=orient;
	IsInit = true;
	return Battlefield->DrawShip(this);
}

Position Ship::GetPosition() {
	return Pos;
}
Orientation Ship::GetOrientation() {
	return Orient;
}
int Ship::GetLength() {
	return Length;
}

Map::Map(){
	for(int i=0; i<MAP_HEIGHT; i++){
		for(int j=0; j<MAP_WIDTH; j++){
			Matrix[i][j]=MAP_ELEMENT_EMPTY;
		}
	}
	//Строчки ниже для тестирования. Потом удалить
	//Matrix[0][1] = MAP_ELEMENT_UNHARMED;
	//Matrix[1][1] = MAP_ELEMENT_UNHARMED;

	/*for (int current_length = 1; current_length <= SHIPS_MAX_LENGTH; current_length++) {
		int ships_count = SHIPS_MAX_LENGTH - current_length + 1;
		for (int i = 0; i < ships_count; i++) {
			Ship new_ship(this, current_length);
			ShipsList.push_back(new_ship);
		}
	}*/
	Ship ship(this, 4);
	ship.Init(Position(1, 1), Orientation::HORISONTAL);

}
void Map::PrintForEnemy(){
	int max_num_length = NumCount(MAP_HEIGHT);
	for (int i = 0; i < max_num_length; i++) {
		cout << MAP_ELEMENT_SPACE;
	}
	cout << MAP_ELEMENT_SPACE;
	for(int i=0;i<MAP_WIDTH;i++){
		cout<< (char)('A'+i)<< MAP_ELEMENT_SPACE;
	}
	cout<<endl;
	for(int i=0; i<MAP_HEIGHT; i++){
		int current_num_length = NumCount(i+1);
		cout << 1 + i << MAP_ELEMENT_SPACE;
		for(int j=0; j<max_num_length-current_num_length; j++){
			cout<<MAP_ELEMENT_SPACE;
		} 
		for(int j=0; j<MAP_WIDTH; j++){
			char current = Matrix[i][j];
			if (current==MAP_ELEMENT_UNHARMED){
				current=MAP_ELEMENT_EMPTY;
			}
			cout<<current<<MAP_ELEMENT_SPACE;
		}
		cout<<endl;
	}
	cout << endl;
}
void Map::PrintForMe(){
	int max_num_length = NumCount(MAP_HEIGHT);
	for (int i = 0; i < max_num_length; i++) {
		cout << MAP_ELEMENT_SPACE;
	}
	cout << MAP_ELEMENT_SPACE;
	for(int i=0;i<MAP_WIDTH;i++){
		cout<< (char)('A'+i)<< MAP_ELEMENT_SPACE;
	}
	cout<<endl;
	for(int i=0; i<MAP_HEIGHT; i++){
		int current_num_length = NumCount(i+1);
		cout << 1 + i << MAP_ELEMENT_SPACE;
		for(int j=0; j<max_num_length-current_num_length; j++){
			cout<<MAP_ELEMENT_SPACE;
		} 
		for(int j=0; j<MAP_WIDTH; j++){
			cout<<Matrix[i][j]<<MAP_ELEMENT_SPACE;
		}
		cout<<endl;
	}
	cout << endl;
}
void Map::Shoot(int x, int y){
	if(Matrix[x][y]==MAP_ELEMENT_UNHARMED){
		Matrix[x][y]=MAP_ELEMENT_DAMAGED;
	} else if (Matrix[x][y]==MAP_ELEMENT_EMPTY) {
		Matrix[x][y]=MAP_ELEMENT_SLIP;
	}
}
bool Map::CheckPosition(Ship *ship){
	if (ship->GetOrientation()==Orientation::VERTICAL){
		if (MAP_HEIGHT-ship->GetPosition().Y < ship->GetLength()){
			return false;
		}
	} else {
		if (MAP_WIDTH-ship->GetPosition().X < ship->GetLength()){
			return false;
		}
	}
	Position pos_begin(ship->GetPosition().X - 1, ship->GetPosition().Y - 1);
	Position pos_end;
	if (ship->GetOrientation()==Orientation::VERTICAL){
		pos_end.X=pos_begin.X+2;
		pos_end.Y=pos_begin.Y + ship->GetLength() + 1;
	} else {
		pos_end.X=pos_begin.X + ship->GetLength() + 1;
		pos_end.Y=pos_begin.Y+2;
	}

	for(int i=pos_begin.X; i <= pos_end.X; i++){
		for(int j=pos_begin.Y; j <= pos_end.Y; j++){
			Position current_pos(i, j);
			if (!IsPosCorrect(current_pos)) {
				continue;
			}
			if (Matrix[current_pos.X][current_pos.Y]==MAP_ELEMENT_UNHARMED) {
				return false;
			}
		}
	}
	return true;
}
bool Map::IsPosCorrect(Position pos){
	if (pos.X >= 0 && pos.Y >= 0 && pos.X < MAP_WIDTH && pos.Y < MAP_HEIGHT){
		return true;
	}
	return false;
}
bool Map::DrawShip(Ship *ship){
	if (!CheckPosition(ship)){
		return false;
	}
	Position current_pos=ship->GetPosition();
	for(int i=0; i<ship->GetLength();i++){
		Matrix[current_pos.X][current_pos.Y]=MAP_ELEMENT_UNHARMED;
		if (ship->GetOrientation()==Orientation::VERTICAL){
			current_pos.Y++;
		} else {
			current_pos.X++;
		}
	}
	return true;
}
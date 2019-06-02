#include <iostream>
#include "environment.h"
#include "map.h"
#include "geometry.h"

using namespace std;

ShootResult GetMaxShootResult(ShootResult a, ShootResult b) {
	return (a > b ? a : b);
}

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
	Damage = 0;
}

bool Ship::Init(Position pos, Orientation orient){
	Pos=pos;
	Orient=orient;
	IsInit = Battlefield->DrawShip(this);
	return IsInit;
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
int Ship::GetDamage(){
	return Damage;
}
bool Ship::IsDead(){
	return Damage >= Length;
}
void Ship::DrawCircuit() {
	Position pos_begin(GetPosition().X - 1, GetPosition().Y - 1);
	Position pos_end;
	if (GetOrientation()==Orientation::VERTICAL){
		pos_end.X=pos_begin.X+2;
		pos_end.Y=pos_begin.Y + GetLength() + 1;
	} else {
		pos_end.X=pos_begin.X + GetLength() + 1;
		pos_end.Y=pos_begin.Y+2;
	}

	for(int i=pos_begin.Y; i <= pos_end.Y; i++){
		for(int j=pos_begin.X; j <= pos_end.X; j++){
			Position current_pos(j, i);
			if (!Battlefield->IsPosCorrect(current_pos)) {
				continue;
			}
			if (Battlefield->Matrix[current_pos.Y][current_pos.X].Sign==MAP_ELEMENT_EMPTY) {
				Battlefield->Matrix[current_pos.Y][current_pos.X].Sign=MAP_ELEMENT_SLIP;
			}
		}
	}
}
bool Ship::Harm(){
	Damage++;
	//cout << "Damage = " << Damage << endl;
	bool is_dead = IsDead();
	//cout << "IsDead = " << (is_dead ? "true" : "false") << endl;
	if (is_dead) {
		DrawCircuit();
	}
	return is_dead;
}

int LinearizePos(Position pos) {
	return pos.X * MAP_WIDTH + pos.Y;
}

Map::Map(){
	for(int i=0; i<MAP_HEIGHT; i++){
		for(int j=0; j<MAP_WIDTH; j++){
			Matrix[i][j].Sign=MAP_ELEMENT_EMPTY;
			Matrix[i][j].Battleship = NULL;
		}
	}
}
void Map::ClearShipsList() {
	for (int current_length = 1; current_length <= SHIPS_MAX_LENGTH; current_length++) {
		for (int i = 0; i < ShipsList[current_length - 1].size(); i++) {
			if (ShipsList[current_length - 1][i] != NULL) {
				delete ShipsList[current_length - 1][i];
				ShipsList[current_length - 1][i] = NULL;
			}
		}
		ShipsList[current_length - 1].clear();
	}
}
void Map::ClearMatrix() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			Matrix[i][j].Sign = MAP_ELEMENT_EMPTY;
			Matrix[i][j].Battleship = NULL;
		}
	}
}
Map::~Map() {
	ClearShipsList();
}
void Map::Clear() {
	ClearMatrix();
	ClearShipsList();
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
			char current = Matrix[i][j].Sign;
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
			cout<<Matrix[i][j].Sign<<MAP_ELEMENT_SPACE;
		}
		cout<<endl;
	}
	cout << endl;
}
ShootResult Map::Shoot(Position pos){
	//curr_pos.Print();
	if (!IsPosCorrect(pos)) {
		return ShootResult::INCORRECT;
	}
	if(Matrix[pos.Y][pos.X].Sign==MAP_ELEMENT_UNHARMED){
		Matrix[pos.Y][pos.X].Sign=MAP_ELEMENT_DAMAGED;
		if (Matrix[pos.Y][pos.X].Battleship->Harm()) {
			return ShootResult::KILLED;
		}
		return ShootResult::HURT;
		//cout << " ::TRUE";

	} else if (Matrix[pos.Y][pos.X].Sign==MAP_ELEMENT_EMPTY) {
		Matrix[pos.Y][pos.X].Sign=MAP_ELEMENT_SLIP;
		
	}
	return ShootResult::SLIP;
}
ShootResult Map::Shoot(Position pos, int radius) {
	//Shoot(pos);
	int size = 2 * radius + 1;
	Position origin(pos.X - radius, pos.Y - radius);

	ShootResult res = ShootResult::INCORRECT;
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
			res = GetMaxShootResult(res, Shoot(curr_pos));
			//A[i][j] = SYMBOL;
		}
	}

	if (IsAllShipsDead()) {
		res = ShootResult::FINISHED;
	}

	return res;
}
bool Map::CheckPosition(Ship *ship){
	//Проверка, умещаются ли корабли в поле
	if (ship->GetOrientation()==Orientation::VERTICAL){
		if (MAP_HEIGHT-ship->GetPosition().Y < ship->GetLength()){
			return false;
		}
	} else {
		if (MAP_WIDTH-ship->GetPosition().X < ship->GetLength()){
			return false;
		}
	}

	//Проверка на соприкосновение с другими кораблями
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
			if (Matrix[current_pos.Y][current_pos.X].Sign==MAP_ELEMENT_UNHARMED) {
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
		Matrix[current_pos.Y][current_pos.X].Sign=MAP_ELEMENT_UNHARMED;
		Matrix[current_pos.Y][current_pos.X].Battleship = ship;
		if (ship->GetOrientation()==Orientation::VERTICAL){
			current_pos.Y++;
		} else {
			current_pos.X++;
		}
	}
	return true;
}
bool Map::AddShip(Position pos, Orientation orient, int length) {
	Ship *new_ship = new Ship(this, length);
	bool is_init = new_ship->Init(pos,orient);
	if (is_init){
		ShipsList[length - 1].push_back(new_ship);
	} else {
		delete new_ship;
	}
	return is_init;
}
bool Map::RemoveShip(Position pos) {
	//Стереть его с карты
	if (Matrix[pos.Y][pos.X].Battleship == NULL) {
		return false;
	}
	Ship *ship = Matrix[pos.Y][pos.X].Battleship;
	Position curr_pos;
	curr_pos = ship->GetPosition();
	for (int i = 0; i < ship->GetLength(); i++) {
		Matrix[curr_pos.Y][curr_pos.X].Sign = MAP_ELEMENT_EMPTY;
		Matrix[curr_pos.Y][curr_pos.X].Battleship = NULL;

		if (ship->GetOrientation() == Orientation::VERTICAL) {
			curr_pos.Y++;
		} else {
			curr_pos.X++;
		}
	}
	//Удалить его из ShipsList
	int found_el;
	for(int i=0; i<ShipsList[ship->GetLength()].size(); i++){
		if(ShipsList[ship->GetLength()][i]==ship){
			found_el=i;
			break;
		}
	}
	for(int i=found_el+1; i<ShipsList[ship->GetLength()].size(); i++){
		ShipsList[ship->GetLength()][i-1]=ShipsList[ship->GetLength()][i];

	}
	ShipsList[ship->GetLength()].resize(ShipsList[ship->GetLength()].size() - 1);

	delete ship;

	return true;
}
void Map::RandomFill(){
	Clear();
	for (int current_length = 1; current_length <= SHIPS_MAX_LENGTH; current_length++) {
		int ships_count = SHIPS_MAX_LENGTH - current_length + 1;
		for (int i = 0; i < ships_count; i++) {
			//Ship new_ship(this, current_length);
			Ship *new_ship = new Ship(this, current_length);
			Orientation tmp_orient;
			Position tmp_pos;
			do {
				tmp_orient = GetRandomOrientation();
				tmp_pos.SetRandom();
			} while (!new_ship->Init(tmp_pos,tmp_orient));
			ShipsList[current_length - 1].push_back(new_ship);
		}
	}
}

bool Map::IsAllShipsDead() {
	for(int i=0; i<SHIPS_MAX_LENGTH; i++){
		for(int j=0; j<ShipsList[i].size(); j++){
			if (!ShipsList[i][j]->IsDead()) {
				return false;
			}
		}
	}
	return true;
}


MapField Map::GetMapElement(int i, int j) {
	return Matrix[i][j];
}
MapField Map::GetMapElement(Position pos) {
	return Matrix[pos.Y][pos.X];
}


MapBasic::MapBasic() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			Matrix[i][j] = MAP_ELEMENT_EMPTY;
		}
	}
}

void MapBasic::Import(Map &map) {
	for(int i=0; i < MAP_HEIGHT; i++ ){
		for(int j=0; j < MAP_WIDTH; j++){
			Matrix[i][j] = (map.GetMapElement(i, j).Sign == MAP_ELEMENT_UNHARMED ? MAP_ELEMENT_EMPTY : map.GetMapElement(i, j).Sign);
		}
	}
}
void MapBasic::Print() {
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

string MapBasic::ConvertToArray() {
	string res;
	res.resize(MAP_HEIGHT * MAP_WIDTH); //На случай ошибки в функции LinearizePos: в таком случае всё равно будет работать
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			//res += Matrix[i][j];
			res[LinearizePos(Position(i, j))] = Matrix[i][j];
		}
	}
	return res;
}

ShootResult GetShootResultByAns(char ans) {
	if (ans == '0') {
		return ShootResult::SLIP;
	}
	if (ans == '1') {
		return ShootResult::HURT;
	}
	if (ans == '2') {
		return ShootResult::KILLED;
	}
	if (ans == '3') {
		return ShootResult::FINISHED;
	}

	return ShootResult::INCORRECT;
}

string SetShootResultAns(ShootResult res) {
	if (res == ShootResult::SLIP) {
		return "0";
	}
	if (res == ShootResult::HURT) {
		return "1";
	}
	if (res == ShootResult::KILLED) {
		return "2";
	}
	if (res == ShootResult::FINISHED) {
		return "3";
	}

	return "0";
}

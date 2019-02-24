#include <iostream>
#include "map.h"

using namespace std;

int NumCount(int num){
	int cnt=0;
	while(num != 0){
		num/=10;
		cnt++;
	}
	return cnt;
}


Map::Map(){
	for(int i=0; i<MAP_HEIGHT; i++){
		for(int j=0; j<MAP_WIDTH; j++){
			Matrix[i][j]=MAP_ELEMENT_EMPTY;
		}
	}
}
	
void Map::Print(){
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
	Matrix[x][y]=MAP_ELEMENT_DAMAGED;
}

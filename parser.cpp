#include <iostream>
#include <cstdlib>
#include <string>
#include "environment.h"
#include "geometry.h"
#include "map.h"
#include "parser.h"
#include "converter.h"

using namespace std;

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
	Position pos(StringToUNum(num_str) - 1, LetterToCoordinate(cmd[0]));
	return pos;
}

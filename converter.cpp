#include "converter.h"

#include <iostream>
#include <string>

using namespace std;

bool IsSeparator(char sym) {
	switch (sym) {
		case '\t':
		case '\n':
		case '\v':
		case '\f':
		case '\r':
		case ' ':
		case '\0':
			return true;
		default:
			return false;
	}
}
string StringReverse(std::string str) {
	string new_str = "";
	for (unsigned int i = str.size(); i > 0; i--) {
		new_str += str[i - 1];
	}
	return new_str;
}

char CharToLower(char sym) {
	int diff = 'a' - 'A';
	if (sym >= 'A' && sym <= 'Z') {
		sym += diff;
	}
	return (char) sym;
}

string StringToLower(string str) {
	for (unsigned int i = 0; i < str.size(); i++) {
		str[i] = CharToLower(str[i]);
	}
	return str;
}

string RemoveSpecial(string str) {
	string new_str = "";
	bool is_first_space = true;
	for (unsigned int i = 0; i < str.size(); i++) {
		if (!IsSeparator(str[i])) {
			is_first_space = true;
		}
		bool is_set_space = false;
		if (str[i] == ' ' && is_first_space) {
			is_set_space = true;
			is_first_space = false;
		}

		if (!IsSeparator(str[i]) || is_set_space) {
			new_str += str[i];
		}
	}
	return new_str;
}

bool IsCharNum(char sym) {
	return sym >= '0' && sym <= '9';
}
bool IsStringNum(string str) {
	for (unsigned int i = 0; i < str.size(); i++) {
		if (!IsCharNum(str[i])) {
			return false;
		}
	}
	return true;
}

char UNumToChar(unsigned int num) {
	return num + '0';
}
string UNumToString(unsigned int num) {
	if (num == 0) {
		return "0";
	}
	string str = "";
	while (num > 0) {
		unsigned int md = num % 10;
		num /= 10;
		str += UNumToChar(md);
	}
	return StringReverse(str);
}
unsigned int CharToUNum(char sym) {
	if (IsCharNum(sym)) {
		return sym - '0';
	}
	return 0;
}
unsigned int StringToUNum(string str) {
	unsigned int res = 0;
	for (unsigned int i = 0; i < str.size(); i++) {
		res *= 10;
		res += CharToUNum(str[i]);
	}
	return res;
}

unsigned int NumLength(int num) {
	unsigned int cnt = 0;

	while (num > 0) {
		cnt++;
		num /= 10;
	}
	return cnt;
}

/*unsigned int GetElementCountByRadius(unsigned int radius) {
	unsigned int size = 2 * radius + 1;
	unsigned int sum = 0;
	for (unsigned int i = 1; i <= size; i += 2) {
		if (i != size) {
			sum += 2 * i;
		} else {
			sum += i;
		}
	}
	return sum;
}*/


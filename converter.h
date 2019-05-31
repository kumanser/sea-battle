#ifndef CONVERTER_H
#define CONVERTER_H

#include <iostream>
#include <string>

bool IsSeparator(char sym);
std::string StringReverse(std::string str);
char CharToLower(char sym);
std::string StringToLower(std::string str);
std::string RemoveSpecial(std::string str);
bool IsCharNum(char sym);
bool IsStringNum(std::string str);
char UNumToChar(unsigned int num);
std::string UNumToString(unsigned int num);
unsigned int CharToUNum(char sym);
unsigned int StringToUNum(std::string str);
unsigned int NumLength(int num);

//unsigned int GetElementCountByRadius(unsigned int radius);

#endif // CONVERTER_H
#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <string>

const char MAP_WIDTH = 10;
const char MAP_HEIGHT = MAP_WIDTH;
const char MAP_ELEMENT_EMPTY = '.';
const char MAP_ELEMENT_SPACE = ' ';
const char MAP_ELEMENT_DAMAGED='X';
const char MAP_ELEMENT_SLIP='o';
const char MAP_ELEMENT_UNHARMED='#';
const int SHIPS_MAX_LENGTH = 4;
const std::string MSG_CONFIRM = "m_cnf";
const std::string MSG_EXIT = "m_ext";

const int MSG_GAMEPLAY_REQ_LEN = 5;
const int MSG_GAMEPLAY_ANS_LEN = MAP_WIDTH * MAP_HEIGHT + 2;

const std::string MSG_GAMEPLAY_ANS_SLIP = "0";
const std::string MSG_GAMEPLAY_ANS_DAMAGED = "1";
const std::string MSG_GAMEPLAY_ANS_KILLED = "2";

const int SHOOT_RADIUS_DEFAULT = 0;
const int SHOOT_RADIUS_INCREASED = 2;

#endif

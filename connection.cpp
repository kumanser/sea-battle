#include <iostream>
#include <string>
#include <cstring>
#include "environment.h"
#include "geometry.h"
#include "connection.h"
#include "converter.h"
#include "zmq.h"

using namespace std;

Connection::Connection() {
	Mode = DeviceMode::UNDEFINED_MODE;
	Host = "";
	Port = -1;
	Connected = false;
	ZmqContext = NULL;
	ZmqSocket = NULL;
}
Connection::Connection(DeviceMode mode, std::string host, std::string port) {
	SetMode(mode);
	SetHost(host);
	SetPort(port);
	Connected = false;
	ZmqContext = NULL;
	ZmqSocket = NULL;
}

Connection::~Connection() {
	Disconnect();
}

void Connection::SetHost(std::string host) {
	Host = host;
}

void Connection::SetPort(std::string port) {
	Port = port;
}

void Connection::SetMode(DeviceMode mode) {
	Mode = mode;
}

bool Connection::Connect() {
	if (Mode == DeviceMode::UNDEFINED_MODE) {
		return false;
	}

	ZmqContext = zmq_ctx_new();

	if (Mode == DeviceMode::SERVER) {
		ZmqSocket = zmq_socket(ZmqContext, ZMQ_REP);
		string addr = "tcp://*:" + Port;
		Connected = (zmq_bind(ZmqSocket, addr.c_str()) == 0);
	} else {
		ZmqSocket = zmq_socket(ZmqContext, ZMQ_REQ);
		string addr = "tcp://" + Host + ":" + Port;
		Connected = (zmq_connect(ZmqSocket, addr.c_str()) == 0);
	}

	return Connected;
}

void Connection::Disconnect() {
	zmq_close(ZmqSocket);
	zmq_ctx_destroy(ZmqContext);

	ZmqSocket = NULL;
	ZmqContext = NULL;
	Connected = false;
}

bool Connection::IsConnected() {
	return Connected;
}

DeviceMode Connection::GetMode() {
	return Mode;
}

string Connection::GetHost() {
	return Host;
}

string Connection::GetPort() {
	return Port;
}

void Connection::SendMessage(string msg) {
	zmq_msg_t zmq_message;
	zmq_msg_init_size(&zmq_message, msg.size());
	memcpy(zmq_msg_data(&zmq_message), msg.c_str(), msg.size());
	zmq_msg_send(&zmq_message, ZmqSocket, 0);
	zmq_msg_close(&zmq_message);
}

string Connection::ReceiveMessage(unsigned int length) {
	zmq_msg_t zmq_message;
	zmq_msg_init(&zmq_message);
	zmq_msg_recv(&zmq_message, ZmqSocket, 0);

	//cout << "PNT0" << endl;
	char *tmp_arr = new char[length];
	//cout << "PNT1" << endl;
	memcpy(tmp_arr, zmq_msg_data(&zmq_message), length);
	//string res(tmp_arr);
	//cout << "PNT2" << endl;
	string res = "";
	for (int i = 0; i < length; i++) {
		res += tmp_arr[i];
	}
	//cout << "PNT3" << endl;
	cout << "len = " << length << endl;
	cout << "msg = " << res << endl;
	//cout << "PNT4" << endl;
	delete [] tmp_arr;
	//cout << "PNT5" << endl;
	zmq_msg_close(&zmq_message);
	cout << res << endl;

	return res;

}

bool Connection::Sync() {
	if (GetMode() == DeviceMode::SERVER) {
		string msg = ReceiveMessage(MSG_CONFIRM.size());
		if (msg == MSG_EXIT) {
			return false;
		}
		//cout << "out = " << test << endl;
		SendMessage(MSG_CONFIRM);
	} else if (GetMode() == DeviceMode::CLIENT) {
		SendMessage(MSG_CONFIRM);
		//cout << "MSG_SENDED" << endl;
		string msg = ReceiveMessage(MSG_CONFIRM.size());
		if (msg == MSG_EXIT) {
			return false;
		}
		//cout << "out = " << test << endl;
	}
	return true;
}

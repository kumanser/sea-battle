#ifndef CONNECTION_H
#define CONNECTION_H

#include "environment.h"
#include "geometry.h"
#include <iostream>
#include <string>

enum DeviceMode {
	SERVER,
	CLIENT,
	UNDEFINED_MODE
};

class Connection {
	bool Connected;
	DeviceMode Mode;
	std::string Host;
	int Port;

	void *ZmqContext;
	void *ZmqSocket;

public:
	Connection();
	Connection(DeviceMode mode, std::string host, int port);
	~Connection();

	void SetHost(std::string host);
	void SetPort(int port);
	void SetMode(DeviceMode mode);
	bool Connect();
	void Disconnect();
	bool IsConnected();

	void SendMessage(std::string msg);
	std::string ReceiveMessage(unsigned int length);

	void Sync();


	DeviceMode GetMode();
	std::string GetHost();
	int GetPort();
};

#endif

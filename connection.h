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
	std::string Port;

	void *ZmqContext;
	void *ZmqSocket;

public:
	Connection();
	Connection(DeviceMode mode, std::string host, std::string port);
	~Connection();

	void SetHost(std::string host);
	void SetPort(std::string port);
	void SetMode(DeviceMode mode);
	bool Connect();
	void Disconnect();
	bool IsConnected();

	void SendMessage(std::string msg);
	std::string ReceiveMessage(unsigned int length);

	bool Sync();

	void *GetSocket();


	DeviceMode GetMode();
	std::string GetHost();
	std::string GetPort();
};

#endif

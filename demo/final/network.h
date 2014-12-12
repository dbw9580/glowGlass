#include<Windows.h>
#include<iostream>
#include"config.h"
#pragma comment(lib,"ws2_32.lib")

class Network
{
public:
	Network(SocketConfig& conf);
	~Network();
	bool init();
	bool startListen();
	int write(const char* wbuf, unsigned int length);
	int read();
	bool close();

	std::string wbuf;
	std::string rbuf;
private:
	
	SOCKET conn, server;
	SocketConfig config;
};
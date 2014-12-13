#include<Windows.h>
#include<iostream>
#include"OnlyMyConfig.h"
#pragma comment(lib,"ws2_32.lib")

//using OnlyMyConfig::Config;
//using OnlyMyConfig::COMConfig;
//using OnlyMyConfig::StyleConfig;
//using OnlyMyConfig::SocketConfig;

class Network
{
public:
	Network(SocketConfig& conf);
	~Network();
	bool init();
	bool startListen();
	//int write(const char* wbuf, unsigned int length);
	int read();
	bool close();

	std::string wbuf;
	std::string rbuf;
private:
	
	SOCKET conn, server;
	SocketConfig config;
	char* readBuffer = NULL;
};
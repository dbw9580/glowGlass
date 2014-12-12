#include<windows.h>
#pragma comment(lib,"ws2_32.lib")
#include<iostream>
#include<string>
#include<fstream>
#include"json/json.h"
#pragma comment(lib,"lib_json.lib")
#include "SerialPort.h"
#include<map>
#include"config.h"
using namespace std;

int loadConfig(const char* filepath, Config& config);
int initComm(COMConfig& conf, CSerialPort& sp);
int initSocket(SocketConfig& conf);
int main()
{
	char filepath[20];
	Config config;
	cin >> filepath;
	if (loadConfig(filepath, config) == 0)
	{
		CSerialPort sp;
		if (initComm(config.com, sp) == 0)
		{
			std::cout << "COM" << config.com.num << "opened!" << std::endl;

		}
		else
		{
			std::cerr << "failed to open COM" << config.com.num << std::endl;
			return -2;
		}
	}
	else
	{
		return -1;
	}
	system("pause");
	
}

int loadConfig(const char* filepath, Config& config)
{
	std::ifstream confFile;
	confFile.open(filepath);
	if (!confFile.is_open())
	{
		cerr << "could not open config file" << endl;
		return -1;
	}
	else
	{
		cout << "config file opened!" << endl;
	}

	confFile.seekg(0, ios::end);
	int fileLen = confFile.tellg();
	char *confStr = NULL;
	confStr = new char[fileLen + 1];
	confFile.seekg(0, ios::beg);
	confFile >> confStr;
	confStr[fileLen] = 0;

	Json::Reader reader;
	Json::Value confRoot;

	if (!reader.parse(confStr, confRoot))
	{
		cerr << "error parsing config file!" << endl << reader.getFormattedErrorMessages() << endl;
		return -2;
	}
	else
	{
		config.com.num = confRoot["COM"]["num"].asInt();
		config.com.baud = confRoot["COM"]["baud"].asInt();
		config.sock.port = confRoot["socket"]["port"].asInt();
		config.sock.rbuff = confRoot["socket"]["read_buffer"].asInt();
		config.sock.wbuff = confRoot["socket"]["send_buffer"].asInt();

		Json::Value::Members styleNames = confRoot["style"].getMemberNames();
		for (Json::Value::Members::iterator itr = styleNames.begin(); itr != styleNames.end();itr++)
		{
			config.style.styles.insert(map<string, string>::value_type(*itr, confRoot["style"][*itr].asString()));
		}
	}
	confFile.close();
	
	delete[] confStr;
	
	return 0;
	
}

int initComm(COMConfig& conf, CSerialPort& sp)
{
	if (!sp.InitPort(conf.num, conf.baud))
	{
		return -1;
	}

	return 0;
}

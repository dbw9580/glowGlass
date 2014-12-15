#pragma once

#include"json/json.h"
#include"network.h"
#include "SerialPort.h"
#include<windows.h>
#pragma comment(lib,"ws2_32.lib")
#include<iostream>
#include<string>
#include<fstream>
#pragma comment(lib,"lib_json.lib")
#include<map>


using namespace std;
//using OnlyMyConfig::Config;
//using OnlyMyConfig::COMConfig;
//using OnlyMyConfig::StyleConfig;
//using OnlyMyConfig::SocketConfig;

int loadConfig(const char* filepath, Config& config);
int initComm(COMConfig& conf, CSerialPort& sp);
string parseCommand(const string& cmd, const StyleConfig& style);
inline void CSreplace(std::string& s1, std::string& s2, std::string& s3, int count);
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
			Network net(config.sock);
			if (net.init())
			{
				while (1)
				{
					net.startListen();
					while (net.read() != 0)
					{
						string res = parseCommand(net.rbuf, config.style);
						sp.WriteData((unsigned char*)res.c_str(), res.length());
						sp.WriteData((unsigned char*)"\r\n");
						cout << res << endl;
					}

				}
			}

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
		cerr << "could not open config file: " << filepath << endl;
		return -1;
	}
	else
	{
		cout << "config file opened" << endl;
	}

	confFile.seekg(0, ios::end);
	int fileLen = confFile.tellg();
	char *confStr = NULL;
	confStr = new char[fileLen+1];
	confFile.seekg(0, ios::beg);
	confFile.get(confStr, fileLen+1);
	//confStr[fileLen] = 0;

	Json::Reader reader;
	Json::Value confRoot;

	if (!reader.parse(confStr, confRoot))
	{
		cerr << "error parsing config file: " << endl << reader.getFormattedErrorMessages() << endl;
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

string parseCommand(const string& cmd, const StyleConfig& style)
{
	Json::Reader reader;
	Json::Value root;

	std::ifstream styleFile;
	if (reader.parse(cmd, root))
	{
		string styleType = root["message"]["styleID"].asString();
		map<string, string>::const_iterator itr = style.styles.find(styleType);
		if (itr != style.styles.end())
		{
			styleFile.open(itr->second);
			if (styleFile.good())
			{
				styleFile.seekg(0, ios::end);
				int fileLen = styleFile.tellg();
				char* inBuf = new char[fileLen + 1];
				styleFile.seekg(0, ios::beg);
				styleFile.get(inBuf, fileLen +1);
				memset(inBuf + fileLen, 0, sizeof(char));
				styleFile.close();
				string resStr(inBuf);

				Json::Value text = root["message"]["text"];
				if (root["message"]["text"].isArray())
				{
					for (unsigned int i = 0; i < text.size(); i++)
					{
						CSreplace(resStr, string("%s"), text[i].asString(), 1);
					}
				}
				else
				{
					CSreplace(resStr, string("%s"), text.asString(), -1);
				}
				
				

				delete[] inBuf;
				return resStr;
			}
			else
			{
				cerr << "failed to open style definition: " << itr->second << endl;
			}
		}
		else
		{
			cerr << "undefined style ID: " << itr->first << endl;
		}
	}
	else
	{
		cerr << "failed to parse command, check your syntax: " << cmd << reader.getFormattedErrorMessages() << endl;
	}
	
	return string("");
}
inline void CSreplace(std::string& s1, std::string& s2, std::string& s3, int count)
{
	std::string::size_type pos = 0;
	std::string::size_type a = s2.size();
	std::string::size_type b = s3.size();

	if (count == -1){
		while ((pos = s1.find(s2, pos)) != std::string::npos)
		{
			s1.replace(pos, a, s3);
			pos += b;
		}
	}
	else{
		int c = 0;
		while ((pos = s1.find(s2, pos)) != std::string::npos)
		{
			s1.replace(pos, a, s3);
			pos += b;
			if (++c == count)
				return;
		}
	}
}
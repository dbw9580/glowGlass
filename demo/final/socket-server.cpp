//#include <WINSOCK2.H>
#include<windows.h>
#include <stdio.h>
#include<iostream>
#include "json/json.h"
#include "SerialPort.h"
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "lib_json.lib")
#define RECV_BUF_LEN 200
#define SERIAL_COM 7
#define SERIAL_BAUD 115200

using namespace std;

int main()
{
	/*JSON 翻译*/
	Json::Reader jsonReader;
	Json::Value root;

	/*打开串口*/
	CSerialPort sp;
	if (sp.InitPort(SERIAL_COM, SERIAL_BAUD))
	{
		;
	}
	else
	{
		cerr << "error to open serial port!" << endl;
	}

	/*打开socket*/
	WORD myVersionRequest;
	WSADATA wsaData;
	myVersionRequest = MAKEWORD(1, 1);
	int err;
	err = WSAStartup(myVersionRequest, &wsaData);
	if (!err)
	{
		std::cerr << "socket opened!" << endl;
	}
	else
	{
		
		printf("failed to open socket");
		return -1;
	}
	SOCKET serSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(6000);

	bind(serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	listen(serSocket, 5);

	
	SOCKADDR_IN clientsocket;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		SOCKET serConn = accept(serSocket, (SOCKADDR*)&clientsocket, &len);
		std::cout << "accepted!" << std::endl;
		
		char receiveBuf[RECV_BUF_LEN];
		//char tempBuf[100];
		char spSentBuf[RECV_BUF_LEN];
		
		memset(receiveBuf, 0, sizeof(char) * RECV_BUF_LEN);
		memset(spSentBuf, 0, sizeof(char)* RECV_BUF_LEN);

		int recvlen = 0;
		int cycle = 0;
		while (recvlen = recv(serConn, receiveBuf + RECV_BUF_LEN*(cycle++), RECV_BUF_LEN, 0))
		{
			if (recvlen == RECV_BUF_LEN)
			{
				continue;
			}
			else
			{
				if (recvlen < RECV_BUF_LEN && recvlen >= 0)
				{
					/*主要输出部分*/
					/*json 翻译 组织输出字符*/
					jsonReader.parse(receiveBuf, root);

					if (root["protocol"]["name"] == "display" && root["message"]["styleID"] == "1")
					{
						sprintf_s(spSentBuf, RECV_BUF_LEN, "CLS(7);DS16(20,20,\'%s\',0);\r\n", root["message"]["text"]);
					}

					sp.WriteData((unsigned char*)spSentBuf, RECV_BUF_LEN);
					//sp.WriteData((const unsigned char*)"\r\n");
					//std::cout << receiveBuf << std::endl;
					std::cout << spSentBuf << std::endl;
					recvlen = 0;
					cycle = 0;
					memset(receiveBuf, 0, sizeof(char) * RECV_BUF_LEN);
				}
				else
				{
					std::cerr << "error!" << endl;
					memset(receiveBuf, 0, sizeof(char) * RECV_BUF_LEN);
					break;
				
				}
			}
		}
			
		
	}
	
	//closesocket(serConn);
	//WSACleanup();
	//return 0;
}
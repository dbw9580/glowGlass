#include <WINSOCK2.H>
#include <stdio.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
#include <string>
#include "network.h"
using std::cout, std::cin;
CRITICAL_SECTION socketSerialSync;
class Network
{
	public:
	Network();
	~Network();
	
	string dataOutput;
	
	bool initNetwork(string addr, int port);
	bool startListenThread();
	bool closeListenThread();
	
	
	private:
	SOCKET serverSocket, connSocket;
	SOCKADDR_IN listenAddr;
	char* recvBuffer[500];
	UINT WINAPI listenThread(void* param);
	bool threadExit;
	HANDLE 	listenThreadHandleHanlde;
	
};
Network::Network()
{
	ZeroMemory(recvBuffer, 500);
	WORD myVersionRequest;
	WSADATA wsaData;
	myVersionRequest = MAKEWORD(1, 1);
	int err;
	err = WSAStartup(myVersionRequest, &wsaData);
	if (!err)
	{
		cerr << "socket opened" << endl;
	}
	else
	{
		cerr << "failed to open socket" <<endl; 
	}
}

bool Network::initNetwork(const char* addr, int port)
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(addr);
	addr.sin_port = htons(port);

	bind(serverSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	
	return true;
}
bool Network::startListenThread()
{
	listen(serverSocket, 5);
	
	int len = sizeof(SOCKADDR);
	SOCKADDR_IN clientAddr;
	connSocket = accept(serverSocket, (SOCKADDR*)&clientADDR, &len);
	
	if ( listenThreadHandle != INVALID_HANDLE_VALUE)  
    {  
        return false;  
    }
	threadExit = false;
	
	UINT threadId;  
    /** 开启串口数据监听线程 */   
    listenThreadHandle = (HANDLE)_beginthreadex(NULL, 0, listenThread, this, 0, &threadId);  
    if (!listenThreadHandle)  
    {  
        return false;
    }  
    /** 设置线程的优先级,高于普通线程 */   
    if (!SetThreadPriority(listenThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL))
    {  
        return false;
    }  
 
    return true;  
}

bool Network::closeListenTread()  
{     
    if (listenThreadHandle != INVALID_HANDLE_VALUE)  
    {  
        /** 通知线程退出 */   
        threadExit = true;  
 
        /** 等待线程退出 */   
        Sleep(10);  
 
        /** 置线程句柄无效 */   
        CloseHandle( listenThreadHandle );  
        listenThreadHandle = INVALID_HANDLE_VALUE;  
    }  
    return true;  
}  
UINT WINAPI listenThread(void* param)
{
	Network *pointer = reinterpret_cast<Network*>(param);  
 
    while (!pointer->threadExit)   
    {  
        recv(connSocket, recvBuffer, strlen(receiveBuf) + 1, 0);
    }  
 
    return 0;  
}
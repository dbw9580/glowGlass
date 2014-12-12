#include <WINSOCK2.H>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>

CRITICAL_SECTION socketSerialSync;
class Network
{
	public:
	network();
	~network();
	
	string dataOutput;
	
	bool initNetwork(const char* addr, int port);
	bool startListenThread();
	bool closeListenThread();
	
	
	private:
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN listenAddr;
	string recvBuffer;
	
};
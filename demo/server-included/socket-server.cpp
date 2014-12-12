#include <WINSOCK2.H>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")


using namespace std;
int main()
{
	WORD myVersionRequest;
	WSADATA wsaData;
	myVersionRequest = MAKEWORD(1, 1);
	int err;
	err = WSAStartup(myVersionRequest, &wsaData);
	if (!err)
	{
		printf("socket opened");
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

	SOCKET serConn = accept(serSocket, (SOCKADDR*)&clientsocket, &len);
	char sendBuf[100];
	char receiveBuf[100];
	char tempBuf[100];
	while (1)
	{
		//sprintf_s(sendBuf, "welcome %s to bejing", inet_ntoa(clientsocket.sin_addr));
		sprintf_s(sendBuf, "hello world!");
		send(serConn, sendBuf, strlen(sendBuf) + 1, 0);
		
		recv(serConn, receiveBuf, strlen(receiveBuf) + 1, 0);
		
		printf("%s\r\n", receiveBuf);
	}
	closesocket(serConn);
	WSACleanup();
	return 0;
}
#include"network.h"

Network::Network(SocketConfig& conf)
{
	WORD myVersionRequest;
	WSADATA wsaData;
	myVersionRequest = MAKEWORD(1, 1);
	int err;
	err = WSAStartup(myVersionRequest, &wsaData);
	if (!err)
	{
		std::cerr << "socket opened!" << std::endl;
		config = conf;
	}
	else
	{
		std::cerr << "failed to open socket" << std::endl;
	}
}

Network::~Network()
{
	if (server && conn)
	{
		closesocket(conn);
		closesocket(server);
	}
	WSACleanup();
}

bool Network::init()
{
	server = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(config.port);

	bind(server, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	listen(server, 5);

	return true;
}

bool Network::startListen()
{
	SOCKADDR_IN clientsocket;
	int len = sizeof(SOCKADDR);
	conn = accept(server, (SOCKADDR*)&clientsocket, &len);
	std::cout << "accepted!" << std::endl;

	return true;
}

int Network::read()
{
	rbuf = "";
	char* recvBuf = new char[config.rbuff];


}
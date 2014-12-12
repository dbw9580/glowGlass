#include<map>
#include<string>

struct COMConfig
{
	int num;
	int baud;
};
struct SocketConfig
{
	int port;
	int rbuff;
	int wbuff;
};
struct StyleConfig
{
	std::map<std::string, std::string> styles;
};
struct Config
{
	struct COMConfig com;
	struct SocketConfig sock;
	struct StyleConfig style;
};
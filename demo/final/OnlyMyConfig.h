#include<map>
#include<string>
struct COMConfig;
struct SocketConfig;
struct StyleConfig;
struct Config;

typedef struct COMConfig COMConfig;
struct COMConfig
{
	int num;
	int baud;
};
typedef struct SocketConfig SocketConfig;
struct SocketConfig
{
	int port;
	int rbuff;
	int wbuff;
};
typedef struct StyleConfig StyleConfig;
struct StyleConfig
{
	std::map<std::string, std::string> styles;
};
typedef struct Config Config;
struct Config
{
	COMConfig com;
	SocketConfig sock;
	StyleConfig style;
};
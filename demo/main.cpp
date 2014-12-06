#include <iostream>
#include "SerialPort.h"
//#include "stdafx.h"

using namespace std;
int main(int argc, char agrv[])
{
	CSerialPort sp;
	//UINT numOfBytes;
	//char strRx[100];
	//int i;
	unsigned char cmdin[100];
	if (sp.InitPort(7, 115200))
	{
		/*
		if (!sp.OpenListenThread())
		{
			cerr << "error to open thread.";
		}
		*/
		int i = 0;
		char data[100];
		while (1)
		{
			if (i == 8)
			{
				i = 0;
			}
			sprintf_s(data, "CLS(%d)", i);

			sp.WriteData((unsigned char*)data, 20);
			sp.WriteData((unsigned char*)"\r\n");
			i++;
			Sleep(500);
			/*
			cin >> cmdin;
			if (cmdin[0] != '0')
			{
				sp.WriteData(cmdin, 20);
				sp.WriteData((unsigned char*)"\r\n");
			}
			else
			{
				break;
			}
			*/
		}
	}
	else
	{
		return -1;
	}

	return 0;
}

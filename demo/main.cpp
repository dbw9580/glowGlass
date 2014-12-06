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
	unsigned char cmdin[200];
	if (sp.InitPort(6, 115200))
	{
		/*
		if (!sp.OpenListenThread())
		{
			cerr << "error to open thread.";
		}
		*/
		int i = 0;
		UINT BytesToRecv = 0;
		char data[100];
		while (1)
		{
			
			if (i == 8)
			{
				i = 0;
			}
			sprintf_s(data, "SPG(%d);\r\n", i);

			sp.WriteData((unsigned char*)data, 10);
			//sp.WriteData((unsigned char*)"\r\n");
			i++;

			BytesToRecv = sp.GetBytesInCOM();
			if (BytesToRecv != 0)
			{
				for (UINT j = 0; j < BytesToRecv; j++)
				{
					cout << sp.ReadChar();
				}
			}
			
			Sleep(3000);
			
			/*
			cin >> cmdin;
			if (cmdin[0] != '0')
			{
				sp.WriteData(cmdin, 200);
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

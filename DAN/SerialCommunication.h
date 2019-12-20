#pragma once

#include "Base.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>



namespace SerialCommunicationNS {

	enum SEND_DATA
	{
		OFF,
		GREENING_10,
		GREENING_30,
		GREENING_50,
		CLEAR,
		FAILURE,
	};

	void send(BYTE data);
}

class SerialCommunication
{
public:
	HANDLE arduino;
	bool returnValue;
	bool success;
public:
	SerialCommunication();
	~SerialCommunication();

	void send(BYTE data);
};


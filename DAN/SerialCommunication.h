#pragma once

#include "Base.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class SerialCommunication
{
public:
	HANDLE arduino;
	bool returnValue;

public:
	SerialCommunication();
	~SerialCommunication();

	void send(BYTE data);

};


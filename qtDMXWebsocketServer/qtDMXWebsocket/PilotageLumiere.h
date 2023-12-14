#pragma once

#include <iostream>
#include <vector>

#include <Windows.h>
#include "DashHard.h"

#define DMX_MAXCHANNEL 512

class PilotageLumiere
{
private:
	int adressLight;
	int redValue;
	int greenValue;
	int blueValue;
	int whiteValue;

	HINSTANCE g_dasusbdll;
	typedef int (*DASHARDCOMMAND)(int, int, unsigned char*);
	DASHARDCOMMAND DasUsbCommand;

	int interface_open;
	unsigned char dmxBlock[512];

public:
	PilotageLumiere(int adressValue, int red, int green, int blue, int white);
	~PilotageLumiere();

	// Setteur
	void setAdressValue(int newValue);
	void setRedValue(int newValue);
	void setGreenValue(int newValue);
	void setBlueValue(int newValue);
	void setWhiteValue(int newValue);

	// BOOLEAN
	int isInterfaceOpen();

	// Envoie d'une trame
	void sendTrame();
};
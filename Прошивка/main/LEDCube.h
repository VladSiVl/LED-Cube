#pragma once
#include "Arduino.h"
#include "Point.h"

#define CUBE_EDGE_SIZE 8
#define CUBE_LEVEL_SIZE CUBE_EDGE_SIZE * CUBE_EDGE_SIZE

class Cube
{
	byte DS;
	byte ST_CP; //latch
	byte SH_CP; //CLK
	byte *levels;
	byte levelsCount;
	byte levelSize;

	void Pulse(byte pin);
	void WriteDS(byte _byte);
public:
	byte *data;

	Cube();
	void Init();
	void Clear();
	void ShowDataXTimes(unsigned int times);
	void TurnLEDOn(Point &point);
	void TurnLEDOff(Point &point);
};


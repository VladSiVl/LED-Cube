#include "LEDCube.h"

Cube::Cube()
{
	this->DS = 12;
	this->ST_CP = 8; //latch
	this->SH_CP = 9; //CLK
	this->levelsCount = CUBE_EDGE_SIZE;
	this->levelSize = levelsCount * CUBE_EDGE_SIZE;
	this->levels = new byte[levelsCount]{ 0, 1, 2, 3, 4, 5, 6, 7 };
	this->data = new byte[levelsCount * levelSize];
	Clear();
}

void Cube::Init() {
	pinMode(DS, OUTPUT);
	pinMode(ST_CP, OUTPUT);
	pinMode(SH_CP, OUTPUT);
	for (byte i = 0; i < levelsCount; i++) {
		pinMode(levels[i], OUTPUT);
	}
}

void Cube::Clear() {
	for (int i = 0; i < levelsCount * levelSize; i++) {
		data[i] = 0;
	}
}

void Cube::ShowDataXTimes(unsigned int times) {
	for (unsigned int _ = 0; _ < times; _++) {
		for (byte level = 0; level < levelsCount; level++) {
			for (int i = 0; i < levelSize; i++) {
				WriteDS(data[i + level * levelSize]);
			}

			if (level == 0) digitalWrite(levels[levelsCount - 1], LOW);
			else digitalWrite(levels[level - 1], LOW);
			Pulse(ST_CP);
			digitalWrite(levels[level], HIGH);
		}
	}
}

void Cube::TurnLEDOn(Point& point) {
	byte x = point.x < 0 ? levelsCount - 1 : point.x % levelsCount;
	byte y = point.y < 0 ? levelsCount - 1 : point.y % levelsCount;
	byte z = point.z < 0 ? levelsCount - 1 : point.z % levelsCount;
	point.x = x;
	point.y = y;
	point.z = z;
	data[z * levelSize + y * levelsCount + x] = 1;
}

void Cube::TurnLEDOff(Point& point) {
	byte x = point.x < 0 ? levelsCount - 1 : point.x % levelsCount;
	byte y = point.y < 0 ? levelsCount - 1 : point.y % levelsCount;
	byte z = point.z < 0 ? levelsCount - 1 : point.z % levelsCount;
	point.x = x;
	point.y = y;
	point.z = z;
	data[z * levelSize + y * levelsCount + x] = 0;
}

void Cube::Pulse(byte pin) {
	digitalWrite(pin, HIGH);
	digitalWrite(pin, LOW);
}

void Cube::WriteDS(byte _byte) {
	digitalWrite(DS, _byte == 1 ? HIGH : LOW);
	Pulse(SH_CP);
}


#include "LEDCube.h"
#include <SoftwareSerial.h>
#include "Snake.h"

SoftwareSerial BT(10, 11);
Cube *cube = new Cube();
Snake *snake = new Snake(cube);
SimpleList<short> *positions = new SimpleList<short>();
SimpleList<short> *new_positions = new SimpleList<short>();
byte mode_3_direction = NORMAL;

byte snakeSpeed;
enum Modes {
	MODE_0 = 11,
	MODE_1 = 12,
	MODE_SNAKE = 10,
	MODE_2 = 14,
	MODE_3 = 13
};

Modes currentMode = MODE_0;

void Mode_0() {
	byte positionToMove = random(CUBE_LEVEL_SIZE);
	byte fromLevel = cube->data[positionToMove] == 1 ? 0 : 7;
	bool top = fromLevel == 0;
	cube->ShowDataXTimes(5);
	while (true) {
		byte toLevel = top ? fromLevel + 1 : fromLevel - 1;
		if (toLevel >= CUBE_EDGE_SIZE || toLevel < 0) break;
		cube->data[fromLevel * CUBE_LEVEL_SIZE + positionToMove] = 0;
		cube->data[toLevel * CUBE_LEVEL_SIZE + positionToMove] = 1;
		cube->ShowDataXTimes(2);
		fromLevel = toLevel;
	}
}

void Mode_0_Init() {
	cube->Clear();
	for (byte i = 0; i < CUBE_LEVEL_SIZE; i++) {
		byte value = random(0, 2);  // max - 1
		cube->data[i] = value;  //first level
		cube->data[i + (CUBE_EDGE_SIZE - 1) * CUBE_LEVEL_SIZE] = !value;  //last level
	}
}

void Mode_1() {
	cube->Clear();
	for (byte cube_size = 0; cube_size < CUBE_EDGE_SIZE; cube_size++) {
		for (byte level = 0; level <= cube_size; level++) {
			for (byte x = 0; x <= cube_size; x++) {
				for (byte y = 0; y <= cube_size; y ++) {
					cube->data[level * CUBE_LEVEL_SIZE + y * CUBE_EDGE_SIZE + x] =
						(y % cube_size == 0 || x % cube_size == 0)
						&& level % cube_size == 0 ||
						(y % cube_size == 0) && (x % cube_size == 0) ? 1 : 0;
				}
			}
		}
		cube->ShowDataXTimes(5);
	}
	for (byte cube_size = CUBE_EDGE_SIZE - 1; cube_size > 0; cube_size--) {
		for (byte level = 0; level <= cube_size; level++) {
			for (byte x = 0; x <= cube_size; x++) {
				for (byte y = 0; y <= cube_size; y++) {
					cube->data[level * CUBE_LEVEL_SIZE + (CUBE_EDGE_SIZE - 1 - y) * CUBE_EDGE_SIZE + (CUBE_EDGE_SIZE - 1 - x)] =
						(((y % (cube_size - 1) == 0 || x % (cube_size - 1) == 0) && (level % (cube_size - 1) == 0))
						|| ((y % (cube_size - 1) == 0) && (x % (cube_size - 1) == 0) && level % cube_size != 0))
						&& x < (cube_size) && y < (cube_size) ? 1 : 0;
				}
			}
		}
		cube->ShowDataXTimes(5);
	}
}

void Mode_2_Init() {
	for (byte z = 0; z < CUBE_EDGE_SIZE; z++) {
		for (byte y = 0; y < CUBE_EDGE_SIZE; y++) {
			for (byte x = 0; x < CUBE_EDGE_SIZE; x++) {
				cube->data[z * CUBE_LEVEL_SIZE + y * CUBE_EDGE_SIZE + x] = 1;
			}
		}
	}
}

void Mode_3_Init() {
	cube->Clear();
	positions->clear();
	new_positions->clear();
	mode_3_direction = NORMAL;
	for (short y = 0; y < CUBE_LEVEL_SIZE * CUBE_EDGE_SIZE; y += CUBE_LEVEL_SIZE) {
		for (byte x = 0; x < CUBE_EDGE_SIZE; x++) {
			cube->data[x + y] = 1;
			positions->push_back(x + y);
		}		
	}
}

void Mode_3() {
	if (positions->size() == 0) {
		delete positions;
		positions = new_positions;
		new_positions = new SimpleList<short>();
		mode_3_direction = mode_3_direction == NORMAL ? INVERSE : NORMAL;
	}
	byte item = random(0, positions->size());
	short position = *((*positions)[item]);
	positions->erase(positions->begin() + item);
	byte i = 1;
	while(i++ < CUBE_EDGE_SIZE ) {
		cube->data[position] = 0;
		if(mode_3_direction == NORMAL) position += CUBE_EDGE_SIZE;
		else position -= CUBE_EDGE_SIZE;
		cube->data[position] = 1;
		cube->ShowDataXTimes(1);
	}
	new_positions->push_back(position);
}

void setup() {
	cube->Init();
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);
	BT.begin(9600);
	Mode_0_Init();
}

void loop() {

	if (BT.available()) {
		int value = BT.read();
		switch(value) {
		case MODE_0:
			currentMode = MODE_0;
			Mode_0_Init();
			break;
		case MODE_1:
			currentMode = MODE_1;
			break;
		case MODE_SNAKE:
			currentMode = MODE_SNAKE;
			snake->Init();
			break;
		case MODE_2:
			currentMode = MODE_2;
			Mode_2_Init();
			break;
		case MODE_3:
			currentMode = MODE_3;
			Mode_3_Init();
			break;
		default:
			snake->ChangeDirection(KEY(value));
		}		
	}
	
	switch(currentMode) {
	case MODE_0:
		Mode_0();
		break;
	case MODE_1:
		Mode_1();
		break;
	case MODE_SNAKE:
		snake->Move();
		snakeSpeed = 50 - 2 * snake->GetSize();
		cube->ShowDataXTimes(snakeSpeed <= 0 ? 1 : snakeSpeed);
		break;
	case MODE_2:
		cube->ShowDataXTimes(5);
		break;
	case MODE_3:
		Mode_3();
		break;
	}
}

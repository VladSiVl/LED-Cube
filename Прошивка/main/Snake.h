#pragma once
#include "Point.h"
#include "LEDCube.h"
#include "SimpleList.h"

enum KEY {
	UP = 2,
	DOWN = 4,
	LEFT = 1,
	RIGHT = 3,
};

enum DIRECTION {
	X, Y, Z
};

enum DIRECTION_TYPE {
	NORMAL, INVERSE
};

class Snake
{
	SimpleList<Point> parts;
	DIRECTION direction;
	DIRECTION_TYPE directionType;
	Point goal;
	Point *head;
	Cube *cube;

	void ApplyUp();
	void ApplyDown();
	void ApplyLeft();
	void ApplyRight();
	void MoveParts();
	void MapPartsToCube();
	bool hasReachedGoal();
	void GenerateGoal();
	void MakeStep();
	bool GoalIsPart();
	bool isClash();

public:
	Snake(Cube *cube);
	void ChangeDirection(KEY key);
	void Move();
	void Init();
	size_t GetSize();
};


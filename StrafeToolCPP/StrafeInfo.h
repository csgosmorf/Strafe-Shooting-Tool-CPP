#pragma once
#include "Vector2D.h"
#include "MemoryTool.h"
#include "Stopwatch.h"
#include "IO.h"
#include <stdlib.h>

struct PivotInfo {
	bool isPivoting;
	int velDir;
};

class StrafeInfo {
public:
	const int LEFT = -1;
	const int RIGHT = 1;
	const int IMMOBILE = 0;
	int shot;
	PivotInfo pivotInfo;
	Stopwatch watch;
	Vector2D vel, orientation, strafeVec, strafeVel;
	MemoryTool mem;

	StrafeInfo(const MemoryTool& tool);
	void setPivotInfo(float strafeVelMag);
	void update();
};


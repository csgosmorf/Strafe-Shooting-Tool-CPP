#pragma once
#include "math.h"
class Vector2D {
public:
	static const bool RECTANGULAR = false;
	static const bool POLAR = true;
	float x, y, theta, mag;
	Vector2D();
	Vector2D(float arg1, float arg2, bool arg3);
	void Set(float arg1, float arg2, bool arg3);
	static Vector2D GetPerpendicular(Vector2D v);
	static Vector2D GetProjection(Vector2D a, Vector2D b);
	static float toRadians(float degrees);
	static float toDegrees(float radians);
};


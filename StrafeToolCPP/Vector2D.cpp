#include "Vector2D.h"

constexpr double PI = 3.14159265358979323846;

Vector2D::Vector2D() {
	Set(0, 0, RECTANGULAR);
}

Vector2D::Vector2D(float arg1, float arg2, bool arg3) {
	Set(arg1, arg2, arg3);
}

void Vector2D::Set(float arg1, float arg2, bool arg3) {
	if (arg3 == RECTANGULAR) {
		x = arg1;
		y = arg2;
		theta = atan2f(y, x);
		mag = sqrtf(x * x + y * y);
	}
	else {
		theta = arg1;
		mag = arg2;
		x = mag * cosf(theta);
		y = mag * sinf(theta);
	}
}
Vector2D Vector2D::GetPerpendicular(Vector2D v) {
	return Vector2D(v.theta - (float)(PI / 2.0), v.mag, POLAR);
}

Vector2D Vector2D::GetProjection(Vector2D a, Vector2D b) {
	float scl = (a.x * b.x + a.y * b.y) / (b.mag * b.mag);
	return Vector2D(b.theta, scl * b.mag, POLAR);
}

float Vector2D::toDegrees(float radians) {
	return (float)(radians * 180.0 / PI);
}

float Vector2D::toRadians(float degrees) {
	return (float)(degrees * PI / 180.0);
}
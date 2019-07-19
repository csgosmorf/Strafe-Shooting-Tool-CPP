#include "StrafeInfo.h"


StrafeInfo::StrafeInfo(const MemoryTool& tool) {
	mem = tool;
	shot = 0;
	pivotInfo = PivotInfo();
	vel = Vector2D(0, 0, false);
	orientation = Vector2D(Vector2D::toRadians(mem.readFloat(mem.aPlayer + addresses::m_angEyeAnglesY)), 1, Vector2D::POLAR);
	strafeVec = Vector2D::GetPerpendicular(orientation);
	strafeVel = Vector2D::GetProjection(vel, strafeVec);
	pivotInfo.velDir = strafeVel.mag < -0.01 ? LEFT : (strafeVel.mag > 0.01 ? RIGHT : IMMOBILE);
}

void StrafeInfo::setPivotInfo(float strafeVelMag) {
	int prevVelDir = pivotInfo.velDir;
	pivotInfo.velDir = strafeVelMag < -0.01 ? LEFT : (strafeVelMag > 0.01 ? RIGHT : IMMOBILE);

	bool aPressed = IO::keyPressed(65);
	bool dPressed = IO::keyPressed(68);
	bool onGround = (mem.readInt(mem.aPlayer + mem.oFlags) & 1) != 0;

	pivotInfo.isPivoting = false;
	if (IO::keyPressed(0x57) == 0 && IO::keyPressed(0x53) == 0 && onGround) {
		if (prevVelDir == LEFT && pivotInfo.velDir != LEFT && dPressed)
			pivotInfo.isPivoting = true;
		else if (prevVelDir == RIGHT && pivotInfo.velDir != RIGHT && aPressed)
			pivotInfo.isPivoting = true;
		else if (prevVelDir == LEFT && pivotInfo.velDir == LEFT && strafeVelMag > -3.0 && dPressed) {
			pivotInfo.isPivoting = true;
			pivotInfo.velDir = RIGHT;
		}
		else if (prevVelDir == RIGHT && pivotInfo.velDir == RIGHT && strafeVelMag < 3.0 && aPressed) {
			pivotInfo.isPivoting = true;
			pivotInfo.velDir = LEFT;
		}
	}
}

void StrafeInfo::update() {
	vel.Set(mem.readFloat(mem.aVelocity), mem.readFloat(mem.aVelocity + 4), Vector2D::RECTANGULAR);
	orientation.Set(Vector2D::toRadians(mem.readFloat(mem.aPlayer + addresses::m_angEyeAnglesY)), 1, Vector2D::POLAR);
	strafeVec = Vector2D::GetPerpendicular(orientation);
	strafeVel = Vector2D::GetProjection(vel, strafeVec);
	setPivotInfo(strafeVel.mag);
	if (pivotInfo.isPivoting) {
		if (watch.isRunning && watch.starter == Stopwatch::GUN) {
			long elapsedTime = watch.ms();
			watch.reset();
			if (elapsedTime < 200) { //updateForm(-elapsedTime);
				system("CLS");
				std::cout << "Early " << elapsedTime << "ms.\n";
			}
			else watch.renew(Stopwatch::PIVOT);
		}
		else watch.renew(Stopwatch::PIVOT);
	}
	int prevShot = shot;
	shot = mem.readInt(mem.aPlayer + addresses::m_iShotsFired);
	if (shot == 1 && prevShot == 0) {
		if (watch.isRunning && watch.starter == Stopwatch::PIVOT) {
			long elapsedTime = watch.ms();
			watch.reset();
			if (elapsedTime < 200) { //updateForm(elapsedTime);
				system("CLS");
				std::cout << "Late " << elapsedTime << "ms.\n";
			}
			else watch.renew(Stopwatch::GUN);
		}
		else watch.renew(Stopwatch::GUN);
	}
}
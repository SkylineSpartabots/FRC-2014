#ifndef SHOOTER_H
#define SHOOTER_H

#include "WPILib.h"
#include "../Collector/Collector.h"

class Shooter {
	public:
		Shooter(Talon *motors, DigitalInput *limitSwitch, Collector *collector, RobotDrive *drive);
		~Shooter();
		void Shoot(double shoot_power);
		void Set(double power);
		void Stop();
		void Reset();
		void ShootWithArm(bool steepShot, double shoot_power);
		bool BringArmDown();
		bool GetLimitSwitch();
		void ShooterPass();
		void WatchdogWait(double time);
	private:
		Collector *m_collector;
		Talon *m_motors;
		DigitalInput *m_limitSwitch;
		RobotDrive *m_drive;
};
#endif

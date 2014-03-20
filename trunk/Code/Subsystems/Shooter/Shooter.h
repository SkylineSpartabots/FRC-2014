#ifndef SHOOTER_H
#define SHOOTER_H

#include "WPILib.h"
#include "../Collector/Collector.h"

class Shooter {
	public:
		bool IsAutonomous();
		Shooter(Talon *motors, DigitalInput *limitSwitch, Collector *collector);
		~Shooter();
		void Shoot();
		void Set(double power);
		void Stop();
		void Reset();
		void ShootWithArm();
		bool BringArmDown();
		bool manualAuto;
		void setManualOff();
		bool GetLimitSwitch();
		void ShooterPass();
		void WatchdogWait(double time);
	private:
		Collector *m_collector;
		Talon *m_motors;
		DigitalInput *m_limitSwitch;
};
#endif

#ifndef SHOOTER_H
#define SHOOTER_H

#include "WPILib.h"
#include "../Collector/Collector.h"

class Shooter {
	public:
		Shooter(Talon *motors, DigitalInput *limitSwitch, Collector *collector);
		~Shooter();
		void Shoot();
		void Set(double power);
		void Stop();
		void Reset();
		void ShootWithArm();
		bool BringArmDown();
		bool GetLimitSwitch();
		void ShooterPass();
		void WatchdogWait(double time);
		bool manualAuto;
	private:
		Collector *m_collector;
		Talon *m_motors;
		DigitalInput *m_limitSwitch;
};
#endif

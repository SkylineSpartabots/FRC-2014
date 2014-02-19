#ifndef SHOOTER_H
#define SHOOTER_H

#include "WPILib.h"
#include "../Collector/Collector.h"

class Shooter {
	public:
		Shooter(Talon *motorLeft1, Talon *motorLeft2, Talon *motorRight1,
				Talon *motorRight2, DigitalInput *limitSwitch, Collector *collector);
		~Shooter();
		void Shoot();
		void Set(double power);
		void Stop();
		void Reset();
		void shootWithArm();
		bool GetLimitSwitch();
	private:
		Collector *m_collector;
		Talon *m_motorLeft1;
		Talon *m_motorLeft2;
		Talon *m_motorRight1;
		Talon *m_motorRight2;
		DigitalInput *m_limitSwitch;
};
#endif

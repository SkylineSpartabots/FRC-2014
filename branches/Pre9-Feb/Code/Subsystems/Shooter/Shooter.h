#ifndef SHOOTER_H
#define SHOOTER_H

#include "WPILib.h"
#include "../Collector/Collector.h"

class Shooter {
	public:
		Shooter(Talon *motorLeft1, Talon *motorLeft2, Talon *motorRight1,
				Talon *motorRight2, DigitalInput *limitSwitchBottom, DigitalInput *limitSwitchTop, Collector *collector);
		~Shooter();
		void Shoot();
		void Reset();
		void shootWithArm(); 
	private:
		Collector *m_collector;
		Talon *m_motorLeft1;
		Talon *m_motorLeft2;
		Talon *m_motorRight1;
		Talon *m_motorRight2;
		DigitalInput *m_limitSwitchBottom;
		DigitalInput *m_limitSwitchTop;
};
#endif

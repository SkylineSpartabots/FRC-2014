#ifndef SHOOTER_H
#define SHOOTER_H

#include "WPILib.h"

class Shooter{
	public:
		Shooter(Victor *motorLeft1, Victor *motorLeft2, Victor *motorRight1, Victor *motorRight2, DigitalInput *limitSwitch);
		~Shooter();
		void Shoot();
		void Reset();
	private:
		Victor *m_motorLeft1;
		Victor *m_motorLeft2;
		Victor *m_motorRight1;
		Victor *m_motorRight2;
		DigitalInput *m_limitSwitch;
};
#endif

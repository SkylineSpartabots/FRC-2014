#ifndef CLAW_H
#define CLAW_H

#include "WPILib.h"

class Claw {
	public:
		Claw(Victor *motor);
		~Claw();
		void Open();
		void Close();
		void Stop();	
	private:
		Victor *m_motor;
};

#endif

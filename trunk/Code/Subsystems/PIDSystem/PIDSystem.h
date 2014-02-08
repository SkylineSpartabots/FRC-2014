#ifndef PIDSYSTEM_H
#define PIDSYSTEM_H

#include "Commands/PIDSubsystem.h"
#include "WPILib.h"

class PIDSystem : public PIDSubsystem{
	public:
		PIDSystem(Encoder *encoder, Victor *motor1, Victor *motor2, Command *m_command);
		void InitDefaultCommand();
		double ReturnPIDInput(); 
		void UsePIDOutput();
		void setSpeed(float speed);
	private:
		Encoder *m_encoder;
		Victor *m_motor1, *m_motor2;
		Command *m_command;
		double *m_speed;
};

#endif

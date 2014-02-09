#ifndef PIDSYSTEM_H
#define PIDSYSTEM_H

#include "Commands/PIDSubsystem.h"
#include "WPILib.h"

class PIDSystem : public PIDSubsystem{
	public:
		PIDSystem(Encoder *encoder, RobotDrive *robotDrive);
		void InitDefaultCommand();
		double ReturnPIDInput(); 
		void UsePIDOutput();
		void setSpeed(float speed);
		void SetSetPoint(float setPoint);
		void setDistancePerPulse(float distancePerPoint);
	private:
		Encoder *m_encoder;
		RobotDrive *m_robotDrive;
		double *m_speed;
};

#endif

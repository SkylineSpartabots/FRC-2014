#include "PIDSystem.h"

PIDSystem::PIDSystem(Encoder *encoder, RobotDrive *robotDrive) : PIDSubsystem("motor", 1.0, 1.0, 0.0, 1.0){
	m_encoder = encoder;
	m_robotDrive = robotDrive;
	*m_speed = 0.0;
	
	GetPIDController()->SetContinuous(false);
	GetPIDController()->SetOutputRange(-1.0, 1.0);
	Enable();
}

void PIDSystem::InitDefaultCommand(){
	//empty
}

double PIDSystem::ReturnPIDInput(){
	return m_encoder->Get();
}

void PIDSystem::setSpeed(float speed){
	*m_speed = speed;
}

void PIDSystem::SetSetPoint(float setPoint){
	SetSetpoint(setPoint);
}

void PIDSystem::UsePIDOutput(){
	m_robotDrive->Drive(*m_speed, 0.0);
}

void PIDSystem::setDistancePerPulse(float distancePerPoint){
	m_encoder->SetDistancePerPulse(1.0f / distancePerPoint);
}


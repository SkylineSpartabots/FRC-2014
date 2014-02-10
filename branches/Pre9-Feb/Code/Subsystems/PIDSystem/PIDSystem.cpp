#include "PIDSystem.h"

PIDSystem::PIDSystem(Encoder *encoder, Victor *motor1, Victor *motor2, Command *m_command) : PIDSubsystem("motor", 1.0, 1.0, 0.0, 1.0){
	GetPIDController()->SetContinuous(false);
	m_encoder = encoder;
	m_motor1 = motor1;
	m_motor2 = motor2;
	*m_speed = 0.0;
	SetSetpoint(*m_speed);
	Enable();
}

void PIDSystem::InitDefaultCommand(){
	SetDefaultCommand(m_command);
}

double PIDSystem::ReturnPIDInput(){
	return m_encoder->Get();
}

void PIDSystem::setSpeed(float speed){
	*m_speed = speed;
}

void PIDSystem::UsePIDOutput(){
	m_motor1->Set(*m_speed);
	m_motor2->Set(*m_speed);
}

#include "Shooter.h"

double shoot_power = 1;
double reset_power = 0.15;
double upDownArmTime = 0.25;

/* The shootTime value is very important to understand before changing.  
 * It controls the time that the shooter arm is allowed to rotate.
 * If the value is too high, the arm will wrap around (and into) the
 * back of the robot. 
 */
double shootTime = 0.2;

Shooter::Shooter(Talon *motorLeft1, Talon *motorLeft2, Talon *motorRight1,
		Talon *motorRight2, DigitalInput *limitSwitch, Collector *collector){
	m_collector = collector;
	m_motorLeft1 = motorLeft1;
	m_motorLeft2 = motorLeft2;
	m_motorRight1 = motorRight1;
	m_motorRight2 = motorRight2;
	m_limitSwitch = limitSwitch;
	
}

Shooter::~Shooter (){
	//empty
}

void Shooter::Set(double power) {
	m_motorLeft1->Set(power);
	m_motorLeft2->Set(power);
	m_motorRight1->Set(power);
	m_motorRight2->Set(power);
}

void Shooter::Shoot (){
	m_motorLeft1->Set(-shoot_power);
	m_motorLeft2->Set(-shoot_power);
	m_motorRight1->Set(-shoot_power);
	m_motorRight2->Set(-shoot_power);
	/* Commented out
	if (m_limitSwitchTop->Get()){
		m_motorLeft1->Set(0);
		m_motorLeft2->Set(0);
		m_motorRight1->Set(0);
		m_motorRight2->Set(0);
	}*/
}

void Shooter::Stop() {
	m_motorLeft1->Set(0);
	m_motorLeft2->Set(0);
	m_motorRight1->Set(0);
	m_motorRight2->Set(0);
}

void Shooter::Reset() {
	m_motorLeft1->Set(reset_power);
	m_motorLeft2->Set(reset_power);
	m_motorRight1->Set(reset_power);
	m_motorRight2->Set(reset_power);
	/*
	Wait(shootTime*7.5);
	m_motorLeft1->Set(0);
	m_motorLeft2->Set(0);
	m_motorRight1->Set(0);
	m_motorRight2->Set(0);*/
}

bool Shooter::GetLimitSwitch() {
	return m_limitSwitch->Get();
}

void Shooter::BringArmDown() {
	Reset();
	while(true) {
		if (m_limitSwitch->Get()) {
			Stop();
			break;
		}
	}
}

void Shooter::ShootWithArm() {
	m_collector->BringArmDown();
	BringArmDown();
	
	Shoot();
	Wait(shootTime);
	Stop();
	Wait(2);
	BringArmDown();
}

void Shooter::ShooterPass(){
	m_collector->PistonPull();
	m_collector->SpinOutwards();
	Set(0.2);
	Wait(0.3);
	Set(0);
	m_collector->SpinStop();
	BringArmDown();
}

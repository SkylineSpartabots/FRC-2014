#include "Shooter.h"

bool shoot_power = 1;
double upDownArmTime = 0.25;
double shootTime = 0.1;

Shooter::Shooter(Talon *motorLeft1, Talon *motorLeft2, Talon *motorRight1,
		Talon *motorRight2, DigitalInput *limitSwitchBottom, DigitalInput *limitSwitchTop, Collector *collector){
	m_collector = collector;
	m_motorLeft1 = motorLeft1;
	m_motorLeft2 = motorLeft2;
	m_motorRight1 = motorRight1;
	m_motorRight2 = motorRight2;
	m_limitSwitchBottom = limitSwitchBottom;
	m_limitSwitchTop = limitSwitchTop; 
	
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
	/* Commented out for now
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
	m_motorLeft1->Set(0.2);
	m_motorLeft2->Set(0.2);
	m_motorRight1->Set(0.2);
	m_motorRight2->Set(0.2);
	
	Wait(shootTime*7.5);
	m_motorLeft1->Set(0);
	m_motorLeft2->Set(0);
	m_motorRight1->Set(0);
	m_motorRight2->Set(0);
}

void Shooter::shootWithArm() {
	m_collector->PistonPush();
	Wait(upDownArmTime);
	m_collector->PistonNeutral();
	Wait(0.1);
	
	Shoot();
	Wait(shootTime);
	Stop();
	Wait(2);
	Reset();
	
	m_collector->PistonPull();
	Wait(upDownArmTime);
}

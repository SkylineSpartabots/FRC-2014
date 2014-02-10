#include "Shooter.h"

Shooter::Shooter(Talon *motorLeft1, Talon *motorLeft2, Talon *motorRight1, Talon *motorRight2, DigitalInput *limitSwitchBotoom, DigitalInput *limitSwitchTop){
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
void Shooter::Shoot (){
	m_motorLeft1->Set(1.0);
	m_motorLeft2->Set(1.0);
	m_motorRight1->Set(1.0);
	m_motorRight2->Set(1.0);
	
	if(m_limitSwitchTop->Get()){
		m_motorLeft1->Set(0);
		m_motorLeft2->Set(0);
		m_motorRight1->Set(0);
		m_motorRight2->Set(0);
	}
}
void Shooter::Reset () {
	m_motorLeft1->Set(-1.0);
	m_motorLeft2->Set(-1.0);
	m_motorRight1->Set(-1.0);
	m_motorRight2->Set(-1.0);
	
	if (m_limitSwitchBottom->Get()){	
		m_motorLeft1->Set(0);
		m_motorLeft2->Set(0);
		m_motorRight1->Set(0);
		m_motorRight2->Set(0);
	}
}

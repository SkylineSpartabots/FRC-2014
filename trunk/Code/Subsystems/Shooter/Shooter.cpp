#include "Shooter.h"

Shooter::Shooter(Victor *motorLeft1, Victor *motorLeft2, Victor *motorRight1, Victor *motorRight2, DigitalInput *limitSwitch){
	m_motorLeft1 = motorLeft1;
	m_motorLeft2 = motorLeft2;
	m_motorRight1 = motorRight1;
	m_motorRight2 = motorRight2;
	m_limitSwitch = limitSwitch;
}

Shooter::~Shooter (){
	//empty
}
void Shooter::Shoot (){
	m_motorLeft1->Set(1.0);
	m_motorLeft2->Set(1.0);
	m_motorRight1->Set(1.0);
	m_motorRight2->Set(1.0);
	
	//wait until switch
	
	m_motorLeft1->Set(0);
	m_motorLeft2->Set(0);
	m_motorRight1->Set(0);
	m_motorRight2->Set(0);
}
void Shooter::Reset () {
	m_motorLeft1->Set(-1.0);
	m_motorLeft2->Set(-1.0);
	m_motorRight1->Set(-1.0);
	m_motorRight2->Set(-1.0);
	
	if (m_limitSwitch->Get()){	
		m_motorLeft1->Set(0);
		m_motorLeft2->Set(0);
		m_motorRight1->Set(0);
		m_motorRight2->Set(0);
	}
}

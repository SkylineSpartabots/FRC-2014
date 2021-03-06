#include "Collector.h"
#include "../../MainRobot.h"

Collector::Collector(Victor *motor, Solenoid *piston1, Solenoid *piston2, Solenoid *piston3,
		Solenoid *piston4, Compressor *compressor, DigitalInput *pistonLimitSwitch, RobotDrive *drive) {
	m_motor = motor;
	m_piston1 = piston1;
	m_piston2 = piston2;
	m_piston3 = piston3;
	m_piston4 = piston4;
	m_compressor = compressor;
	m_pistonLimitSwitch = pistonLimitSwitch;
	m_drive = drive;
	
}

/*
bool Collector::GetLimitSwitch() {
	return m_limitSwitch->Get();
}
*/

void Collector::SpinInwards() {
	m_motor->Set(0.70);
}

void Collector::SpinOutwards() {
	m_motor->Set(-0.70);
}

void Collector::SpinStop() {
	m_motor->Set(0.0);
}

void Collector::PistonPush() {
	m_piston1->Set(false);
	m_piston2->Set(true);
	m_piston3->Set(true);
	m_piston4->Set(false);
}

void Collector::PistonPull() {
	m_piston1->Set(true);
	m_piston2->Set(false);
	m_piston3->Set(false);
	m_piston4->Set(true);
}

void Collector::PistonNeutral(){
	m_piston1->Set(true);
	m_piston2->Set(false);
	m_piston3->Set(true);
	m_piston4->Set(false);
}

void Collector::PistonSlightPush(){
	m_piston1->Set(false);
	m_piston2->Set(true);
	m_piston3->Set(false);
	m_piston4->Set(true);
}

bool Collector::BringArmDown(){
	bool success = true;
	
	PistonPush();
	Timer* timer = new Timer();
	timer->Start();
	while (true) {
		RobotBase::getInstance().GetWatchdog().Feed();
		m_drive->Drive(0,0);
		if (isFullyExtended()){
			PistonNeutral();
			break;
		}
		
		if (timer->Get() > 2.75){
			PistonNeutral();
			success = false;
			break;
		}
	}
	timer->Stop();
	delete timer;
	return success;
}

bool Collector::isExtended(){
	if (m_piston1->Get() == true && m_piston2->Get() == false && m_piston3->Get() == false && m_piston4->Get() == true){
		return true;
	}
	else return false;
}

bool Collector::isFullyExtended(){
	return !m_pistonLimitSwitch->Get();
}

void Collector::WatchdogWait(double time) {
	Timer* timer = new Timer();
	timer->Start();
	while (true) {
		RobotBase::getInstance().GetWatchdog().Feed();
		m_drive->Drive(0,0);
		if (timer->Get() >= time) {
			break;
		}
		Wait(.05);
	}
	timer->Stop();
	delete timer;
}

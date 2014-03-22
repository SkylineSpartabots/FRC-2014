#include "Shooter.h"

double shoot_power = 1;
double reset_power = 0.2;
double upDownArmTime = 0.25;
bool manualAuto = true;

/* The shootTime value is very important to understand before changing.  
 * It controls the time that the shooter arm is allowed to rotate.
 * If the value is too high, the arm will wrap around (and into) the
 * back of the robot. 
 */
double shootTime = 0.22;

Shooter::Shooter(Talon *motors, DigitalInput *limitSwitch, Collector *collector){
	m_collector = collector;
	m_motors = motors;
	/*m_motorLeft2 = motorLeft2;
	m_motorRight1 = motorRight1;
	m_motorRight2 = motorRight2;*/
	m_limitSwitch = limitSwitch;
	
}

Shooter::~Shooter (){
	//empty
}

void Shooter::Set(double power) {
	m_motors->Set(power);
}

void Shooter::Shoot (){
	m_motors->Set(-shoot_power);
	/* Commented out
	if (m_limitSwitchTop->Get()){
		m_motorLeft1->Set(0);
		m_motorLeft2->Set(0);
		m_motorRight1->Set(0);
		m_motorRight2->Set(0);
	}*/
}

void Shooter::Stop() {
	m_motors->Set(0);
}

void Shooter::Reset() {
	m_motors->Set(reset_power);
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

bool Shooter::BringArmDown() {
	if (manualAuto == true)
	{
		//Autonomous mode
		bool success = true;
		
		Reset();
		Timer* timer = new Timer();
		timer->Start();
		while(true && IsAutonomous()) {
			RobotBase::getInstance().GetWatchdog().Feed();
			if (m_limitSwitch->Get()) {
				Stop();
				break;
			}
			if (timer->Get() > 4) {
				Stop();
				success = false;
				break;
			}
		}
		timer->Stop();
		timer->Reset();
		return success;
	}
	else
	{
		//Not autonomous mode
		bool success = true;
		
		Reset();
		Timer* timer = new Timer();
		timer->Start();
		while(true) {
			if (IsAutonomous())
			{
			RobotBase::getInstance().GetWatchdog().Feed();
			if (m_limitSwitch->Get()) {
				Stop();
				break;
			}
			if (timer->Get() > 4) {
				Stop();
				success = false;
				break;
			}
			}
		}
		timer->Stop();
		timer->Reset();
		return success;
	}
}

void Shooter::ShootWithArm() {
	m_collector->BringArmDown();
	BringArmDown();
	
	Shoot();
	WatchdogWait(shootTime);
	Stop();
	WatchdogWait(1);
	BringArmDown();
}


void Shooter::ShooterPass(){
	m_collector->PistonPull();
	WatchdogWait(.75);
	m_collector->SpinOutwards();
	WatchdogWait(0.5);
	Set(-0.3);
	WatchdogWait(0.45);
	Set(0);
	m_collector->SpinStop();
	BringArmDown();
}


void Shooter::WatchdogWait(double time) {
	Timer* timer = new Timer();
	timer->Start();
	while (true) {
		RobotBase::getInstance().GetWatchdog().Feed();
		if (timer->Get() >= time) {
			break;
		}
		Wait(.05);
	}
}

#include "Shooter.h"

double shoot_power = 1;
double reset_power = 0.35;
double upDownArmTime = 0.25;

/* The shootTime value is very important to understand before changing.  
 * It controls the time that the shooter arm is allowed to rotate.
 * If the value is too high, the arm will wrap around (and into) the
 * back of the robot. 
 */
double shootTime = 0.32;

Shooter::Shooter(Talon *motors, DigitalInput *limitSwitch, Collector *collector){
	m_collector = collector;
	m_motors = motors;
	/*m_motorLeft2 = motorLeft2;
	m_motorRight1 = motorRight1;
	m_motorRight2 = motorRight2;*/
	m_limitSwitch = limitSwitch;
	manualAuto = false;
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
	if (manualAuto == true) {
		//Autonomous mode
		bool success = true;
		
		Reset();
		Timer* timer = new Timer();
		timer->Start();
		while(true && manualAuto) {
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
	} else {
		//Not autonomous mode
		bool success = true;
		
		Reset();
		Timer* timer = new Timer();
		timer->Start();
		while(true) {
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
}

void Shooter::ShootWithArm() {
	m_collector->BringArmDown();
	SmartDashboard::PutNumber("Shooting", 1);
	BringArmDown();
	RobotBase::getInstance().GetWatchdog().Feed();
	SmartDashboard::PutNumber("Shooting", 2);
	
	Shoot();
	RobotBase::getInstance().GetWatchdog().Feed();
	WatchdogWait(shootTime);
	SmartDashboard::PutNumber("Shooting", 3);
	Stop();
	RobotBase::getInstance().GetWatchdog().Feed();
	WatchdogWait(1);
	SmartDashboard::PutNumber("Shooting", 4);
	BringArmDown();
	SmartDashboard::PutNumber("Shooting", 5);
}


void Shooter::ShooterPass(){
	m_collector->PistonPull(); // Bring collector arm up
	WatchdogWait(.75); // Wait for arm to go up
	m_collector->SpinOutwards(); // Start spinning outwards
	//WatchdogWait(0.5); // Wait for half a second (why is this even here?)
	Set(-0.3); // Make shooter arm go up
	WatchdogWait(0.45); // Wait 0.45 seconds for arm to go up
	Set(0); // Stop arm
	WatchdogWait(2.2); // Wait a little bit with arm up for spin
	m_collector->SpinStop(); // Stop spinning
	BringArmDown(); // Bring shooter arm down
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

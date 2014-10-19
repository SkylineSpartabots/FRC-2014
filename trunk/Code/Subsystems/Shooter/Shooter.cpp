#include "Shooter.h"

double reset_power = 0.35;
double upDownArmTime = 0.25;

/* The shootTime value is very important to understand before changing.  
 * It controls the time that the shooter arm is allowed to rotate.
 * If the value is too high, the arm will wrap around (and into) the
 * back of the robot. 
 */
double shootTime = 0.34;
double shootTimeSteep = 0.192;

Shooter::Shooter(Talon *motors, DigitalInput *limitSwitch, Collector *collector, RobotDrive *drive){
	m_collector = collector;
	m_motors = motors;
	m_limitSwitch = limitSwitch;
	m_drive = drive;
}

Shooter::~Shooter (){
	//empty
}

void Shooter::Set(double power) {
	m_motors->Set(power);
}

void Shooter::Shoot(double shoot_power){
	m_motors->Set(-shoot_power);
}

void Shooter::Stop() {
	m_motors->Set(0);
}

void Shooter::Reset() {
	m_motors->Set(reset_power);
}

bool Shooter::GetLimitSwitch() {
	return m_limitSwitch->Get();
}

bool Shooter::BringArmDown() {
	bool success = true;
		
	Reset();
	Timer* timer = new Timer();
	timer->Start();
	while(true) {
		RobotBase::getInstance().GetWatchdog().Feed();
		m_drive->Drive(0,0);
		if (m_limitSwitch->Get()) {
			Stop();
			break;
		}
		if (timer->Get() > 4) {
			Stop();
			success = false;
			break;
		}
		WatchdogWait(.005);
	}
	timer->Stop();
	delete timer;
	return success;
}

void Shooter::ShootWithArm(bool steepShot, double shoot_power) {
	SmartDashboard::PutNumber("Shooting", 0);
	
	//bool bringArmDownSuccess = m_collector->BringArmDown();
	SmartDashboard::PutNumber("Shooting 1", 1);
	BringArmDown();
	//SmartDashboard::PutBoolean("Bring arm down success", bringArmDownSuccess);
	RobotBase::getInstance().GetWatchdog().Feed();
	SmartDashboard::PutNumber("Shooting", 2);
	
	Shoot(shoot_power);
	RobotBase::getInstance().GetWatchdog().Feed();
	WatchdogWait(steepShot ? shootTimeSteep : shootTime);
	SmartDashboard::PutNumber("Shooting", 3);
	Stop();
	RobotBase::getInstance().GetWatchdog().Feed();
	WatchdogWait(1);
	SmartDashboard::PutNumber("Shooting", 4);
	BringArmDown();
	SmartDashboard::PutNumber("Shooting", 5);

	SmartDashboard::PutNumber("Shooting 1", 0);
	SmartDashboard::PutBoolean("Extended", false);
}

void Shooter::ShooterPass(){
	m_collector->PistonPull(); // Bring collector arm up
	WatchdogWait(1.9); // Wait for arm to go up
	m_collector->SpinOutwards(); // Start spinning outwards
	Set(-0.3); // Make shooter arm go up
	WatchdogWait(0.45); // Wait 0.45 seconds for arm to go up
	Set(0); // Stop arm
	WatchdogWait(1.5); // Wait a little bit with arm up for spin
	m_collector->SpinStop(); // Stop spinning
	BringArmDown(); // Bring shooter arm down
}


void Shooter::WatchdogWait(double time) {
	Timer* timer = new Timer();
	timer->Start();
	while (true) {
		RobotBase::getInstance().GetWatchdog().Feed();
		m_drive->Drive(0,0);
		if (timer->Get() >= time) {
			break;
		}
		Wait(.005);
	}
	timer->Stop();
	delete timer;
}

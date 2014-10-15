#include "WPILib.h"
#include "Ports.h"
//#include "Subsystems/Claw/Claw.h"
#include "Subsystems/Shooter/Shooter.h"
//#include "Subsystems/Vision/Vision.h"
#include "Subsystems/Collector/Collector.h"
#include "Subsystems/Controllers/XboxController.h"
#include "Subsystems/Collector/Collector.h"

//#include "Vision/RGBImage.h"
#include "SmartDashboard/SmartDashboard.h"
#include "NetworkTables/NetworkTable.h"

class MainRobot : public SimpleRobot
{
private:
	RobotDrive *m_drive;
	Joystick *m_rightStick;
	Joystick *m_leftStick;
	XboxController *driveController;
	XboxController *shootController;
	Victor *m_collectorMotor;
	Compressor *m_compressor;
	Solenoid *m_solenoid1;
	Solenoid *m_solenoid2;
	Solenoid *m_solenoid3;
	Solenoid *m_solenoid4;
	
	Talon *m_shooterMotors;
	DigitalInput *m_shooterLimitSwitch; 
	DigitalInput *m_pistonLimitSwitch; 
	
	Collector *m_collector;
	Shooter *m_shooter;
	
	
public:
	MainRobot();
	void RobotInit();
	void InitializeHardware();
	void InitializeSoftware();
	void WatchdogWait(double time); // Use only in autonomous mode
	void Autonomous();
	void AutonomousDrive(float magnitude, float curve);
	void OperatorControl();
	void Test();
	float Cutoff(float num);
};

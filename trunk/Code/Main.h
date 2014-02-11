#include "WPILib.h"
#include "Ports.h"
#include "Subsystems/Claw/Claw.h"
#include "Subsystems/Shooter/Shooter.h"
#include "Subsystems/Vision/Vision.h"
#include "Subsystems/Collector/Collector.h"
#include "Subsystems/Controllers/XboxController.h"

#include "Vision/RGBImage.h"
#include "SmartDashboard/SmartDashboard.h"

class MainRobot : public SimpleRobot
{
private:
	RobotDrive *m_drive;
	Joystick *m_rightStick;
	Joystick *m_leftStick;
	XboxController *xbox;
	
	Victor *m_collectorMotor;
	Compressor *m_compressor;
	Solenoid *m_solenoid1;
	Solenoid *m_solenoid2;
	Solenoid *m_solenoid3;
	Solenoid *m_solenoid4;
	
	//Victor *m_clawMotor;
	
	Talon *m_shooterLeft1;
	Talon *m_shooterLeft2;
	Talon *m_shooterRight1;
	Talon *m_shooterRight2;
	DigitalInput *m_shooterLimitSwitchBottom;
	DigitalInput *m_shooterLimitSwitchTop;
	
	//Claw *m_claw;
	Collector *m_collector;
	Shooter *m_shooter;
	
public:
	MainRobot();
	void RobotInit();
	void InitializeHardware();
	void InitializeSoftware();
	void Autonomous();
	void OperatorControl();
	void Test();
	float Cutoff(float num);
};

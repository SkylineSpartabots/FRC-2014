#include "WPILib.h"
#include "Ports.h"
#include "Subsystems/Claw/Claw.h"

class MainRobot : public SimpleRobot
{
private:
	RobotDrive *m_drive;
	Joystick *m_rightStick;
	Joystick *m_leftStick;
	
	Victor *m_clawMotor;
	
	Claw *m_claw;
	
public:
	MainRobot();
	void InitializeHardware();
	void InitializeSoftware();
	void Autonomous();
	void OperatorControl();
	void Test();
	
};


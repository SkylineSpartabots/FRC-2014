#include "Main.h"

MainRobot::MainRobot()
{
	InitializeHardware();
	InitializeSoftware();
	
	m_drive->SetExpiration(0.1);
}

void MainRobot::InitializeHardware() {
	m_drive = new RobotDrive(Ports::DigitalSidecar::Pwm1, 
							 Ports::DigitalSidecar::Pwm2, 
							 Ports::DigitalSidecar::Pwm3, 
							 Ports::DigitalSidecar::Pwm4);
	m_leftStick = new Joystick(Ports::Computer::Usb1);
	m_rightStick = new Joystick(Ports::Computer::Usb2);
	
	m_clawMotor = new Victor(Ports::DigitalSidecar::Pwm5);
	
	m_shooterLeft1 = new Victor(Ports::DigitalSidecar::Pwm6);
	m_shooterLeft2 = new Victor(Ports::DigitalSidecar::Pwm7);
	m_shooterRight1 = new Victor(Ports::DigitalSidecar::Pwm8);
	m_shooterRight2 = new Victor(Ports::DigitalSidecar::Pwm9);
	m_shooterLimitSwitch = new DigitalInput(Ports::DigitalSidecar::Gpio1);
}

void MainRobot::InitializeSoftware() {
	m_claw = new Claw(m_clawMotor);
	m_shooter = new Shooter(m_shooterLeft1, m_shooterLeft2, m_shooterRight1, m_shooterRight2, m_shooterLimitSwitch);
}

/**
 * Drive left & right motors for 2 seconds then stop
 */
void MainRobot::Autonomous()
{
	m_drive->SetSafetyEnabled(false);
	m_drive->Drive(-0.5, 0.0); 	// drive forwards half speed
	Wait(2.0); 				//    for 2 seconds
	m_drive->Drive(0.0, 0.0); 	// stop robot
}

void MainRobot::OperatorControl()
{
	m_drive->SetSafetyEnabled(true);
	while (IsOperatorControl())
	{
		m_drive->TankDrive(m_leftStick, m_rightStick); // drive with arcade style (use right stick)
		
		if (m_rightStick->GetRawButton(2)) {
			m_claw->Open();
		} else if (m_rightStick->GetRawButton(3)) {
			m_claw->Close();
		} else if (m_rightStick->GetRawButton(4)) {
			m_claw->Stop();
		}
		
		Wait(0.005);				// wait for a motor update time
	}
}

void MainRobot::Test() {

}

START_ROBOT_CLASS(MainRobot);


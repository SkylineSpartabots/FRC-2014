#include "Main.h"

MainRobot::MainRobot()
{
	InitializeHardware();
	InitializeSoftware();
	
	m_drive->SetExpiration(0.1);
}

void MainRobot::InitializeHardware() {
	m_drive = new RobotDrive(Ports::DigitalSidecar::Pwm8, 
							 Ports::DigitalSidecar::Pwm7, 
							 Ports::DigitalSidecar::Pwm10, 
							 Ports::DigitalSidecar::Pwm9);
	m_leftStick = new Joystick(Ports::Computer::Usb1);
	m_rightStick = new Joystick(Ports::Computer::Usb2);
	
	m_compressor = new Compressor(Ports::DigitalSidecar::Gpio1,
								  Ports::DigitalSidecar::Relay1);
	m_compressor->Start();
	m_solenoid = new Solenoid(Ports::Crio::SolenoidBreakout1);
	
	//m_clawMotor = new Victor(Ports::DigitalSidecar::Pwm5);
	
	m_shooterLeft1 = new Talon(Ports::DigitalSidecar::Pwm3);
	m_shooterLeft2 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_shooterRight1 = new Talon(Ports::DigitalSidecar::Pwm5);
	m_shooterRight2 = new Talon(Ports::DigitalSidecar::Pwm6);
	m_shooterLimitSwitch = new DigitalInput(Ports::DigitalSidecar::Gpio2);
}

void MainRobot::InitializeSoftware() {
	//m_claw = new Claw(m_clawMotor);
	m_shooter = new Shooter(m_shooterLeft1, m_shooterLeft2, m_shooterRight1, m_shooterRight2, m_shooterLimitSwitch);
}

void MainRobot::Autonomous()
{
	m_drive->SetSafetyEnabled(false);
	m_drive->Drive(-0.5, 0.0); 	// Drive forwards half speed
	Wait(2.0); 					// For 2 seconds
	m_drive->Drive(0.0, 0.0); 	// Stop robot
	
	AxisCamera &camera = AxisCamera::GetInstance("10.29.76.11");	//To use the Axis camera uncomment this line
	
	int autonomousLifetime = 0;
	while (IsAutonomous() && IsEnabled()) {
		ColorImage *image;
		//image = new RGBImage("/testImage.jpg");	// get the image from the cRIO flash
		image = camera.GetImage();					// Get the image from the Camera
		
		if ((image == (void *) 0) || (image->GetWidth() == 0) || (image->GetHeight() == 0)) {
			continue;
		}
		
		SmartDashboard::PutNumber("Autonomous Lifetime", ++autonomousLifetime);
		
		TargetReport* report = Vision::process(image);
		SmartDashboard::PutBoolean("Target Hot", report->Hot);
		SmartDashboard::PutNumber("Target Distance", report->distance/1.0);
		SmartDashboard::PutNumber("Particle Reports", report->reports);
		SmartDashboard::PutNumber("Left Score", report->leftScore);
		SmartDashboard::PutNumber("Right Score", report->rightScore);
		
		Wait(0.5);
	}
	
}

void MainRobot::OperatorControl()
{
	m_drive->SetSafetyEnabled(true);
	
	while (IsOperatorControl())
	{
		m_drive->TankDrive(m_leftStick, m_rightStick); // drive with arcade style (use right stick)
		
		if (m_rightStick->GetRawButton(5)) {
			m_solenoid->Set(true);
		} else if (m_rightStick->GetRawButton(6)) {
			m_solenoid->Set(false);
		}
		/*if (m_rightStick->GetRawButton(2)) {
			m_claw->Open();
		} else if (m_rightStick->GetRawButton(3)) {
			m_claw->Close();
		} else if (m_rightStick->GetRawButton(4)) {
			m_claw->Stop();
		}*/
		
		Wait(0.005);				// wait for a motor update time
	}
}

void MainRobot::Test() {

}

START_ROBOT_CLASS(MainRobot);


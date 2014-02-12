#include "Main.h"

#define CONTROLLER XBOX

#define XBOX 1
#define JOYSTICKS 2

MainRobot::MainRobot()
{
	InitializeHardware();
	InitializeSoftware();
	
	m_drive->SetExpiration(0.1);
}

void MainRobot::InitializeHardware()
{
	m_drive = new RobotDrive(Ports::DigitalSidecar::Pwm8, 
							 Ports::DigitalSidecar::Pwm7, 
							 Ports::DigitalSidecar::Pwm10, 
							 Ports::DigitalSidecar::Pwm9);
	
	if (CONTROLLER == XBOX) {
		xbox = new XboxController(1);
	} else if (CONTROLLER == JOYSTICKS) {
		m_leftStick = new Joystick(Ports::Computer::Usb1);
		m_rightStick = new Joystick(Ports::Computer::Usb2);
	}

	m_compressor = new Compressor(Ports::DigitalSidecar::Gpio1,
								  Ports::DigitalSidecar::Relay1);
	m_compressor->Start();
	
	m_solenoid1 = new Solenoid(Ports::Crio::SolenoidBreakout1);
	m_solenoid2 = new Solenoid(Ports::Crio::SolenoidBreakout2);
	m_solenoid3 = new Solenoid(Ports::Crio::SolenoidBreakout3);
	m_solenoid4 = new Solenoid(Ports::Crio::SolenoidBreakout4);
	
	//m_clawMotor = new Victor(Ports::DigitalSidecar::Pwm5);
	
	m_collectorMotor = new Victor(Ports::DigitalSidecar::Pwm1);
	m_shooterLeft1 = new Talon(Ports::DigitalSidecar::Pwm3);
	m_shooterLeft2 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_shooterRight1 = new Talon(Ports::DigitalSidecar::Pwm5);
	m_shooterRight2 = new Talon(Ports::DigitalSidecar::Pwm6);
	m_shooterLimitSwitchBottom = new DigitalInput(Ports::DigitalSidecar::Gpio2);
	m_shooterLimitSwitchTop = new DigitalInput(Ports::DigitalSidecar::Gpio3);
}

void MainRobot::InitializeSoftware()
{
	//m_claw = new Claw(m_clawMotor);
	m_collector = new Collector(m_collectorMotor, m_solenoid1, m_solenoid2, m_solenoid3, m_solenoid4, m_compressor);
	m_shooter = new Shooter(m_shooterLeft1, m_shooterLeft2, m_shooterRight1,
			m_shooterRight2, m_shooterLimitSwitchBottom, m_shooterLimitSwitchTop, m_collector);
}


//bool autonomousDidShoot = false;
void MainRobot::Autonomous()
{
	m_drive->SetSafetyEnabled(false);
	m_drive->Drive(-0.5, 0.0);
	Wait(2.5);
	m_drive->Drive(0.0, 0.0);
	
	AxisCamera &camera = AxisCamera::GetInstance("10.29.76.11");
	
	int autonomousLifetime = 0;
	while (IsAutonomous() && IsEnabled()) {
		ColorImage *image = camera.GetImage();					// Get the image from the Camera
		
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
		/*
		if (!autonomousDidShoot && report->Hot) {
			m_shooter->Shoot();
		}*/
		
		Wait(0.5);
	}
}


void MainRobot::OperatorControl()
{
	m_drive->SetSafetyEnabled(true);

	int operatorControlLifetime = 0;
	while (IsOperatorControl()) {
		SmartDashboard::PutNumber("Operator Lifetime", ++operatorControlLifetime);
		
		if (CONTROLLER == XBOX) {
			float leftY = Cutoff(xbox->GetAxis(xbox->LeftY));
			float rightY = Cutoff(xbox->GetAxis(xbox->RightY));
			rightY = ((-0.1 <= rightY) && (rightY <= 0.1)) ? 0 : rightY;
			m_drive->TankDrive(leftY, rightY);
			
			if (xbox->GetLeftBumperButton()) {
				m_collector->PistonPull();
			} else if (xbox->GetRightBumperButton()) {
				m_collector->PistonPush();
				Wait(0.5);
				m_collector->PistonNeutral();
			}
			if(xbox->GetAButton()){
				m_collector->SpinInwards();
			}
			if(xbox->GetBButton()){
				m_collector->SpinOutwards();
			}
			if(xbox->GetYButton()){
				m_collector->SpinStop();
			}
			
			float bumper = xbox->GetAxis(xbox->Bumper); 
			leftY = ((-0.1 <= leftY) && (leftY <= 0.1)) ? 0 : leftY;
			if(bumper >= 0.4){
				m_shooter->shootWithArm();  
			}
			
		} else if (CONTROLLER == JOYSTICKS) {
			m_drive->TankDrive(m_leftStick, m_rightStick);
			
			// Xbox is a higher priority, do this later
		}
		
		Wait(0.005); // wait for a motor update time
	}
}

void MainRobot::RobotInit()
{
	SmartDashboard::PutBoolean("RobotInit", true);
}

void MainRobot::Test()
{
	
}

float MainRobot::Cutoff(float num)
{
	if ((-0.1 <= num) && (num <= 0.1)) {
		num = 0;
	}
	return num;
}

START_ROBOT_CLASS(MainRobot);


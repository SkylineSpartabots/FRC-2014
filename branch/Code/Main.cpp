#include "Main.h"
#define CONTROLLER XBOX

#define XBOX 1
#define JOYSTICKS 2 // Joystick controlling not implemented

// Constructor: Called before RobotInit(); WPILIB is not guaranteed to be fully working here
MainRobot::MainRobot()
{
	InitializeHardware();
	InitializeSoftware();
	
	m_drive->SetExpiration(0.1);
}

// RobotInit: Called after WPILIB are guaranteed to be initialized
void MainRobot::RobotInit()
{
	
}

// InitializeHardware: Objects for interacting with hardware are initialized here
void MainRobot::InitializeHardware()
{
	m_drive = new RobotDrive(Ports::DigitalSidecar::Pwm8, 
							 Ports::DigitalSidecar::Pwm7, 
							 Ports::DigitalSidecar::Pwm10, 
							 Ports::DigitalSidecar::Pwm9);
	
	if (CONTROLLER == XBOX) {
		driveController = new XboxController(Ports::Computer::Usb2);
		shootController = new XboxController(Ports::Computer::Usb1);
	} else if (CONTROLLER == JOYSTICKS) {
		m_leftStick = new Joystick(Ports::Computer::Usb1);
		m_rightStick = new Joystick(Ports::Computer::Usb2);
	}

	m_compressor = new Compressor(Ports::DigitalSidecar::Gpio5,
								  Ports::DigitalSidecar::Relay1);
	m_compressor->Start();
	
	m_solenoid1 = new Solenoid(Ports::Crio::SolenoidBreakout1);
	m_solenoid2 = new Solenoid(Ports::Crio::SolenoidBreakout2);
	m_solenoid3 = new Solenoid(Ports::Crio::SolenoidBreakout3);
	m_solenoid4 = new Solenoid(Ports::Crio::SolenoidBreakout4);
	
	//m_clawMotor = new Victor(Ports::DigitalSidecar::Pwm5);
	
	m_collectorMotor = new Victor(Ports::DigitalSidecar::Pwm1);
	m_shooterLeft1 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_shooterLeft2 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_shooterRight1 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_shooterRight2 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_shooterLimitSwitchBottom = new DigitalInput(Ports::DigitalSidecar::Gpio4);
	m_shooterLimitSwitchTop = new DigitalInput(Ports::DigitalSidecar::Gpio3);

	m_motor = new Victor(Ports::Crio::Module2,Ports::DigitalSidecar::Pwm2);
	m_motorEncoder = new Encoder(Ports::Crio::Module2, Ports::DigitalSidecar::Gpio1, Ports::Crio::Module2, Ports::DigitalSidecar::Gpio2);

}

// InitializeSoftware: Initialize subsystems
void MainRobot::InitializeSoftware()
{
	//m_claw = new Claw(m_clawMotor);
	m_collector = new Collector(m_collectorMotor, m_solenoid1, m_solenoid2, m_solenoid3, m_solenoid4, m_compressor);
	m_shooter = new Shooter(m_shooterLeft1, m_shooterLeft2, m_shooterRight1,
			m_shooterRight2, m_shooterLimitSwitchBottom, m_shooterLimitSwitchTop, m_collector);
}

bool autonomousDidShoot = false;
void MainRobot::Autonomous()
{
	// Drive foward at 0.5 for 2.5 seconds
	m_drive->SetSafetyEnabled(false);
	m_drive->Drive(-0.5, 0.0);
	Wait(2.5);
	m_drive->Drive(0.0, 0.0); // Stop driving
	
	AxisCamera &camera = AxisCamera::GetInstance("10.29.76.11");
	
	// Inside this while loop, the ribit will check if the best detected target is hot, if not then it
	// will wait until it is hot, once it is hot, it will shoot. Once it shoots, it will not attempt
	// to shoot again
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
		
		if (!autonomousDidShoot && report->Hot) {
			m_shooter->Shoot();
      		}
		
		Wait(0.5);
	}
}

bool isShooting = false;
void MainRobot::OperatorControl()
{
	while (IsOperatorControl()) {
		SmartDashboard::PutNumber("Encoder", m_motorEncoder->Get());
		SmartDashboard::PutNumber("EncoderRaw", m_motorEncoder->GetRaw());

		if (shootController->GetAButton()) {
			m_motor->Set(.3);
		} else if (shootController->GetBButton()) {
			m_motor->Set(0);
		}
	}
	
	/*
	m_drive->SetSafetyEnabled(true);
	
	int operatorControlLifetime = 0;
	while (IsOperatorControl()) {
		SmartDashboard::PutNumber("Operator Lifetime", ++operatorControlLifetime);	
		
		if (CONTROLLER == XBOX) {
			// DRIVING
			
			// (for the two code lines below) The minus in front makes the robot drive in the
			// direction of the collector (when joysticks are forward)
			float leftY = -Cutoff(driveController->GetAxis(driveController->LeftY));
			float rightY = -Cutoff(driveController->GetAxis(driveController->RightY));
			
			// Drive reversal when left trigger down (go opposite direction)
			if (driveController->GetAxis(driveController->Bumper) >= 0.4) {
				leftY = -leftY;
				rightY = -rightY;
			}
			
			SmartDashboard::PutNumber("Triggers", driveController->GetAxis(driveController->Bumper));
			
			m_drive->TankDrive(leftY, rightY);
			
			// PISTON BUTTONS (FOR COLLECTOR)
			if (driveController->GetLeftBumperButton()) {
				m_collector->PistonPull();
			} else if (driveController->GetRightBumperButton()) {
				m_collector->PistonPush();
				Wait(0.5);
				m_collector->PistonNeutral();
			}
			
			// SPIN BUTTONS (FOR COLLECTOR)
			if (driveController->GetAButton()){
				m_collector->SpinInwards();
			} else if (driveController->GetBButton()){
				m_collector->SpinOutwards();
			} else if (driveController->GetYButton()){
				m_collector->SpinStop();
			}
			
			// SHOOTING
			
			if (shootController->GetAButton()) {
				m_shooter->Shoot();
			} else if (shootController->GetBButton()) {
				m_shooter->Stop();
			}
			
			float bumper = shootController->GetAxis(shootController->Bumper); 
			if (bumper >= 0.4){
				if (!isShooting) {
					m_shooter->shootWithArm();  // This is a blocking function, it will stop the main loop
												// until it finishes executing. We'll need a separate thread
												// if we want it to not temporarily stop the main loop
					isShooting = true;
				}
			} else {
				isShooting = false;
			}
			
		} else if (CONTROLLER == JOYSTICKS) { // Xbox is a higher priority, do this later
			m_drive->TankDrive(m_leftStick, m_rightStick);
			
		}
		
		Wait(0.005); // wait for a motor update time
	}
	*/
}

void MainRobot::Test()
{
    int count = 0;
	while (1) {
	//	SmartDashboard::PutNumber("Encoder", m_motorEncoder->Get());
	//	SmartDashboard::PutNumber("EncoderRaw", m_motorEncoder->GetRaw());

		if (count > 10 && count <30){
			m_motor->Set(.3);
		}
		else
		{
			m_motor->Set(0);
		}
		Wait(.25);
		count++;
	}

}

float MainRobot::Cutoff(float num)
{
	if ((-0.1 <= num) && (num <= 0.1)) {
		num = 0;
	}
	return num;
}

START_ROBOT_CLASS(MainRobot);


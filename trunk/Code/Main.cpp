#include "Main.h"

#define XBOX 1
#define JOYSTICKS 2 // Joystick controlling not implemented
#define TANK 1
#define ARCADE 2

int CONTROLLER = XBOX;
int DRIVING = TANK;

MainRobot::MainRobot() {
	InitializeHardware();
	InitializeSoftware();
	
	m_drive->SetExpiration(0.1);
}

void MainRobot::RobotInit() {
	
}

// InitializeHardware: Objects for interacting with hardware are initialized here
void MainRobot::InitializeHardware()
{
	m_drive = new RobotDrive(Ports::DigitalSidecar::Pwm8, 
							 Ports::DigitalSidecar::Pwm7, 
							 Ports::DigitalSidecar::Pwm10, 
							 Ports::DigitalSidecar::Pwm9);
	
	if (CONTROLLER == XBOX) {
		driveController = new XboxController(Ports::Computer::Usb1);
		shootController = new XboxController(Ports::Computer::Usb2);
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
	m_shooterLeft1 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_shooterLeft2 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_shooterRight1 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_shooterRight2 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_pistonLimitSwitch = new DigitalInput(Ports::DigitalSidecar::Gpio11);
	m_shooterLimitSwitch = new DigitalInput(Ports::DigitalSidecar::Gpio12);
}

// InitializeSoftware: Initialize subsystems
void MainRobot::InitializeSoftware()
{
	//m_claw = new Claw(m_clawMotor);
	m_collector = new Collector(m_collectorMotor, m_solenoid1, m_solenoid2, m_solenoid3, m_solenoid4, m_compressor, m_pistonLimitSwitch);
	m_shooter = new Shooter(m_shooterLeft1, m_shooterLeft2, m_shooterRight1,
			m_shooterRight2, m_shooterLimitSwitch, m_collector);
	netTable = NetworkTable::GetTable("VisionTargetInfo");
	netTable->PutNumber("Driving", DRIVING);
}

bool autonomousDidShoot = false;
void MainRobot::Autonomous()
{
	m_collector->BringArmDown();
	m_shooter->BringArmDown();
	
	m_collector->SpinInwards();
	Wait(4.5);
	
	// Drive foward at 0.5 for 2.5 seconds
	m_drive->SetSafetyEnabled(false);
	m_drive->Drive(-0.5, 0.0);
	Wait(1.2);
	m_drive->Drive(0.0, 0.0); // Stop driving
	
	m_shooter->ShootWithArm();
	
	/*AxisCamera &camera = AxisCamera::GetInstance("10.29.76.11");
	
	// Inside this while loop, the ribit will check if the best detected target is hot, if not then it
	// will wait until it is hot, once it is hot, it will shoot. Once it shoots, it will not attempt
	// to shoot again
	int autonomousLifetime = 0;
	while (IsAutonomous() && IsEnabled()) {
		ColorImage *image = camera.GetImage();
		
		if ((image == (void *) 0) || (image->GetWidth() == 0) || (image->GetHeight() == 0)) {
			continue;
		}
		
		SmartDashboard::PutNumber("Autonomous Lifetime", ++autonomousLifetime);
		
		TargetReport* report = Vision::process(image);
		netTable->PutBoolean("Target Hot", report->Hot);
		netTable->PutNumber("Target Distance", report->distance);
		
		if (!autonomousDidShoot && report->Hot) {
			autonomousDidShoot = true;
			m_shooter->ShootWithArm();
		}
		
		Wait(0.5);
	}*/
}

bool isShooting = false;
int nextImageCheck = 0;
void MainRobot::OperatorControl()
{
	m_drive->SetSafetyEnabled(true);
	int operatorControlLifetime = 0;
	AxisCamera &camera = AxisCamera::GetInstance("10.29.76.11");
	
	while (IsOperatorControl()) {
		int driving = (int) netTable->GetNumber("Driving");
		if (driving != DRIVING) {
			DRIVING = driving;
		}
		SmartDashboard::PutNumber("Operator Lifetime", ++operatorControlLifetime);	
		
		nextImageCheck++;
		if (nextImageCheck >= 800) {
			nextImageCheck = 0;
			ColorImage *image = camera.GetImage();
					
			if ((image == (void *) 0) || (image->GetWidth() == 0) || (image->GetHeight() == 0)) {
				continue;
			}
			
			TargetReport* report = Vision::process(image);
			netTable->PutBoolean("Target Hot", report->Hot);
			netTable->PutNumber("Target Distance", report->distance);
		}
		
		if (CONTROLLER == XBOX) {
			// DRIVING
			// ----------------------------------------------------------------------
			if (DRIVING == TANK) {
				// (for the two code lines below) The minus in front makes the robot drive in the
				// direction of the collector (when joysticks are forward)
				float leftY = -Cutoff(driveController->GetAxis(driveController->LeftY));
				float rightY = -Cutoff(driveController->GetAxis(driveController->RightY));
				
				// Drive reversal when left trigger down (go opposite direction)
				if (driveController->GetTriggerAxis() >= 0.4) {
					leftY = -leftY;
					rightY = -rightY;
				}
				
				m_drive->TankDrive(leftY, rightY);
			} else if (DRIVING == ARCADE) {
				float arcadeY = -Cutoff(driveController->GetLeftYAxis());
				float arcadeX = -Cutoff(driveController->GetLeftXAxis());
				
				if (arcadeY == 0 && arcadeX == 0) {
					arcadeY = -Cutoff(driveController->GetRightYAxis());
					arcadeX = -Cutoff(driveController->GetRightXAxis());
				}
				
				// Drive reversal when left trigger down (go opposite direction)
				if (driveController->GetTriggerAxis() >= 0.4) {
					arcadeY = -arcadeY;
					arcadeX = -arcadeX;
				}
				
				m_drive->ArcadeDrive(arcadeY, arcadeX);
			}
				
			// PISTON BUTTONS (FOR COLLECTOR)
			// ----------------------------------------------------------------------
			if (shootController->GetLeftBumperButton()) {
				m_collector->PistonPull();
			} else if (shootController->GetRightBumperButton()) {
				if (m_shooter->GetLimitSwitch()) {
					m_collector->BringArmDown();
				}
			}
				
			// SPIN BUTTONS (FOR COLLECTOR)
			// ----------------------------------------------------------------------
			if (shootController->GetXButton()){
				m_collector->SpinInwards();
			} else if (shootController->GetYButton()){
				m_collector->SpinOutwards();
			} else {
				m_collector->SpinStop();
			}
			
			// SHOOTING
			// ----------------------------------------------------------------------
			// The Set() values below (-.15 & .15) indicate the power
			// to the motor (15%) and are very important to understand
			// before changing. It controls the speed of the shooter
			// arm while the button is pressed. If the values are too high,
			// you run the risk of wrapping the arm into the robot
			// (or around the robot).
			
			if (shootController->GetAButton()) {
				m_shooter->Set(-.15);
			} else if (shootController->GetBButton()) {
				m_shooter->Set(.15);
			} else {
				m_shooter->Set(0);
			}
			
			float trigger = shootController->GetTriggerAxis();
			if (trigger >= 0.4){
				if (!isShooting) {
					m_shooter->ShootWithArm();
					isShooting = true;
				}
			} else {
				isShooting = false;
			}
			
		} else if (CONTROLLER == JOYSTICKS) { // Xbox is a higher priority, do this later
			if (DRIVING == TANK) {
				float leftY = -Cutoff(m_leftStick->GetY());
				float rightY = -Cutoff(m_leftStick->GetX());
				
				m_drive->TankDrive(leftY, rightY);
			} else if (DRIVING == ARCADE) {
				float arcadeY = -Cutoff(m_leftStick->GetY());
				float arcadeX = -Cutoff(m_leftStick->GetX());
				
				if (arcadeY == 0 && arcadeX == 0) {
					arcadeY = -Cutoff(m_rightStick->GetY());
					arcadeX = -Cutoff(m_rightStick->GetX());
				}
				
				m_drive->ArcadeDrive(arcadeY, arcadeX);
			}
			
		}
		
		Wait(0.005); // wait for a motor update time
	}
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


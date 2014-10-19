#include "MainRobot.h"

#define XBOX 1
#define JOYSTICKS 2 // Joystick controlling not implemented
#define TANK 1
#define ARCADE 2
#define ARCADE2 3

int CONTROLLER = XBOX;
int DRIVING = ARCADE2;

float autonomousDriveMagnitude = 0;
float autonomousDriveCurve = 0;

MainRobot::MainRobot() {
	InitializeHardware();
	InitializeSoftware();
}

// Use only in autonomous mode
void MainRobot::WatchdogWait(double time) {
	Timer* timer = new Timer();
	timer->Start();
	while (true) {
		RobotBase::getInstance().GetWatchdog().Feed();
		m_drive->Drive(autonomousDriveMagnitude, autonomousDriveCurve);
		if (timer->Get() >= time) {
			break;
		}
		Wait(.05);
	}
	timer->Stop();
	delete timer;
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
	m_collectorMotor->SetSafetyEnabled(true);
	m_shooterMotors = new Talon(Ports::DigitalSidecar::Pwm4);   // This one Talon object powers
																// all four shooter motors
	
	m_pistonLimitSwitch = new DigitalInput(Ports::DigitalSidecar::Gpio3);
	m_shooterLimitSwitch = new DigitalInput(Ports::DigitalSidecar::Gpio12);
}

// InitializeSoftware: Initialize subsystems
void MainRobot::InitializeSoftware()
{
	//m_claw = new Claw(m_clawMotor);
	m_collector = new Collector(m_collectorMotor, m_solenoid1, m_solenoid2,
			m_solenoid3, m_solenoid4, m_compressor, m_pistonLimitSwitch, m_drive);
	m_shooter = new Shooter(m_shooterMotors, m_shooterLimitSwitch, m_collector, m_drive);
}

void MainRobot::AutonomousDrive(float magnitude, float curve) {
	m_drive->Drive(magnitude, curve);
	autonomousDriveMagnitude = magnitude;
	autonomousDriveCurve = curve;
}

void MainRobot::Autonomous()
{
	// DO NOT use m_drive in autonomous mode, use the MainRobot#AutonomousDrive(float magnitude, float curve) instead
	
	// Bring collector arm down
	if (!m_collector->BringArmDown())
		return;
	// Bring shooter arm down to let ball drop into cradle
	if (!m_shooter->BringArmDown())
		return;
	
	// Drive towards goal
	AutonomousDrive(-0.8, 0.0);
	WatchdogWait(0.65);
	AutonomousDrive(0.0, 0.0);
	
	// Shoot
	WatchdogWait(1.75);
	m_shooter->ShootWithArm(false, 0.825);
	
	// DRIVE FORWARD ONLY CODE
	// ----------------------------------------------------------------------
	/*
	AutonomousDrive(-0.8, 0.0);
	Wait(1);
	AutonomousDrive(0.0, 0.0);*/
}

bool isShooting = false;
void MainRobot::OperatorControl()
{
	int operatorControlLifetime = 0;
	while (IsOperatorControl()) {
		SmartDashboard::PutNumber("Operator Lifetime", ++operatorControlLifetime);	
		if (CONTROLLER == XBOX) {
			// DRIVING
			// ----------------------------------------------------------------------
			if (DRIVING == TANK) {
				// (for the two code lines below) The minus in front makes the robot drive in the
				// direction of the collector (when joysticks are forward)
				float leftY = -Cutoff(driveController->GetAxis(driveController->LeftY));
				float rightY = -Cutoff(driveController->GetAxis(driveController->RightY));
				
				// Drive reversal when right trigger down (go opposite direction)
				if (driveController->GetTriggerAxis() <= -0.4) {
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
				
				// Drive reversal when right trigger down (go opposite direction)
				if (driveController->GetTriggerAxis() <= -0.4) {
					arcadeY = -arcadeY;
					//arcadeX = -arcadeX;
				}
				
				m_drive->ArcadeDrive(arcadeY, arcadeX);
			} else if (DRIVING == ARCADE2) {
				float arcadeY = -Cutoff(driveController->GetLeftYAxis());
				float arcadeX = -Cutoff(driveController->GetRightXAxis());
				
				// Drive reversal when right trigger down (go opposite direction)
				if (driveController->GetTriggerAxis() <= -0.4) {
					arcadeY = -arcadeY;
					//arcadeX = -arcadeX; // May need to be uncommented?
				}
				
				m_drive->ArcadeDrive(arcadeY, arcadeX);
			}
				
			// PISTON BUTTONS (FOR COLLECTOR)
			// ----------------------------------------------------------------------
			if (shootController->GetLeftBumperButton()) {
				SmartDashboard::PutBoolean("ShootController LB", true);
				m_collector->PistonPull();
			} else if (shootController->GetRightBumperButton()) {

				SmartDashboard::PutBoolean("ShootController RB", true);
				m_collector->BringArmDown();
				if (m_shooter->GetLimitSwitch()) {
					m_collector->BringArmDown();
				}
			} else {
				SmartDashboard::PutBoolean("ShootController LB", false);
				SmartDashboard::PutBoolean("ShootController RB", false);
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
			
			// PASSING
			// ----------------------------------------------------------------------
			if(driveController->GetRightBumperButton()) {
				m_shooter->ShooterPass();
			}
			
			// SHOOTER ARM LIFT / LOWER SLOWLY
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
			
			// SHOOTING
			// ----------------------------------------------------------------------
			
			float trigger = shootController->GetTriggerAxis();
			if (trigger <= -0.4){ // High shot
				if (!isShooting) {
					m_compressor->Stop();
					isShooting = true;
					m_shooter->ShootWithArm(false, 1);
					m_compressor->Start();
				}
			} else if (trigger >= 0.4){ // Low shot
				if (!isShooting) {
					m_compressor->Stop();
					isShooting = true;
					m_shooter->ShootWithArm(true, 0.9);
					m_compressor->Start();
				}
			} else {
				isShooting = false;
			}
		} else if (CONTROLLER == JOYSTICKS) { // Xbox is a higher priority, do this later/never
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
		SmartDashboard::PutBoolean("Pistons extended", m_collector->IsExtended ());
		SmartDashboard::PutBoolean("shooter limit switch", m_shooter->GetLimitSwitch());
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

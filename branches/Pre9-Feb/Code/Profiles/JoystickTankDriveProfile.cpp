#include "JoystickTankDriveProfile.h"

JoystickTankDriveProfile::JoystickTankDriveProfile() {
	InitializeHardware();
	InitializeSoftware();
	
	m_drive->SetExpiration(0.1);
}

void JoystickTankDriveProfile::OperatorControl(void){
	m_drive->SetSafetyEnabled(true);
		
	while (this->IsOperatorControl())
	{
		m_drive->TankDrive(m_leftStick, m_rightStick);
		
		Wait(0.005); // wait for a motor update time
	}
}


void JoystickTankDriveProfile::InitializeHardware() {
	m_drive = new RobotDrive(Ports::DigitalSidecar::Pwm8, 
							 Ports::DigitalSidecar::Pwm7, 
							 Ports::DigitalSidecar::Pwm10, 
							 Ports::DigitalSidecar::Pwm9);
	m_leftStick = new Joystick(Ports::Computer::Usb1);
	m_rightStick = new Joystick(Ports::Computer::Usb2);
	
	m_compressor = new Compressor(Ports::DigitalSidecar::Gpio1,
								  Ports::DigitalSidecar::Relay1);
	m_compressor->Start();
	m_solenoid1 = new Solenoid(Ports::Crio::SolenoidBreakout1);
	m_solenoid2 = new Solenoid(Ports::Crio::SolenoidBreakout2);
	m_solenoid3 = new Solenoid(Ports::Crio::SolenoidBreakout3);
	m_solenoid4 = new Solenoid(Ports::Crio::SolenoidBreakout4);
	
	//m_clawMotor = new Victor(Ports::DigitalSidecar::Pwm5);
	
	m_shooterLeft1 = new Talon(Ports::DigitalSidecar::Pwm3);
	m_shooterLeft2 = new Talon(Ports::DigitalSidecar::Pwm4);
	m_shooterRight1 = new Talon(Ports::DigitalSidecar::Pwm5);
	m_shooterRight2 = new Talon(Ports::DigitalSidecar::Pwm6);
	m_shooterLimitSwitchBottom = new DigitalInput(Ports::DigitalSidecar::Gpio2);
	m_shooterLimitSwitchTop = new DigitalInput(Ports::DigitalSidecar::Gpio3);
}

void JoystickTankDriveProfile::InitializeSoftware() {
	//m_claw = new Claw(m_clawMotor);
	m_collector = new Collector(m_collectorMotor, m_solenoid1, m_solenoid2, m_solenoid3, m_solenoid4, m_compressor);
	m_shooter = new Shooter(m_shooterLeft1, m_shooterLeft2, m_shooterRight1,
			m_shooterRight2, m_shooterLimitSwitchBottom, m_shooterLimitSwitchTop, m_collector);
}

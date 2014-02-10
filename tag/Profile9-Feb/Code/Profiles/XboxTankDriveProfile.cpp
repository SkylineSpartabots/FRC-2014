#include "XboxTankDriveProfile.h"

XboxTankDriveProfile::XboxTankDriveProfile() {
	InitializeHardware();
	InitializeSoftware();
	
	m_drive->SetExpiration(0.1);
}

void XboxTankDriveProfile::Autonomous(void){
}

void XboxTankDriveProfile::OperatorControl(void){
	m_drive->SetSafetyEnabled(true);
		
	while (this->IsOperatorControl())
	{
		float leftY = Cutoff(xbox->GetAxis(xbox->LeftY));
		float rightY = Cutoff(xbox->GetAxis(xbox->RightY));
		m_drive->TankDrive(leftY, rightY);
		
		if (xbox->GetLeftBumperButton()) {
			m_collector->PistonPull();
		} else if (xbox->GetRightBumperButton()) {
			m_collector->PistonPush();
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
		/*
		float bumper = Cutoff(xbox->GetAxis(xbox->Bumper)); 
		if(bumper >= 0.4){
			m_shooter->shootWithArm();  
		}*/
		
		Wait(0.005); // wait for a motor update time
	}
}

float XboxTankDriveProfile::Cutoff(float num)
{
	if ((-0.1 <= num) and (num <= 0.1)) {
			num = 0;
	}
	return num;
}

void XboxTankDriveProfile::InitializeHardware() {
	m_drive = new RobotDrive(Ports::DigitalSidecar::Pwm8, 
							 Ports::DigitalSidecar::Pwm7, 
							 Ports::DigitalSidecar::Pwm10, 
							 Ports::DigitalSidecar::Pwm9);
	xbox = new XboxController(1);
	
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

void XboxTankDriveProfile::InitializeSoftware() {
	//m_claw = new Claw(m_clawMotor);
	m_collector = new Collector(m_collectorMotor, m_solenoid1, m_solenoid2, m_solenoid3, m_solenoid4, m_compressor);
	m_shooter = new Shooter(m_shooterLeft1, m_shooterLeft2, m_shooterRight1,
			m_shooterRight2, m_shooterLimitSwitchBottom, m_shooterLimitSwitchTop, m_collector);
}

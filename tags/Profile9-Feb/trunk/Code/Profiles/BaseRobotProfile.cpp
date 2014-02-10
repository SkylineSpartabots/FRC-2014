#include "BaseRobotProfile.h"

bool autonomousDidShoot = false;
void BaseRobotProfile::Autonomous(void) {
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
		
		if (!autonomousDidShoot && report->Hot) {
			m_shooter->Shoot();
		}
		
		Wait(0.5);
	}
	
}

void BaseRobotProfile::OperatorControl(void) {
}

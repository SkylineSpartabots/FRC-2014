#ifndef XBOX_TANK_DRIVE_PROFILE_H
#define XBOX_TANK_DRIVE_PROFILE_H

#include "BaseRobotProfile.h"
#include "../Ports.h"
#include "../Subsystems/Shooter/Shooter.h"
#include "../Subsystems/Collector/Collector.h"
#include "../Subsystems/Controllers/XboxController.h"

class XboxTankDriveProfile : public BaseRobotProfile {
	XboxController *xbox;
public:
	XboxTankDriveProfile();
	void InitializeHardware();
	void InitializeSoftware();
	void Autonomous(void);
	void OperatorControl(void);
	float Cutoff(float num);
};

#endif

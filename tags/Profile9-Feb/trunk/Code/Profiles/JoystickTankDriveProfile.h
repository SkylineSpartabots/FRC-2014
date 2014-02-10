#ifndef XBOX_PROFILE_H
#define XBOX_PROFILE_H

#include "BaseRobotProfile.h"
#include "../Ports.h"
#include "../Subsystems/Shooter/Shooter.h"
#include "../Subsystems/Collector/Collector.h"

class JoystickTankDriveProfile : public BaseRobotProfile {
public:
	JoystickTankDriveProfile();
	void InitializeHardware();
	void InitializeSoftware();
	void Autonomous(void);
	void OperatorControl(void);
	float Cutoff(float num);
};

#endif

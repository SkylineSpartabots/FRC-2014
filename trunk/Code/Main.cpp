// Modify these lines below:

#define ROBOT XBOX
//#define ROBOT JOYSTICK_TANKDRIVE

// DO NOT MODIFY BELOW

#include "Profiles/XboxTankDriveProfile.h"
#include "Profiles/JoystickTankDriveProfile.h"

#define XBOX_TANKDRIVE 1
#define JOYSTICK_TANKDRIVE 2

#if ROBOT == XBOX_TANKDRIVE
        START_ROBOT_CLASS(XboxTankDriveProfile);
#elif ROBOT == JOYSTICK_TANKDRIVE
        START_ROBOT_CLASS(XboxTankDriveProfile);
#endif

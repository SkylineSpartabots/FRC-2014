#ifndef _XBOX_CONTROLLER_H
#define _XBOX_CONTROLLER_H

#include "WPILib.h"

/**
 * A class representing a physical XboxController, analogous to a Joystick.
 */
class XboxController : public Joystick
{
public:
        enum Axis {
                LeftX = 1,
                LeftY = 2,
                Trigger = 3,
                RightX = 4,
                RightY = 5
        };
        enum Dpad {
                XDir = 6,
                YDir = 7
        };
        enum Button {
                A = 1,
                B = 2,
                X = 3,
                Y = 4,
                LeftBumper = 5,
                RightBumper = 6,
                Back = 7,
                Start = 8,
                LeftStickClick = 9,
                RightStickClick = 10
        };
        
        static const UINT32 numAxisTypes = 7;
        static const UINT32 numButtonTypes = 10;
        
        XboxController(UINT32);
        
        float GetAxis(Axis);
        int GetDpad(Dpad);
        bool GetButton(Button);
        
        float GetLeftXAxis();
        float GetLeftYAxis();
        float GetTriggerAxis();
        float GetRightXAxis();
        float GetRightYAxis();
        
        int GetXDirDpad();
        int GetYDirDpad();
        
        bool GetAButton();
        bool GetBButton();
        bool GetXButton();
        bool GetYButton();
        bool GetLeftBumperButton();
        bool GetRightBumperButton();
        bool GetBackButton();
        bool GetStartButton();
        bool GetLeftStickClickButton();
        bool GetRightStickClickButton();
};

#endif

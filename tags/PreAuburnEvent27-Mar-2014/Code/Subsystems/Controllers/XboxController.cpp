#include "XboxController.h"

XboxController::XboxController(UINT32 port) : 
                Joystick(port, numAxisTypes, numButtonTypes) {
        // Empty
}

/**
 * Gets the value of an axis of the specified axis, ranging
 * from -1 to 1
 */
float XboxController::GetAxis(Axis axisName) {
        return GetRawAxis(axisName);
}

/**
 * Gets the value of the dpad direction. 
 * Y-dir:
 *      Top = 1
 *      Bottom = -1
 * X-dir:
 *  Top = 1
 *  Bottom = -1
 */
int XboxController::GetDpad(Dpad dpad)
{
        return (int) GetRawAxis(dpad);
}

/**
 * Gets if the specified button is pressed.
 */
bool XboxController::GetButton(Button buttonName)
{
        return GetRawButton(buttonName);
}

float XboxController::GetLeftXAxis() {
        return GetAxis(LeftX);
}

float XboxController::GetLeftYAxis() {
        return GetAxis(LeftY);
}

/**
 * Returns the trigger. The triggers are the two trigger-like things.
 * If the left one is fully pressed, this returns 1.
 * If the right one is fully pressed, this returns -1. If both
 * are pressed, the sum of both is the outputted value.
 */
float XboxController::GetTriggerAxis() {
        return GetAxis(Trigger);
}

float XboxController::GetRightXAxis() {
        return GetAxis(RightX);
}

float XboxController::GetRightYAxis() {
        return GetAxis(RightY);
}

int XboxController::GetXDirDpad() {
        return GetDpad(XDir);
}

int XboxController::GetYDirDpad() {
        return GetDpad(YDir);
}

bool XboxController::GetAButton() {
        return GetButton(A);
}

bool XboxController::GetBButton() {
        return GetButton(B);
}

bool XboxController::GetXButton() {
        return GetButton(X);
}

bool XboxController::GetYButton() {
        return GetButton(Y);
}

bool XboxController::GetLeftBumperButton() {
        return GetButton(LeftBumper);
}

bool XboxController::GetRightBumperButton() {
        return GetButton(RightBumper);
}

bool XboxController::GetBackButton() {
        return GetButton(Back);
}
bool XboxController::GetStartButton() {
        return GetButton(Start);
}

bool XboxController::GetLeftStickClickButton() {
        return GetButton(LeftStickClick);
}

bool XboxController::GetRightStickClickButton() {
        return GetButton(RightStickClick);
}

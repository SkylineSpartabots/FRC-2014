#include "WPILib.h"
#include "Ports.h"
#include "Subsystems/Claw/Claw.h"
#include "Subsystems/Shooter/Shooter.h"
#include "Subsystems/Vision/Vision.h"

#include "Vision/RGBImage.h"
#include "SmartDashboard/SmartDashboard.h"

class MainRobot : public SimpleRobot
{
private:
        RobotDrive *m_drive;
        Joystick *m_rightStick;
        Joystick *m_leftStick;
        
        Compressor *m_compressor;
        Solenoid *m_solenoid;
        
        //Victor *m_clawMotor;
        
        Talon *m_shooterLeft1;
        Talon *m_shooterLeft2;
        Talon *m_shooterRight1;
        Talon *m_shooterRight2;
        DigitalInput *m_shooterLimitSwitch;
        
        //Claw *m_claw;   
        Shooter *m_shooter;
        
public:
        MainRobot();
        void InitializeHardware();
        void InitializeSoftware();
        void Autonomous();
        void OperatorControl();
        void Test();
};

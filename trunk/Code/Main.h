#include "WPILib.h"
#include "Ports.h"
#include "Subsystems/Claw/Claw.h"
#include "Subsystems/Shooter/Shooter.h"
#include "Subsystems/Vision/Vision.h"

#include "Vision/RGBImage.h"

class MainRobot : public SimpleRobot
{
private:
        RobotDrive *m_drive;
        Joystick *m_rightStick;
        Joystick *m_leftStick;
        
        Compressor *m_compressor;
        Solenoid *m_solenoid;
        
        /*Victor *m_clawMotor;
        
        Victor *m_shooterLeft1;
        Victor *m_shooterLeft2;
        Victor *m_shooterRight1;
        Victor *m_shooterRight2;
        DigitalInput *m_shooterLimitSwitch;
        
        Claw *m_claw;   
        Shooter *m_shooter;*/
        
public:
        MainRobot();
        void InitializeHardware();
        void InitializeSoftware();
        void Autonomous();
        void OperatorControl();
        void Test();
};

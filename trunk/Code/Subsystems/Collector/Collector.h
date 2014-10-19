#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "WPILib.h"

class Collector {
public:
	Collector(Victor *motor, Solenoid *piston1, Solenoid *piston2, Solenoid *piston3,
			Solenoid *piston4, Compressor *compressor,  DigitalInput *pistonLimitSwitch, RobotDrive *drive);
	//bool GetLimitSwitch();
	void SpinInwards();
	void SpinOutwards();
	void SpinStop();
	void PistonPush();
	void PistonPull();
	void PistonNeutral();
	void PistonSlightPush();
	bool BringArmDown();
	bool IsExtended();
	bool GetLimitSwitch();
	void WatchdogWait(double time);
private:
	Victor *m_motor;
	Solenoid *m_piston1;
	Solenoid *m_piston2;
	Solenoid *m_piston3;
	Solenoid *m_piston4;
	Compressor *m_compressor;
	DigitalInput *m_pistonLimitSwitch;
	RobotDrive *m_drive;
};

#endif

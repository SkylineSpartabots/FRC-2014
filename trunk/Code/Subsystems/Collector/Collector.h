#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "WPILib.h"

/*
 SAFETY PIG HAS ARRIVED
  _._ _..._ .-',     _.._(`))
 '-. `     '  /-._.-'    ',/
    )         \            '.
   / _    _    |             \
  |  a    a    /              |
  \   .-.                     ;  
   '-('' ).-'       ,'       ;
      '-;           |      .'
         \           \    /
         | 7  .__  _.-\   \
         | |  |  ``/  /`  /
        /,_|  |   /,_/   /
           /,_/      '`-'

 This shouldn't be taken out in the final code,
 If this safety pig is removed, the program will blow up (true story)
*/

class Collector {
public:
	Collector(Victor *motor, Solenoid *piston1, Solenoid *piston2, Solenoid *piston3,
			Solenoid *piston4, Compressor *compressor,  DigitalInput *pistonLimitSwitch);
	//bool GetLimitSwitch();
	void SpinInwards();
	void SpinOutwards();
	void SpinStop();
	void PistonPush();
	void PistonPull();
	void PistonNeutral();
	void PistonSlightPush();
	void BringArmDown();
	bool isExtended();
	bool isFullyExtended(); 
private:
	Victor *m_motor;
	Solenoid *m_piston1;
	Solenoid *m_piston2;
	Solenoid *m_piston3;
	Solenoid *m_piston4;
	Compressor *m_compressor;
	DigitalInput *m_pistonLimitSwitch;
};

#endif

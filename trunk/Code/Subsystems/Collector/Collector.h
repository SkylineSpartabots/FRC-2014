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
	Collector(Victor *motor, Solenoid *piston, Compressor *compressor, DigitalInput *limitSwitch);
	bool GetLimitSwitch();
	void SpinInwards();
	void SpinOutwards();
	void SpinStop();
	void PistonUp();
	void PistonDown();
	void PistonUpAndDown();
private:
	Victor *m_motor;
	Solenoid *m_piston;
	Compressor *m_compressor;
	DigitalInput *m_limitSwitch;
};

#endif

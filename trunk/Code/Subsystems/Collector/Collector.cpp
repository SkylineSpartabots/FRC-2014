#include "Collector.h"

/* Roflcopter
 
   ROFL:ROFL:LOL:ROFL:ROFL
              ^
  L     /------------- 
  O ====          []  \
  L     \              \
         \______________]
            I      I
        ----------------/

 */

Collector::Collector(Victor *motor, Solenoid *piston1, Solenoid *piston2, Solenoid *piston3, Solenoid *piston4, Compressor *compressor, DigitalInput *limitSwitch) {
	m_motor = motor;
	m_piston1 = piston1;
	m_piston2 = piston2;
	m_piston3 = piston3;
	m_piston4 = piston4;
	m_compressor = compressor;
	m_limitSwitch = limitSwitch;
}

bool Collector::GetLimitSwitch() {
	return m_limitSwitch->Get();
}

void Collector::SpinInwards() {
	m_motor->Set(1.0);
}

void Collector::SpinOutwards() {
	m_motor->Set(-1.0);
}

void Collector::SpinStop() {
	m_motor->Set(0.0);
}

void Collector::PistonPush() {
	m_piston1->Set(false);
	m_piston2->Set(true);
	m_piston3->Set(true);
	m_piston4->Set(false);
}

void Collector::PistonPull() {
	m_piston1->Set(true);
	m_piston2->Set(false);
	m_piston3->Set(false);
	m_piston4->Set(true);
}

void Collector::PistonNeutral(){
	m_piston1->Set(true);
	m_piston2->Set(false);
	m_piston3->Set(true);
	m_piston4->Set(false);
}

void Collector::PistonSlightPush(){
	m_piston1->Set(false);
	m_piston2->Set(true);
	m_piston3->Set(false);
	m_piston4->Set(true);
}

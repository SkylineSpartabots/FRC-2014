#include "Claw.h"

Claw::Claw(Victor *motor) {
	m_motor = motor;
}

Claw::~Claw() {
	// empty
}

void Claw::Open() {
	m_motor->Set(1.0);
}

void Claw::Close() {
	m_motor->Set(-1.0);
}

void Claw::Stop() {
	m_motor->Set(0.0);
}

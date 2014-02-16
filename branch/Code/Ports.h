#ifndef PORTS_H
#define PORTS_H
/**
 * \file Ports.h
 * 
 * \brief Contains various enums representing physical ports and pins on
 * the robot and client computer. 
 * 
 * There is no corresponding .cpp file.
 * 
 * \defgroup ports Ports
 * \addtogroup ports
 * \{
 */

/**
 * \brief Contains all enums within the [Ports module](\ref ports) 
 */
namespace Ports {
	/**
	 * \brief Mappings for the digital sidecar (digital values)
	 */
	namespace DigitalSidecar {
		/**
		 * \brief Pulse-width modulation: used primarily for motors and servos.
		 */
		enum Pwm {
			Pwm1 = 1,
			Pwm2 = 2,
			Pwm3 = 3,
			Pwm4 = 4,
			Pwm5 = 5,
			Pwm6 = 6,
			Pwm7 = 7,
			Pwm8 = 8,
			Pwm9 = 9,
			Pwm10 = 10
		};
		
		/**
		 * \brief General purpose input/output: used mostly for sensors
		 */
		enum Gpio {
			Gpio1 = 1,
			Gpio2 = 2,
			Gpio3 = 3,
			Gpio4 = 4,
			Gpio5 = 5,
			Gpio6 = 6,
			Gpio7 = 7,
			Gpio8 = 8,
			Gpio9 = 9,
			Gpio10 = 10,
			Gpio11 = 11,
			Gpio12 = 12,
			Gpio13 = 13,
			Gpio14 = 14
		};
		
		/**
		 * \brief Used mostly for pneumatics (Spikes, etc)
		 */
		enum Relay {
			Relay1 = 1,
			Relay2 = 2,
			Relay3 = 3,
			Relay4 = 4,
			Relay5 = 5,
			Relay6 = 6,
			Relay7 = 7,
			Relay8 = 8
		};
	}
	
	/**
	 * \brief Mappings on the controller computer
	 */
	namespace Computer {
		/**
		 * \brief Used mostly for joysticks (or other forms of input)
		 */
		enum Usb {
			Usb1 = 1,
			Usb2 = 2,
			Usb3 = 3,
			Usb4 = 4	// Not sure if we actually have a 4th one
		};
	}
	
	/**
	 * \brief Additional mappings for the breakouts on the cRIO
	 */
	namespace Crio {
		/**
		 * \brief Used mostly for analog sensors
		 */
		enum AnalogBreakout {
			AnalogChannel1 = 1,
			AnalogChannel2 = 2,
			AnalogChannel3 = 3,
			AnalogChannel4 = 4,
			AnalogChannel5 = 5,
			AnalogChannel6 = 6,
			AnalogChannel7 = 7,
			AnalogChannel8 = 8
		};
		
		/**
		 * \brief Used mostly for pneumatics
		 */
		enum SolenoidBreakout {
			SolenoidBreakout1 = 1,
			SolenoidBreakout2 = 2,
			SolenoidBreakout3 = 3,
			SolenoidBreakout4 = 4,
			SolenoidBreakout5 = 5,
			SolenoidBreakout6 = 6,
			SolenoidBreakout7 = 7,
			SolenoidBreakout8 = 8
		};
		
		/**
		 * \brief Sections of the cRIO
		 * 
		 * This is a relatively new concept, which is meant
		 * to deal with how some FRC teams have 4-slot cRIOS
		 * while others have 8-slot cRIOS.
		 * 
		 * Slot: the physical space you can plug a module
		 * in.  Module number -- something FIRST invented.
		 * 
		 * For a 8-slot cRIO:
		 * -   Slot 1: Module 1 {Analog Module 9201)
		 * -   Slot 2: Module 1 {Digital Module 9403)
		 * -   Slot 3: Module 1 {Solenoid Module 9472)
		 * -   Slot 4: Module 2 {empty)
		 * -   Slot 5: Module 2 {Analog Module 9201)
		 * -   Slot 6: Module 2 {Digital Module 9403)
		 * -   Slot 7: Module 2 {Solenoid Module 9472)
		 * -   Slot 8: Module   {empty)
		 */
		enum Module {
			Module1 = 1,
			Module2 = 2
		};
	}
}

/**
 * \}
 */
#endif

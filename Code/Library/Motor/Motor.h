/*
 * Motor.h
 * Controls a single motor connected to a DRV8833 motor driver via PWM.
 * Created by Edward Shin, 11 July 2014.
 */
 
#ifndef Motor_h
#define Motor_h

#include "Energia.h"

enum motor_directions_t {FORWARD, REVERSE};

class Motor
{
  public:
	Motor(uint16_t forwardPin, uint16_t reversePin, bool hardBrake)
    	void setSpeed(uint8_t speed);
    	void setDirection(motor_directions_t direction);
	void setHardBrake(bool hardBrake);
	void setVelocity(int16_t velocity);
  private:
    	uint16_t _pin, _forwardPin, _reversePin;
	motor_directions_t _direction;
	uint8_t _speed;
	bool _hardBrake;
};

#endif

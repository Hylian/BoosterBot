/*
 * Motor.cpp
 * Controls a single motor connected to a DRV8833 motor driver via PWM.
 * Created by Edward Shin, 11 July 2014.
 */

#include "Energia.h"
#include "Motor.h"


Motor::Motor(uint16_t forwardPin, uint16_t reversePin, bool hardBrake)
{
	_forwardPin = forwardPin;
	_reversePin = reversePin;
	_hardBrake = hardBrake;
	_pin = _forwardPin;
	_speed = 0;
	_direction = FORWARD;

	pinMode(_forwardPin, OUTPUT);
	pinMode(_reversePin, OUTPUT);
}

void setSpeed(uint8_t speed)
{
	_speed = speed;
	analogWrite(_pin, _speed);

	if(_speed = 0)
	{
		if(_pin == _forwardPin)
			analogWrite(_reversePin, _hardBrake ? 255 : 0);
		else
			analogWrite(_forwardPin, _hardBrake ? 255 : 0);
	}

	return;
}

void setDirection(motor_directions_t direction)
{
	_direction = direction;
	_pin = (_direction == FORWARD) ? _forwardPin : _reversePin;
	analogWrite(_pin, _speed);

	if(_pin == _forwardPin)
		analogWrite(_reversePin, 0);
	else
		analogWrite(_forwardPin, 0);

	return;
}

void setHardBrake(bool hardBrake)
{
	if(_hardBrake != hardBrake)
	{
		if(_pin == _forwardPin)
			analogWrite(_reversePin, hardBrake ? 255 : 0);
		else
			analogWrite(_forwardPin, hardBrake ? 255 : 0);
	}

	_hardBrake = hardBrake;

	return;
}

void setVelocity(int16_t velocity)
{
	if(!(-255 <= velocity && velocity <= 255))
		return;

	_direction = (velocity >= 0) ? FORWARD : REVERSE;
	setDirection(_direction);

	_speed = abs(velocity);
	setSpeed(_speed);

	return;
}

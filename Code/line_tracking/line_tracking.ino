#include "../libraries/Motor/Motor.h"
#include "PID_v1.h"

double Setpoint, Input, Output;

bool sensorBool[5];
uint32_t threshold;

const uint8_t sensorPin[5] = {23, 24, 25, 26, 27};

const uint8_t speed = 100;

PID linePID(&Input, &Output, &Setpoint, 40, 5, 1, DIRECT);

Motor leftMotor(37, 38, true);
Motor rightMotor(40, 39, true);

void calibrate()
{
	threshold = 0;
	for(char i = 0; i < 5; i++)
		threshold += analogRead(sensorPin[i]);
	threshold /= 5;
	threshold += 100;
        Serial.print("Threshold set: ");
        Serial.println(threshold);
}

void readSensors()
{
	for(char i = 0; i < 5; i++)
		sensorBool[i] = (analogRead(sensorPin[i]) < threshold);
}

void calculateError()
{
	if(sensorBool[1] && !sensorBool[2] && !sensorBool[3])
		Input = 2.0;
	else if(sensorBool[1] && sensorBool[2] && !sensorBool[3])
		Input = 1.0;
	else if(	(sensorBool[1] && sensorBool[2] && sensorBool[3]) ||
				(!sensorBool[1] && sensorBool[2] && !sensorBool[3]) 
	  		)
	  	Input = 0.0;
	else if(!sensorBool[1] && sensorBool[2] && sensorBool[3])
		Input = -1.0;
	else if(!sensorBool[1] && !sensorBool[2] && sensorBool[3])
		Input = -2.0;
	else
		Input = 0.0;

        Serial.print("Error: ");
        Serial.println(Input);
}

void setup()
{
        Serial.begin(9600);
	Input = 0;
	Setpoint = 0;
	linePID.SetMode(AUTOMATIC);
	linePID.SetOutputLimits(-100, 100);
        leftMotor.setVelocity(speed);
	rightMotor.setVelocity(speed);
        calibrate();
}

void loop()
{
	readSensors();
	calculateError();
	linePID.Compute();
	leftMotor.setSpeed(speed + Output);
	rightMotor.setSpeed(speed - Output);
        Serial.print("Output: ");
        Serial.println(Output);
        Serial.print("Reading: ");
        Serial.print(sensorBool[0]);
        Serial.print(" ");
        Serial.print(sensorBool[1]);
        Serial.print(" ");
        Serial.print(sensorBool[2]);
        Serial.print(" ");
        Serial.print(sensorBool[3]);
        Serial.print(" ");
        Serial.println(sensorBool[4]);
	delay(50);
}

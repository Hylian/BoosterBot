#include "Motor.h"
#include "PID_v1.h"

double Setpoint, prevInput, Input, Output;

PID motorPID(&Input, &Output, &Setpoint, 0.1, 0.05, 0.007, DIRECT);

Motor leftMotor(37, 38, true);
Motor rightMotor(40, 39, true);


void setup()
{
        pinMode(P7_0, INPUT);
        Serial.begin(9600);
	Input = 0;
        prevInput = 0;
	Setpoint = 1000;
 	motorPID.SetMode(AUTOMATIC);
	motorPID.SetOutputLimits(-50, 50);
}

void loop()
{
	Input = analogRead(P7_0);
        Serial.println(Input);
	motorPID.Compute();
        if(abs(Output) < 25)
          Output = 0;
	leftMotor.setVelocity(Output);
	rightMotor.setVelocity(Output);
}


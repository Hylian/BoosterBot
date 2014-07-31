#include "Motor.h"
#include "PID_v1.h"

double Setpoint, Input, Output;

bool sensorBool[5];
bool prevLeft, prevRight;
uint32_t threshold;

const uint8_t sensorPin[5] = {23, 24, 25, 26, 27};

const uint8_t baseSpeed = 100;
const uint8_t turnSpeed = 30;

PID linePID(&Input, &Output, &Setpoint, 40, 5, 1, DIRECT);

Motor leftMotor(37, 38, true);
Motor rightMotor(40, 39, true);

enum mazeState {STOP, FOLLOW, TURN_LEFT, TURN_RIGHT, DONE};

void calibrate()
{
	threshold = 0;
	for(char i = 0; i < 5; i++)
		threshold += analogRead(sensorPin[i]);
	threshold /= 5;
	threshold += 10;
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
        mazeState state = STOP;
	Input = 0;
	Setpoint = 0;
	linePID.SetMode(MANUAL);
	linePID.SetOutputLimits(-100, 100);
        leftMotor.setVelocity(speed);
	rightMotor.setVelocity(speed);  
        calibrate();
}

void loop()
{
	switch(state)
	{
		case STOP:
			linePID.SetMode(MANUAL);
			leftMotor.setSpeed(0);
			rightMotor.setSpeed(0);
			break;

		case FOLLOW:
			linePID.SetMode(AUTOMATIC);
			while(!sensorBool[0] || !sensorBool[4] || (!sensorBool[1]&&!sensorBool[2]&&!sensorBool[3]))
			{
				readSensors();
				calculateError();
				linePID.Compute();
				leftMotor.setSpeed(baseSpeed + Output);
				rightMotor.setSpeed(baseSpeed - Output);
			}
			prevLeft = sensorBool[0];
			prevRight = sensorBool[4];
			delay(400);
			readSensors();
			if(sensorBool[0]&&sensorBool[1]&&sensorBool[2]&&sensorBool[3]&&sensorBool[4])
				state = DONE;
			else if(prevLeft)
				state = TURN_LEFT;
			else
				state = TURN_RIGHT;
			break;

		case TURN_LEFT:
			linePID.SetMode(MANUAL);
			leftMotor.setSpeed(baseSpeed - turnSpeed);
			rightMotor.setSpeed(baseSpeed + turnSpeed);
			do
			{
				readSensors();
			}
			while(sensorBool[2]);
			do
			{
				readSensors();
			}
			while(!sensorBool[2]);
			state = FOLLOW;
			break;

		case TURN_RIGHT:
			linePID.SetMode(MANUAL);
			leftMotor.setSpeed(baseSpeed + turnSpeed);
			rightMotor.setSpeed(baseSpeed - turnSpeed);
			do
			{
				readSensors();
			}
			while(sensorBool[2]);
			do
			{
				readSensors();
			}
			while(!sensorBool[2]);
			state = FOLLOW;
			break;

		case DONE:
			linePID.SetMode(MANUAL);
			leftMotor.setSpeed(0);
			leftMotor.setSpeed(0);
			for(;;) {}
	}


	delay(50);
}

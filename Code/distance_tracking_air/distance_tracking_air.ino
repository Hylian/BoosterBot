#include <SPI.h>
#include <AIR430BoostFCC.h>
#include "Motor.h"
#include "PID_v1.h"

double Setpoint, prevInput, Input, Output;

PID motorPID(&Input, &Output, &Setpoint, 0.104, 0.03, 0.01, DIRECT);

Motor leftMotor(37, 38, true);
Motor rightMotor(40, 39, true);

struct dataPacket
{
  uint16_t distance;
};

struct dataPacket data;

void setup()
{
        pinMode(P7_0, INPUT);
        Radio.begin(0x01, CHANNEL_1, POWER_MAX);
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
        data.distance = Input;
        Radio.transmit(ADDRESS_BROADCAST, (unsigned char*)&data, sizeof(data));
        while(Radio.busy());
}


#include "Motor.h"
#include "PID_v1.h"
#include "RunningAverage.h"

double Setpoint, prevInput, Input, Output;

bool sensorBool[5];
bool lostInput;
uint32_t threshold[5];
uint32_t maxValues[5], minValues[5], avgValues[5];
uint32_t tempValue;

const uint8_t sensorPin[5] = {23, 24, 25, 26, 27};

const uint8_t speed = 45;

PID linePID(&Input, &Output, &Setpoint, 5, 0, 0, DIRECT);

RunningAverage RA0(30);
RunningAverage RA1(30);
RunningAverage RA2(30);
RunningAverage RA3(30);
RunningAverage RA4(30);

Motor leftMotor(37, 38, true);
Motor rightMotor(40, 39, true);

void calibrate()
{
        while(digitalRead(P1_1)){}
        while(!digitalRead(P1_1)){}
        
        for(char i = 0; i < 5; i++)
        {
          minValues[i] = 0;
          maxValues[i] = 0;
        }        
        
        delay(500);
        
        Serial.println("Starting calibration");
        
        for(char i = 0; i < 5; i++)
        {
          minValues[i] = analogRead(sensorPin[i]);
          maxValues[i] = minValues[i];
        }
        
        while(digitalRead(P1_1))
        {
          for(char i = 0; i < 5; i++)
          {
            tempValue = analogRead(sensorPin[i]);
            if(tempValue < minValues[i])
              minValues[i] = tempValue;
            if(tempValue > maxValues[i])
              maxValues[i] = tempValue;
            Serial.print(tempValue);
            Serial.print(" ");
          }
          Serial.println();
        }
        
        for(char i = 0; i < 5; i++)
        {
          avgValues[i] = (2*minValues[i] + 3*maxValues[i])/5;
        }
        
        Serial.println("Calibration complete!");
            
        for(char i = 0; i < 5; i++)
          {
          Serial.println(avgValues[i]);        
          }
        /**
        for(char i = 0; i < 5; i++)
		threshold[i] = 0;
        for(char j = 0; j < 5; j++)
        {
            for(char i = 0; i < 5; i++)
		threshold[i] += analogRead(sensorPin[i]);
        }
        for(char i = 0; i < 5; i++)
        {
                threshold[i] /= 5;
                threshold[i] -= 400;
        }
        
        threshold[1] -= 500;
        threshold[2] -= 500;
	
        Serial.print("Threshold set: ");
        Serial.print(threshold[0]);
        Serial.print(" ");
        Serial.print(threshold[1]);
        Serial.print(" ");
        Serial.print(threshold[2]);
        Serial.print(" ");
        Serial.print(threshold[3]);
        Serial.print(" ");
        Serial.println(threshold[4]);
        
        */

}

void calibrate2()
{
  while(digitalRead(P1_1)){}
  while(!digitalRead(P1_1)){}
  
  Serial.println("Starting white calibration...");
  
  RA0.clear();
  RA1.clear();
  RA2.clear();
  RA3.clear();
  RA4.clear();
  
  while(digitalRead(P1_1))
  {
    RA0.addValue(analogRead(sensorPin[0]));
    RA1.addValue(analogRead(sensorPin[1]));
    RA2.addValue(analogRead(sensorPin[2]));
    RA3.addValue(analogRead(sensorPin[3]));
    RA4.addValue(analogRead(sensorPin[4]));
    Serial.println("averaging whites...");
  }

  minValues[0] = RA0.getAverage();
  minValues[1] = RA1.getAverage();
  minValues[2] = RA2.getAverage();
  minValues[3] = RA3.getAverage();
  minValues[4] = RA4.getAverage();
  
  while(!digitalRead(P1_1));
  
  delay(100);

  Serial.println("Starting black calibration...");

  RA0.clear();
  RA1.clear();
  RA2.clear();
  RA3.clear();
  RA4.clear();
  
  while(digitalRead(P1_1))
  {
    RA0.addValue(analogRead(sensorPin[0]));
    RA1.addValue(analogRead(sensorPin[1]));
    RA2.addValue(analogRead(sensorPin[2]));
    RA3.addValue(analogRead(sensorPin[3]));
    RA4.addValue(analogRead(sensorPin[4]));
        Serial.println("averaging blacks...");
  }

  maxValues[0] = RA0.getAverage();
  maxValues[1] = RA1.getAverage();
  maxValues[2] = RA2.getAverage();
  maxValues[3] = RA3.getAverage();
  maxValues[4] = RA4.getAverage();
  
  avgValues[0] = (2 * minValues[0] + 3 * maxValues[0])/5;
  avgValues[1] = (2 * minValues[1] + 3 * maxValues[1])/5;
  avgValues[2] = (2 * minValues[2] + 3 * maxValues[2])/5;
  avgValues[3] = (2 * minValues[3] + 3 * maxValues[3])/5;
  avgValues[4] = (2 * minValues[4] + 3 * maxValues[4])/5;
  
          Serial.println("Calibration complete!");
            
        for(char i = 0; i < 5; i++)
          {
          Serial.println(avgValues[i]);        
          }
}  
  

void readSensors()
{
  for(char i = 0; i < 5; i++)
  {
    sensorBool[i] = analogRead(sensorPin[i]) >   avgValues[i];
  }
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
	else if(!lostInput)
        {
          Input = -prevInput;
          lostInput = true;
          return;
        }
        else
          return;
        lostInput = false;
                 
        prevInput = Input;
        //Serial.print("Error: ");
        //Serial.println(Input);
}

void setup()
{
        pinMode(P1_1, INPUT_PULLUP);
        Serial.begin(9600);
	Input = 0;
        prevInput = 0;
	Setpoint = 0;
        leftMotor.setVelocity(0);
	rightMotor.setVelocity(0);
        calibrate();
	linePID.SetMode(AUTOMATIC);
	linePID.SetOutputLimits(-100, 100);
        leftMotor.setVelocity(speed);
	rightMotor.setVelocity(speed);

}

void loop()
{
	readSensors();
	calculateError();
	linePID.Compute();
	//leftMotor.setVelocity(speed - Output);
	//rightMotor.setVelocity(speed + Output);
	leftMotor.setVelocity(0);
	rightMotor.setVelocity(0);
        //Serial.print("Output: ");
        //Serial.println(Output);
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
        //Serial.print("Raw Input: ");
        //Serial.print(analogRead(sensorPin[1]));
        //Serial.print(" ");
        //Serial.print(analogRead(sensorPin[2]));
        //Serial.print(" ");
        //Serial.println(analogRead(sensorPin[3]));
}

#include <SPI.h>
#include <AIR430BoostFCC.h>

struct dataPacket
{
  uint8_t Kp;
  uint8_t Ki;
  uint8_t Kd;
};

struct dataPacket data;
String command;
char nextByte;
char stringBuf[30];
int dataCounter;

void setup()
{
  Radio.begin(0x01, CHANNEL_1, POWER_MAX);
  Serial.begin(9600);
}

void loop()
{
  if(Serial.available() > 0)
  {
    nextByte = Serial.read();
    command += nextByte;
    if(nextByte == ' ' || nextByte == '\n')
    {
      if(dataCounter == 0)
        data.Kp = command.toInt();
      else if(dataCounter == 1)
        data.Ki = command.toInt();
      else if(dataCounter == 2)
        data.Kd = command.toInt();
      dataCounter++;
      command = "";
      if(nextByte == '\n')
      {
        dataCounter = 0;
        Radio.transmit(ADDRESS_BROADCAST, (unsigned char*)&data, sizeof(data));
        while(Radio.busy());
      }
    }
  }
}

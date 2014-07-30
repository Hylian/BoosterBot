#include <SPI.h>
#include <AIR430BoostFCC.h>

struct dataPacket
{
  uint16_t distance;
};

struct dataPacket data;

void setup()
{
  Radio.begin(0x01, CHANNEL_1, POWER_MAX);
  Serial.begin(9600);
  Serial.println("test");
}

void loop()
{
  if(Radio.receiverOn((unsigned char*)&data, sizeof(data), 1000) > 0)
  {
    Serial.println(data.distance);
  }
}

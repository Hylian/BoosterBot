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
}

void loop()
{
  data.distance = analogRead(P7_0);
  Radio.transmit(ADDRESS_BROADCAST, (unsigned char*)&data, sizeof(data));
  while(Radio.busy());
  delay(100);
}

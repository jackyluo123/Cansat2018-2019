// Depending how many address you want to reset, this process can take about a few minutes
#include <Wire.h>

#define EEPROM_I2C_ADDRESS 0x50
#define TeleArrayLength 16

int packetCount = 0;

//this union is for converting floats into bytes for storing into the EEPROM
union{
  float fValue;
  byte bValue[4];
} floatToByte;

// This union is for testing and converting the bytes back to floats
union {
  float fValue;
  byte bValue[4];
} byteToFloat;

void setup() 
{
  Wire.begin();
  Serial.begin(19200);
}

void loop()
{
  dataReset();
  packetCount++;
  Serial.println (packetCount);
}

void dataReset(){
  writeFloat(0,0);
  writeFloat(4,0);
  writeFloat(8,0);
  writeFloat(12,0);
  int startAddress = 16 + (TeleArrayLength * packetCount * 4);
  for (int i = 0; i < TeleArrayLength; i++)
  {
    writeFloat(startAddress + (i*4), 0);
    Serial.print(readFloat(startAddress + (i*4)));
    if (i < TeleArrayLength - 1)
    {
    Serial.print (", ");
    }
  }
  Serial.println ("");
}

void writeFloat(int address, float val)
{
  floatToByte.fValue = val;
  writeAddress(address, floatToByte.bValue[0]); 
  address++;
  writeAddress(address, floatToByte.bValue[1]); 
  address++;
  writeAddress(address, floatToByte.bValue[2]); 
  address++;
  writeAddress(address, floatToByte.bValue[3]); 
}

// Writes a byte to an address in an external EEPROM
void writeAddress(int address, byte val)
{
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  
   
  Wire.write(val);
  Wire.endTransmission();

  // Delay must be at least 4 seconds, but 5 seconds is used just in case
  delay(5);
}

float readFloat (int address)
{
  byteToFloat.bValue[0] = readAddress(address);
  address++;
  byteToFloat.bValue[1] = readAddress(address);
  address++;
  byteToFloat.bValue[2] = readAddress(address);
  address++;
  byteToFloat.bValue[3] = readAddress(address);
  float answer = byteToFloat.fValue;
  return answer;
}

byte readAddress(int address)
{
  byte rData = 0xFF;
  
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  Wire.endTransmission();  


  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);  

  rData =  Wire.read();

  return rData;
}

/* 
 * This was done using Windows
 *  
 * Arduino cannot create txt files or excel files nor access them without third-party
 * software.
 * 
 * *****INSTRUCTIONS*****
 * 
 * Before uploading this code, check if:
 * You are using a separate arduino than the arduino used for the flight
 * Does the previous code on this arduino involve the external EEPROM? If so or you don't know,
 * Upload this code without the external EEPROM wired to prevent overwriting any addresses
 * 
 * The telemetry array will be shown in the serial monitor
 * Eventually, you'll see 0's for everything or numbers that you don't think should be there
 * **** Copy the bulk of the text that is part of the telemetry array, not including the first few
 * values at the top which are not part of the telemetry array.
 * **** Paste this into a txt file using Notepad or something similar
 * **** Open an excel file and in the tabs at the top, select "Data", then "Get Data" on the left.
 * A list will drop and select "From File" then "From Text/CSV". 
 * **** Find your txt file to import it.
 * **** Excel will show you what your data will look like when imported. If the data appears to be 
 * correct, then select "Load" at the bottom. Otherwise, check the options at the top.
 * 
 * **** Excel will create a table for you but if you don't want to work with this table, you can copy
 * and paste the data into another sheet on the bottom-left
 */

#include <Wire.h>

#define EEPROM_I2C_ADDRESS 0x50
#define TeleArrayLength 16

int packetCount = 0;

// This union is for testing and converting the bytes back to floats
union {
  float fValue;
  byte bValue[4];
} byteToFloat;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(19200);
  Serial.print("0: "); // This one is for the packet count
  Serial.println (readFloat(0));
  Serial.print ("4: "); // This one is for the software state
  Serial.println (readFloat(4));
  Serial.print ("8: "); // This one is for the BMP sea level pressure
  Serial.println (readFloat(8));
}

void loop() {
  // put your main code here, to run repeatedly:
  dataPrint();
  packetCount++;
}

void dataPrint(){
  int startAddress = 16 + (TeleArrayLength * packetCount * 4);
  for (int i = 0; i < TeleArrayLength; i++)
  {
    Serial.print(readFloat(startAddress + (i*4)));
    if (i < TeleArrayLength - 1)
    {
    Serial.print (", ");
    }
  }
  Serial.println ("");
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

void setupFunctions(){
  Wire.begin();
  setupCamera();
  setupBuzzer();
  setupBMP();
  setupGPS();
  setupVoltage();
  setupRTC();
  setupMPU();
}

void dataCollection(){
  getTime();
  getTilt();
  TeleArray[TelePacket]=packetCount;
  getVolt();
  getTemp();
  getPressure();
  getAlt();
  getGPS();
  TeleArray[TeleState] = sensorState; 
}

void transmitData()
{
  Serial.print(TeleArray[TeleID]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleMissionTime]);
  Serial.print(", ");
  Serial.print(TeleArray[TelePacket]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleAlt]);
  Serial.print(", ");
  Serial.print(TeleArray[TelePressure]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleTemp]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleVolt]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleGTime]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleGLat]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleGLong]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleGAlt]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleGSats]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleTiltX]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleTiltY]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleTiltZ]);
  Serial.print(", ");
  Serial.print(TeleArray[TeleState]);
  Serial.print("\n");
}

void checkFlown(float alt){
  if(alt>5){
    hasFlown = true;
    eepromCheck = 1;
  }
}

void checkDescend (float alt)
{
  if (hasFlown == true)
  {
    if (alt < lastAlt)
    {
      if (alt < lastAlt2) // Double check in case a bad altitude was measured
      {
        isDescending = true;
      }
    }
  }
  lastAlt2 = lastAlt;
  lastAlt = alt;
}

void stateCheck(){
  checkFlown(TeleArray[TeleAlt]); //Did it fly
  checkDescend(TeleArray[TeleAlt]); // Is it descending?
  sensorState = PreFlight;

  if(hasFlown == true)
  {
    if(TeleArray[TeleAlt] < 4)
    {
      sensorState = Landed;
    }
    else
    {
      sensorState = Flying;
    }
  }
}

// First burn is 350
// Second burn is 300

void checkBurn(){
  receiveRadioData();
  if(sensorState == 2 && TeleArray[TeleAlt] < 10 && isDescending == true)
  {
    digitalWrite(deployPinA, HIGH);
    //digitalWrite(deployPinB, HIGH);
    //videoON();
  }
  if(sensorState == 2 && TeleArray[TeleAlt]< 8 && isDescending == true)
  {
    //digitalWrite(deployPinA, LOW);
    digitalWrite(deployPinB, HIGH);
  }
  if(sensorState == 3)
  {
    videoOFF();
    startBuzzer();
    eepromCheck = 0;
  }
}

void receiveRadioData() {
  //Checks to see if commands have come in via radio
  if (Serial.available()) {
    String RadioReceive = Serial.readString();
    if (RadioReceive == "A")
    {
      digitalWrite(deployPinA, HIGH);
    }
    if (RadioReceive == "B"){
      digitalWrite(deployPinB, HIGH);
    }
  }
}

void timeDelay(){
  //Get the tilt every tentth of a second
  while (millis() - teleTime < 100){}
  //getTilt();
  getGPS2();
  while (millis() - teleTime < 200){}
  //getTilt();
  getGPS2();
  while (millis() - teleTime < 300){}
  //getTilt();
  getGPS2();
  while (millis() - teleTime < 400){}
  //getTilt();
  getGPS2();
  while (millis() - teleTime < 500){}
  //getTilt();
  getGPS2();
  while (millis() - teleTime < 600){}
  //getTilt();
  getGPS2();
  while (millis() - teleTime < 700){}
  //getTilt();
  getGPS2();
  while (millis() - teleTime < 800){}
  //getTilt();
  getGPS2();
  while (millis() - teleTime < 900){}
  //getTilt();
  getGPS2();
  while (millis() - teleTime < 1000){}
}


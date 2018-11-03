int check = 0;
float timer = 0;
void setupCamera()
{
  timer = millis();
  pinMode(cameraTrigger, OUTPUT);
  digitalWrite(cameraTrigger, HIGH);
  while (millis() - timer < 3000){}
  // Leave the video on the whole time
  timer = millis();
  digitalWrite(cameraTrigger, LOW);
  while (millis() - timer < 3000){}
  digitalWrite(cameraTrigger, HIGH);
}

//Turns camera on
void cameraON()
{
  digitalWrite(cameraTrigger, HIGH);
}

//Starts recording
void videoON()
{
  if (check == 0){
  digitalWrite(cameraTrigger, LOW);
  check = 1;
  }
}

//Stops recording
void videoOFF()
{
  timer = millis();
  digitalWrite(cameraTrigger, LOW);
  while (millis() - timer < 3000){}
  digitalWrite(cameraTrigger, HIGH);
}


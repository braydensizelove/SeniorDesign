#include "DoorControls.h"
#include "DistanceSensor.h"
#include "ESP32_ARDUCAM.h"
#include "webApp.h"

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("SGDO Begin");
  
  setupWeb(); //get fixed code each startup for door controls
  setupDistanceSensor();
  setupDoorControl();
  setupCam();
}

void loop() {
  // put your main code here, to run repeatedly:
  pollDistanceSensor(); //return data to send to webapp on request
  pollCam(); //blocking, cannot poll cam.
}

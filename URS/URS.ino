
/**************************************************/
/* File name:        URS.ino                      */
/* File description: Main functon implementation  */
/*                   for the URS project in the   */
/*                   ESP32-CAM micro controller.  */
/* Author name:      Richard Netto                */
/* Creation date:    20/11/2020                   */
/* Revision date:    20/11/2020                   */
/**************************************************/

#include "SonarSensor.h"
// Pin Variables
int iEchoPinSensor1 = 13;
int iTriggerPinSensor1 = 12;
int iEchoPinSensor2 = 14;
int iTriggerPinSensor2 = 15;
// Sonar Variables
SonarSensor sensor1(iEchoPinSensor1,iTriggerPinSensor1);
SonarSensor sensor2(iEchoPinSensor2,iTriggerPinSensor2);

void setup() {
  Serial.begin(115200); // Start the serial communication
}

void loop() {
  Serial.print("Distance 1: ");
  Serial.print(sensor1.getDistance());
  Serial.print(" cm           Distance 2: ");
  Serial.print(sensor2.getDistance());
  Serial.println(" cm.");
  delay(500);
}

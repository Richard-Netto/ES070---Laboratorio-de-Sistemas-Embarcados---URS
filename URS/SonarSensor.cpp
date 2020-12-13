/**************************************************/
/* File name:        SonarSensor.c                */
/* File description: File for the implementation  */
/*                   of SonarSensor Class, for    */
/*                   the HC-SC04 distance sensor. */
/* Author name:      Richard Netto                */
/* Creation date:    20/11/2020                   */
/* Revision date:    20/11/2020                   */
/**************************************************/
#include "Arduino.h"
#include "SonarSensor.h"

/****************************************************/
/* Creator name:       SonarSensor                  */
/* Method description: Class Object creator         */
/*                                                  */
/* Input params:       iEchoPin - Pin associated to */
/*                     the eccho signal of the      */
/*                     sensor. (int)                */
/*                     iTriggerPin - Pin associated */
/*                     to the trigger signal of the */
/*                     sensor. (int)                */
/****************************************************/
SonarSensor::SonarSensor(int iEchoPinNumber, int iTriggerPinNumber)
{
  // Salve the Pin Numbers
  iEchoPin = iEchoPinNumber;
  iTriggerPin = iTriggerPinNumber;
  // Set the Pin Modes for the sensor
  pinMode(iEchoPin, INPUT);
  pinMode(iTriggerPin, OUTPUT);
  // Initial variables of a linear sensor
  lA = 29;
  lB = 0.00;
}

/****************************************************/
/* Method name:        getDistance                  */
/* Method description: Method that returns the      */
/*                     distance measured by the     */
/*                     sensor in cm.                */
/*                                                  */
/* Input params:     iEchoPin - Pin associated to   */
/*                   the eccho signal of the        */
/*                   sensor. (int)                  */
/*                   iTriggerPin - Pin associated   */
/*                   to the trigger signal of the   */
/*                   sensor. (int)                  */
/* Output params:    Distance in cm.                */
/****************************************************/
int SonarSensor::getDistance()
{
  // Short LOW pulse beforehand to ensure a clean HIGH pulse
  digitalWrite(iTriggerPin, LOW);
  delayMicroseconds(2);
  // Send HIGH pulse PING
  digitalWrite(iTriggerPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(iTriggerPin, LOW);

  // Read the PING echo from an obstacle and gives back the time it took
  lDuration = pulseIn(iEchoPin, HIGH);
  // Calculate the distance
  iDistanceCm = lDuration / lA / 2 + lB;
  return iDistanceCm;
}

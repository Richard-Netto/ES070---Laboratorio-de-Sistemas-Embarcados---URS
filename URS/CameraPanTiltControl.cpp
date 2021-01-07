/*****************************************************/
/* File name:        CameraPanTiltControl.h          */
/* File description: File for the implementation     */
/*                   of CameraPanTiltControl Class,  */
/*                   controlling the Servo motors    */
/*                   SG90 that moves the camera.     */
/* Author name:      Richard Vaz da Silva Netto      */
/* Creation date:    20/11/2020                      */
/* Revision date:    16/12/2020                      */
/*****************************************************/

#include "Arduino.h"
#include "CameraPanTiltControl.h"

/******************************************************/
/* Method name:        CameraPanTiltControl           */
/* Method description: Creates CameraPanTiltControl   */
/*                     object to control the Pan Tilt */
/*                     Servo motors of the camera     */
/*                     support.                       */
/*                                                    */
/* Input params:       int iServoPanPin - Pin number  */
/*                     for the Pan Servo Motor.       */
/*                     int iServoTiltPin - Pin number */
/*                     for the Tilt Servo Motor.      */
/* Output params:                                     */
/******************************************************/
CameraPanTiltControl::CameraPanTiltControl(int iServoTiltPin, int iServoPanPin)
{
  // Tilt Servo Setting
  ledcSetup(TILT_SERVO_CHANNEL, SERVO_FREQUENCY_HZ, PWM_RESOLUTION);
  ledcAttachPin(iServoTiltPin, TILT_SERVO_CHANNEL);

  // Pan Servo Setting
  ledcSetup(PAN_SERVO_CHANNEL, SERVO_FREQUENCY_HZ, PWM_RESOLUTION);
  ledcAttachPin(iServoPanPin, PAN_SERVO_CHANNEL);
}

/*****************************************************/
/* Method name:        updatePosition                */
/* Method description: Method updates the PWM signal */
/*                     of each servo motor to move   */
/*                     the camera.                   */
/*                                                   */
/* Input params:     iAxisY - Value of the axis      */
/*                   position in Y. Range 0 - 1023   */
/*                   iAxisX - Value of the axis      */
/*                   position in X. Range 0 - 1023   */
/* Output params:                                    */
/*****************************************************/
void CameraPanTiltControl::updatePosition(int iAxisX, int iAxisY)
{
  // Clamp Pan Values
  int iClampedAxisX = clampValues(iAxisX + PAN_CENTERING_VALUE, MIN_PAN_SERVO_10BIT_VALUE, MAX_PAN_SERVO_10BIT_VALUE);
  // Update Pan dutycycle
  int iPanDutyCycle = map(iClampedAxisX, 1023, 0, 0, 8888); // 10 bit to 16 bit map
  ledcWrite(PAN_SERVO_CHANNEL, iPanDutyCycle);

  // Clamp Tilt Values
  int iClampedAxisY = clampValues(iAxisY + TILT_CENTERING_VALUE, MIN_TILT_SERVO_10BIT_VALUE, MAX_TILT_SERVO_10BIT_VALUE);
  // Update Tilt dutycycle
  int iTiltDutyCycle = map(iClampedAxisY, 1023, 0, 0, 8888); // 10 bit to 16 bit map
  ledcWrite(TILT_SERVO_CHANNEL, iTiltDutyCycle);
}

/******************************************************/
/* Method name:        clampValues                    */
/* Method description: Method that clamps a value     */
/*                     between a interval.            */
/*                                                    */
/* Input params:       int iValue - Value to clamp    */
/*                     between the iMinValue and      */
/*                     iMaxValue.                     */
/*                     int iMinValue - Max Value to   */
/*                     clamp.                         */
/*                     int iMaxValue - Min Value to   */
/*                     clamp.                         */
/* Output params:      int iFinalValue - Clamped      */
/*                     value obtained.                */
/******************************************************/
int CameraPanTiltControl::clampValues(int iValue, int iMinValue, int iMaxValue) {
  // Pan Clamp
  int iFinalValue = iValue;
  if (iMaxValue < iValue) {
    iFinalValue = iMaxValue;
  } else if (iMinValue > iValue) {
    iFinalValue = iMinValue;
  }
  return iFinalValue;
}

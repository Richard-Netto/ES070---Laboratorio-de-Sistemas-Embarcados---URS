/***************************************************/
/* File name:        MovementControl.cpp           */
/* File description: File for the implementation   */
/*                   of SonarSensor Class, for the */
/*                   SM-S4303R servo motor.        */
/* Author name:      Paola Harduim                 */
/* Creation date:    20/11/2020                    */
/* Revision date:    17/12/2020                    */
/***************************************************/

#include "Arduino.h"
#include "MovementControl.h"

/*****************************************************/
/* Creator name:       MovementControl               */
/* Method description: Class Object creator          */
/*                                                   */
/* Input params:       leftServoPin - Pin associated */
/*                     to the PWM signal of the      */
/*                     left servo. (int)             */
/*                     rightServoPin - Pin           */
/*                     associated to the PWM signal  */
/*                     of the right servo. (int)     */
/* Output params:                                    */
/*****************************************************/
MovementControl::MovementControl(int iLeftServoPin, int iRightServoPin)
{
  // Left Servo Setting
  ledcSetup(LEFT_SERVO_CHANNEL, SERVO_FREQUENCY_HZ, PWM_RESOLUTION);
  ledcAttachPin(iLeftServoPin, LEFT_SERVO_CHANNEL);

  // Right Servo Setting
  ledcSetup(RIGHT_SERVO_CHANNEL, SERVO_FREQUENCY_HZ, PWM_RESOLUTION);
  ledcAttachPin(iRightServoPin, RIGHT_SERVO_CHANNEL);
}

/*****************************************************/
/* Method name:        updateMovement                */
/* Method description: Method updates the PWM signal */
/*                     of each servo by the          */
/*                     correlation of the analog     */
/*                     input axis (1024 levels)      */
/*                     changing velocity.            */
/*                                                   */
/* Input params:       iAxisY - Value of the axis    */
/*                     position in Y, ranging from 0 */
/*                     to 1023. (int)                */
/*                     iAxisX - Value of the axis    */
/*                     position in X, ranging from 0 */
/*                     to 1023. (int)                */
/* Output params:                                    */
/*****************************************************/
void MovementControl::updateMovement(int iAxisY, int iAxisX)
{
  // Clamp Values for a 10 bit range
  int iClampedAxisX = clampValues(iAxisX, 0, 1023);
  // Map Left Servo Values for movement valid range
  int iMapedAxisX = map(iAxisX, 0, 1023, MIN_VELOCITY_SERVO_VALUE, MAX_VELOCITY_SERVO_VALUE);
  // Update Left Servo dutycycle
  int iLeftDutyCycle = map(iMapedAxisX, 0, 1023, 0, 8888); // 10 bit to 16 bit map
  ledcWrite(LEFT_SERVO_CHANNEL, iLeftDutyCycle);

  // Clamp Values for a 10 bit range
  int iClampedAxisY = clampValues(iAxisY, 0, 1023);
  // Map Right Servo Values for movement valid range
  int iMapedAxisY = map(iAxisY, 0, 1023, MIN_VELOCITY_SERVO_VALUE, MAX_VELOCITY_SERVO_VALUE);
  // Update Right Servo dutycycle
  int iRightDutyCycle = map(iMapedAxisY, 0, 1023, 8888, 0); // 10 bit to 16 bit map, inverted for inverted direction
  ledcWrite(RIGHT_SERVO_CHANNEL, iRightDutyCycle);
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
/* Output params:      int iClampedValue - Clamped    */
/*                     value obtained.                */
/******************************************************/
int MovementControl::clampValues(int iValue, int iMinValue, int iMaxValue) {
  int iFinalValue = iValue;
  if (iMaxValue < iValue) {
    iFinalValue = iMaxValue;
  } else if (iMinValue > iValue) {
    iFinalValue = iMinValue;
  }
  return iFinalValue;
}

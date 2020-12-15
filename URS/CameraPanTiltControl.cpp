/*****************************************************/
/* File name:        CameraPanTiltControl.cpp        */
/* File description: File for the implementation     */
/*                   of cameraPanTiltControl Class,  */
/*                   the SG90 that moves the camera. */
/* Author name:      Juliane Moraes Vianna           */
/* Creation date:    20/11/2020                      */
/* Revision date:    20/11/2020                      */
/*****************************************************/

#include "Arduino.h"
#include "CameraPanTiltControl.h"

/******************************************************/
/* Method name:        initPanTiltControl             */
/* Method description: Method that attach the Servo   */
/*                     motors for Pan and Tilt.       */
/*                                                    */
/* Input params:       int iServoPanPin - Pin number  */
/*                     for the Pan Servo Motor.       */
/*                     int iServoTiltPin - Pin number */
/*                     for the Tilt Servo Motor.      */
/******************************************************/
CameraPanTiltControl::CameraPanTiltControl(int iServoPanPin, int iServoTiltPin)
{
  // Atach the servos considering the model SG90
  // with min/max of 400us and 2400us
  servPanServoMotor.attach(iServoPanPin, 500, 2200);
  servTiltServoMotor.attach(iServoTiltPin, 500, 2200);
  servTiltServoMotor.readMicroseconds();
  servPanServoMotor.writeMicroseconds(1500);
  servTiltServoMotor.writeMicroseconds(1500);
}

/*****************************************************/
/* Method name:        updatePosition                */
/* Method description: Method updates the PWM signal */
/*                     of each servo motor to move   */
/*                     the camera.                   */
/*                                                   */
/* Input params:     axisY - Value of the axis       */
/*                   position in Y. Range 0 - 1024   */
/*                   axisY - Value of the axis       */
/*                   position in X. Range 0 - 1024   */
/*****************************************************/
void CameraPanTiltControl::updatePosition(int iAxisY, int iAxisX)
{
  servPanServoMotor.writeMicroseconds(iAxisY);
  servTiltServoMotor.writeMicroseconds(iAxisX);
  //servPanServoMotor.write(iAxisY);
  //servTiltServoMotor.write(iAxisX);
}

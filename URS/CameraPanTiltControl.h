/*****************************************************/
/* File name:        CameraPanTiltControl.h          */
/* File description: File for the implementation     */
/*                   of CameraPanTiltControl Class,  */
/*                   the SG90 that moves the camera. */
/* Author name:      Juliane Moraes Vianna           */
/* Creation date:    20/11/2020                      */
/* Revision date:    20/11/2020                      */
/*****************************************************/

#ifndef CameraPanTiltControl_h
#define CameraPanTiltControl_h
#include "Arduino.h"
#include "ESP32_Servo.h"

/****************************************************/
/* Class name:        CameraPanTiltControl          */
/* Class description: Class that encapsulates the   */
/*                    services for the Servo Motor  */
/*                    SG90                          */
/****************************************************/
class CameraPanTiltControl
{
  private:
    // Variáveis Privadas:
    Servo servPanServoMotor;
    Servo servTiltServoMotor;

  public:

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
    CameraPanTiltControl(int iServoPanPin, int iServoTiltPin);

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
    void updatePosition(int iAxisY, int iAxisX);
};

#endif

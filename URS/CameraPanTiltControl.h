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

#ifndef CameraPanTiltControl_h
#define CameraPanTiltControl_h
#include "Arduino.h"

// Defines
#define SERVO_FREQUENCY_HZ          50   // Product Manufactor Manual Value 
#define PAN_SERVO_CHANNEL           8
#define TILT_SERVO_CHANNEL          9
#define PWM_RESOLUTION              16
#define PAN_CENTERING_VALUE         19   // Obtained by empirical manners
#define TILT_CENTERING_VALUE        0    // Obtained by empirical manners
#define MAX_PAN_SERVO_10BIT_VALUE   930  // Obtained by empirical manners
#define MIN_PAN_SERVO_10BIT_VALUE   190  // Obtained by empirical manners
#define MAX_TILT_SERVO_10BIT_VALUE  840  // Obtained by empirical manners
#define MIN_TILT_SERVO_10BIT_VALUE  200  // Obtained by empirical manners

/****************************************************/
/* Class name:        CameraPanTiltControl          */
/* Class description: Class that encapsulates the   */
/*                    services for the Pan Tilt     */
/*                    Support.                      */
/****************************************************/
class CameraPanTiltControl
{
  private:
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
    int clampValues(int iValue, int iMinValue, int iMaxValue);

  public:


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
    CameraPanTiltControl(int iServoTiltPin, int iServoPanPin);

    /*****************************************************/
    /* Method name:        updatePosition                */
    /* Method description: Method updates the PWM signal */
    /*                     of each servo motor to move   */
    /*                     the camera.                   */
    /*                                                   */
    /* Input params:     iAxisX - Value of the axis      */
    /*                   position in X. Range 0 - 1023   */
    /*                   iAxisY - Value of the axis      */
    /*                   position in Y. Range 0 - 1023   */
    /* Output params:                                    */
    /*****************************************************/
    void updatePosition(int iAxisX, int iAxisY);
};

#endif

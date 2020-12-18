/***************************************************/
/* File name:        MovementControl.h             */
/* File description: Header File for the           */
/*                   MovementControl Class, that   */
/*                   controls the servo motors     */
/*                   SM-S4303R for continium       */
/*                   rotation .                    */
/* Author name:      Paola Harduim                 */
/* Creation date:    20/11/2020                    */
/* Revision date:    17/12/2020                    */
/***************************************************/

#ifndef MovementControl_h
#define MovementControl_h
#include "Arduino.h"

// Defines
#define SERVO_FREQUENCY_HZ               50  // Product Manufactor Manual Value 
#define RIGHT_SERVO_CHANNEL              2
#define LEFT_SERVO_CHANNEL               3
#define PWM_RESOLUTION                   16
#define RIGHT_CENTERING_VALUE            0   // Obtained by empirical manners
#define LEFT_CENTERING_VALUE             0   // Obtained by empirical manners
#define MAX_VELOCITY_SERVO_VALUE         646 //Obtained by empirical manners
#define MIN_VELOCITY_SERVO_VALUE         376 //Obtained by empirical manners

/*****************************************************/
/* Class name:        MovementControl                */
/* Class description: Class that encapsulates the    */
/*                    services for the SM-S4303R     */
/*                    servo motor hardware.          */
/*****************************************************/
class MovementControl
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


    /*****************************************************/
    /* Creator name:       MovementControl               */
    /* Method description: Class Object creator          */
    /*                                                   */
    /* Input params:       iLeftServoPin - Pin associated*/
    /*                     to the PWM signal of the      */
    /*                     left servo. (int)             */
    /*                     iRightServoPin - Pin          */
    /*                     associated to the PWM signal  */
    /*                     of the right servo. (int)     */
    /* Output params:                                    */
    /*****************************************************/
    MovementControl(int iLeftServoPin, int iRightServoPin);

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
    void updateMovement(int iAxisY, int iAxisX);
};

#endif

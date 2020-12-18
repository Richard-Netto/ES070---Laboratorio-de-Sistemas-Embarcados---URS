/**************************************************/
/* File name:        SonarSensor.h                */
/* File description: Header File for the          */
/*                   SonarSensor Class, for the   */
/*                   HC-SC04 distance sensor.     */
/* Author name:      Richard Netto                */
/* Creation date:    20/11/2020                   */
/* Revision date:    18/12/2020                   */
/**************************************************/

#ifndef SonarSensor_h
#define SonarSensor_h
#include "Arduino.h"

/****************************************************/
/* Class name:        SonarSensor                   */
/* Class description: Class that encapsulates the   */
/*                    services for the Sonar Sensor */
/*                    HC-SC04 hardware.             */
/****************************************************/
class SonarSensor
{
  private:
    // Variáveis Privadas:
    int iEchoPin;
    int iTriggerPin;
    float fDuration;
    float fDistanceCm;
    float fA, fB;

  public:

    // Métodos Públicos

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
    /*                     fFittingA - Linear Fit       */
    /*                     contant of the angle obtained*/
    /*                     with the raw data of the     */
    /*                     sensor.(float)               */
    /*                     fFittingB - Linear Fit       */
    /*                     contant of the displacement  */
    /*                     obtained with the raw data   */
    /*                     of the sensor.(float)        */
    /* Output params:                                   */
    /****************************************************/
    SonarSensor(int iEchoPinNumber, int iTriggerPinNumber, float fFittingA, float fFittingB);

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
    /* Output params:                                   */
    /****************************************************/
    float getDistance();

};

#endif

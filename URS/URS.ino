
/**************************************************/
/* File name:        URS.ino                      */
/* File description: Main functon implementation  */
/*                   for the URS project in the   */
/*                   ESP32-CAM micro controller.  */
/* Author name:      Juliane Vianna               */
/* Creation date:    20/11/2020                   */
/* Revision date:    17/12/2020                   */
/**************************************************/

#include "SonarSensor.h"

// Defines
#define FLOOR_SENSOR_ECHO_PIN             14
#define WALL_SENSOR_ECHO_PIN              13
#define FLOOR_SENSOR_TRIGGER_PIN          15
#define WALL_SENSOR_TRIGGER_PIN           12
#define FLOOR_SENSOR_FITTING_A            54.6839 // Obtained by empirical manners
#define FLOOR_SENSOR_FITTING_B            5.7238  // Obtained by empirical manners
#define WALL_SENSOR_FITTING_A             54.6536 // Obtained by empirical manners
#define WALL_SENSOR_FITTING_B             -7.3905 // Obtained by empirical manners

// Variables
SonarSensor ssFloorSensor(FLOOR_SENSOR_ECHO_PIN, FLOOR_SENSOR_TRIGGER_PIN, FLOOR_SENSOR_FITTING_A, FLOOR_SENSOR_FITTING_B);
SonarSensor ssWallSensor(WALL_SENSOR_ECHO_PIN, WALL_SENSOR_TRIGGER_PIN, WALL_SENSOR_FITTING_A, WALL_SENSOR_FITTING_B);
float iFloorDistance;
float iWallDistance;
int iContMiliseconds;
hw_timer_t *hwTimer = NULL;

/******************************************************/
/* Method name:        updateFunction                 */
/* Method description: Callback function for the      */
/*                     Alarm Timer.                   */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void IRAM_ATTR updateFunction() {
  iContMiliseconds += 1;
  // Every 250 ms call
  if (iContMiliseconds % 1000 == 0) {
    printSensorData(iFloorDistance, iWallDistance);
  }
}

/******************************************************/
/* Method name:        initTimerAlarm                 */
/* Method description: Callback function for the      */
/*                     Alarm Timer.                   */
/*                                                    */
/* Input params:       int iTimerNumber - Timer number*/
/*                     to be set, can be one of the   */
/*                     4 timers. (0 to 3)             */
/*                     int iPrescaler - Preescaler to */
/*                     divide the timer Frequency, the*/
/*                     base value is 80 Mhz, so it's  */
/*                     common to use 80 to obtain     */
/*                     1 MHz. (16 bit)                */
/*                     int iAlarmPeriod - Multiplier  */
/*                     of the obtained frequency, to  */
/*                     set the timer. (For a          */
/*                     Preescaler of 80, 1000 will    */
/*                     result in a 1 ms Alarm period) */
/* Output params:                                     */
/******************************************************/
void initTimerAlarm(int iTimerNumber, int iPrescaler, int iAlarmPeriod) {
  //Timer setup
  // Inicia o timer 0 (of 4) e divide sua
  // frequência base por 80 (1 MHz de resultado)
  hwTimer = timerBegin(iTimerNumber, iPrescaler, true);
  // Adiciona uma função de retorno para a interrupção
  timerAttachInterrupt(hwTimer, &updateFunction, true);
  // Cria alarme para chamar a função a cada 1 ms
  timerAlarmWrite(hwTimer, iAlarmPeriod, true);
  // Inicia o Alarme
  timerAlarmEnable(hwTimer);
}

/******************************************************/
/* Method name:        setup                          */
/* Method description: Setup function of Arduino, used*/
/*                     only once on start.            */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void setup() {
  Serial.begin(115200);
  // Create Timer of 1 MHz with an alarm of 1 ms
  initTimerAlarm(0, 80, 1000);
}

/******************************************************/
/* Method name:        loop                           */
/* Method description: Loop function of Arduino, used */
/*                     continuosly after setup        */
/*                     function.                      */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void loop() {
  iFloorDistance = ssFloorSensor.getDistance();
  iWallDistance = ssWallSensor.getDistance();
  printSensorData(iFloorDistance, iWallDistance);
  delay(250);
}

/******************************************************/
/* Method name:        printSensorData                */
/* Method description: Test function to print to      */
/*                     serial monitor both sensor     */
/*                     values for verification and    */
/*                     calibration.                   */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void printSensorData(float iFloorDistance, float iWallDistance) {
  Serial.print("Floor Sensor: ");
  Serial.print(iFloorDistance);
  Serial.print(" cm                Wall Sensor: ");
  Serial.print(iWallDistance);
  Serial.println(" cm.");
  delay(500);
}


/**************************************************/
/* File name:        URS.ino                      */
/* File description: Main functon implementation  */
/*                   for the URS project in the   */
/*                   ESP32-CAM micro controller.  */
/* Author name:      Paola Harduim                */
/* Creation date:    20/11/2020                   */
/* Revision date:    17/12/2020                   */
/**************************************************/

// Library Includes
#include "MovementControl.h"

// Defines
#define LEFT_SERVO_PIN         15
#define PAN_SERVO_PIN          14


// Variables
int iLeftMotorPosition = 511;
int iRightMotorPosition = 511;
int iNewMotorPosition = 511;
MovementControl mcMovementControl(LEFT_SERVO_PIN, PAN_SERVO_PIN);
hw_timer_t *timer = NULL;
int iContMiliseconds = 0;

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
  // Every 10 ms call
  if (iContMiliseconds % 10 == 0) {
    mcMovementControl.updateMovement(iLeftMotorPosition, iRightMotorPosition);
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
  timer = timerBegin(iTimerNumber, iPrescaler, true);
  // Adiciona uma função de retorno para a interrupção
  timerAttachInterrupt(timer, &updateFunction, true);
  // Cria alarme para chamar a função a cada 1 ms
  timerAlarmWrite(timer, iAlarmPeriod, true);
  // Inicia o Alarme
  timerAlarmEnable(timer);
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
  while (Serial.available()) {
    iNewMotorPosition = Serial.parseInt();
    Serial.println(iNewMotorPosition);
    testFunction_1(iNewMotorPosition);
    testFunction_2(iNewMotorPosition);
  }
}

/******************************************************/
/* Method name:        testFunction_1                 */
/* Method description: Function to test the movement  */
/*                     control, it will move the car  */
/*                     foward and backward until the  */
/*                     desired value.                 */
/*                                                    */
/* Input params:       int pos - Value of the máx     */
/*                     velocity.                      */
/* Output params:                                     */
/******************************************************/
void testFunction_1(int iPosition){
  Serial.println("Foward");
  iLeftMotorPosition = 511 +(iPosition - 511);
  iRightMotorPosition = 511 +(iPosition - 511);
  delay(2000);
  Serial.println("Backward");
  iLeftMotorPosition = 511 -(iPosition - 511);
  iRightMotorPosition = 511 -(iPosition - 511);
  delay(2000);
  
  iLeftMotorPosition = 511;
  iRightMotorPosition = 511;
}

/******************************************************/
/* Method name:        testFunction_2                 */
/* Method description: Function to test the movement  */
/*                     control, it will rotate the car*/ 
/*                     in place in the desired        */
/*                     velocity.                      */
/*                                                    */
/* Input params:       int pos - Value of the máx     */
/*                     velocity.                      */
/* Output params:                                     */
/******************************************************/
void testFunction_2(int iPosition){
  iLeftMotorPosition = 511 +(iPosition - 511);
  iRightMotorPosition = 511 -(iPosition - 511);
  delay(2000);
  iLeftMotorPosition = 511 -(iPosition - 511);
  iRightMotorPosition = 511 +(iPosition - 511);
  delay(2000);
  iLeftMotorPosition = 511;
  iRightMotorPosition = 511;
}

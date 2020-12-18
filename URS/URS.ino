
/**************************************************/
/* File name:        URS.ino                      */
/* File description: Main functon implementation  */
/*                   for the URS project in the   */
/*                   ESP32-CAM micro controller.  */
/* Author name:      Richard Netto                */
/* Creation date:    20/11/2020                   */
/* Revision date:    16/12/2020                   */
/**************************************************/

// Library Includes
#include "CameraPanTiltControl.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

// Defines
#define TILT_SERVO_PIN         12
#define PAN_SERVO_PIN          13


// Variables
int iAxisX = 511;
int iAxisY = 511;
CameraPanTiltControl cptCameraPanTiltControl(TILT_SERVO_PIN, PAN_SERVO_PIN);
hw_timer_t *hwTimer = NULL;
int iContMiliseconds = 0;
HTTPClient httpClient;
const char* ssid = "Quarto";
const char* password = "Netto2014";
const char* serverName = "http://blynk-cloud.com/2bJCP-DrOGFj3RMy1Rm76e8N3_54lLk8/get/V2";

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
    cptCameraPanTiltControl.updatePosition(iAxisX, iAxisY);
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

  // Criar função para conexão com internet
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  httpClient.begin(serverName);

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
  //  while (Serial.available()) {
  //    pos = Serial.parseInt();
  //    Serial.println(pos)
  //  }
  int httpCode = httpClient.GET();
  String payload = httpClient.getString();
  JSONVar myArray = JSON.parse(payload);
  iAxisX = JSON.parse(myArray[0]);
  iAxisY = JSON.parse(myArray[1]);
  delay(20);
}

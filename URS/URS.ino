
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
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include "OV2640.h"
#include "CameraPanTiltControl.h"
#include "MovementControl.h"
#include "SonarSensor.h"

// Defines
#define PWDN_GPIO_NUM              32
#define RESET_GPIO_NUM             -1
#define XCLK_GPIO_NUM              00
#define SIOD_GPIO_NUM              26
#define SIOC_GPIO_NUM              27
#define Y9_GPIO_NUM                35
#define Y8_GPIO_NUM                34
#define Y7_GPIO_NUM                39
#define Y6_GPIO_NUM                36
#define Y5_GPIO_NUM                21
#define Y4_GPIO_NUM                19
#define Y3_GPIO_NUM                18
#define Y2_GPIO_NUM                05
#define VSYNC_GPIO_NUM             25
#define HREF_GPIO_NUM              23
#define PCLK_GPIO_NUM              22

#define TILT_SERVO_PIN             12
#define PAN_SERVO_PIN              13

#define LEFT_SERVO_PIN             15
#define RIGHT_SERVO_PIN            14

#define FRONT_SENSOR_ECHO_PIN      02
#define FRONT_SENSOR_TRIGGER_PIN   04
#define FRONT_SENSOR_FITTING_A     54.6839 // Obtained by empirical manners
#define FRONT_SENSOR_FITTING_B     5.7238  // Obtained by empirical manners
#define FRONT_SENSOR_STOP_DISTANCE 12

#define WIFI_SSID "Quarto"
#define WIFI_PWD "Netto2014"

// Variables
OV2640 cam;
WebServer server(80);
const char HEADER[] = "HTTP/1.1 200 OK\r\n" \
                      "Access-Control-Allow-Origin: *\r\n" \
                      "Content-Type: multipart/x-mixed-replace; boundary=123456789000000000000987654321\r\n";
const char BOUNDARY[] = "\r\n--123456789000000000000987654321\r\n";
const char CTNTTYPE[] = "Content-Type: image/jpeg\r\nContent-Length: ";
const int hdrLen = strlen(HEADER);
const int bdrLen = strlen(BOUNDARY);
const int cntLen = strlen(CTNTTYPE);
const char JHEADER[] = "HTTP/1.1 200 OK\r\n" \
                       "Content-disposition: inline; filename=capture.jpg\r\n" \
                       "Content-type: image/jpeg\r\n\r\n";
const int jhdLen = strlen(JHEADER);
int iContMiliseconds = 0;
MovementControl mcMovementControl(LEFT_SERVO_PIN, RIGHT_SERVO_PIN);
CameraPanTiltControl cptCameraPanTiltControl(TILT_SERVO_PIN, PAN_SERVO_PIN);
SonarSensor ssFloorSensor(FRONT_SENSOR_ECHO_PIN, FRONT_SENSOR_TRIGGER_PIN, FRONT_SENSOR_FITTING_A, FRONT_SENSOR_FITTING_B);
int iTempX = 511;
int iTempY = 511;
int iAxisX = 511;
int iAxisY = 511;
int iLeftMotorPosition = 511;
int iRightMotorPosition = 511;
float iFloorDistance;
boolean bThereIsNoFloor = false;
hw_timer_t *hwTimer = NULL;
HTTPClient httpClientPanTilt;
HTTPClient httpClientMovement;
const char* panTiltServerName = "http://blynk-cloud.com/dGnsTBbfB-q_hLBMolkq0zI2Qafz5tbA/get/V2";
const char* movementServerName = "http://blynk-cloud.com/dGnsTBbfB-q_hLBMolkq0zI2Qafz5tbA/get/V1";

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
    // If there's no Floor, stop the car from going foward
    if (bThereIsNoFloor && 511 < iLeftMotorPosition ) iLeftMotorPosition = 511;
    if (bThereIsNoFloor && 511 < iRightMotorPosition) iRightMotorPosition = 511;
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
  hwTimer = timerBegin(iTimerNumber, iPrescaler, true);
  // Adiciona uma função de retorno para a interrupção
  timerAttachInterrupt(hwTimer, &updateFunction, true);
  // Cria alarme para chamar a função a cada 1 ms
  timerAlarmWrite(hwTimer, iAlarmPeriod, true);
  // Inicia o Alarme
  timerAlarmEnable(hwTimer);
}

/******************************************************/
/* Method name:        handleJpegStream               */
/* Method description: Function to handle the jpeg    */
/*                     stream of camera images.       */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void handleJpegStream(void)
{
  char buf[32];
  int s;

  WiFiClient client = server.client();

  client.write(HEADER, hdrLen);
  client.write(BOUNDARY, bdrLen);
  int iTemp = 0;
  while (true)
  {
    if (!client.connected()) break;
    if (10 < iTemp) {
      handleVariables();
      iTemp = 0;
    }
    iTemp++;
    cam.run();
    s = cam.getSize();
    client.write(CTNTTYPE, cntLen);
    sprintf( buf, "%d\r\n\r\n", s );
    client.write(buf, strlen(buf));
    client.write((char *)cam.getfb(), s);
    client.write(BOUNDARY, bdrLen);
  }
}

/******************************************************/
/* Method name:        handleNotFound                 */
/* Method description: Function to erros on image     */
/*                     handle.                        */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void handleNotFound()
{
  String message = "Server is running!\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(200, "text / plain", message);
}

/******************************************************/
/* Method name:        initCamera                     */
/* Method description: Set up camera configurations   */
/*                     for the best configuration for */
/*                     fast streaming.                */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void initCamera(void)
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Frame parameters
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 50;
  config.fb_count = 2;

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  cam.init(config);
}

/******************************************************/
/* Method name:        handleVariables                */
/* Method description: Function to apply the update of*/ 
/*                     the variables necessary for    */
/*                     movement and distance          */
/*                     measurement.                   */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void handleVariables(void) {
  // Pan Tilt Variables
  int httpCode = httpClientPanTilt.GET();
  String payload = httpClientPanTilt.getString();
  JSONVar myArray = JSON.parse(payload);
  iAxisX = JSON.parse(myArray[0]);
  iAxisY = JSON.parse(myArray[1]);
  // Movement Variables
  httpCode = httpClientMovement.GET();
  payload = httpClientMovement.getString();
  myArray = JSON.parse(payload);
  iTempX = JSON.parse(myArray[0]);
  iTempY = JSON.parse(myArray[1]);
  iLeftMotorPosition = 511 + (iTempY - 511) - (iTempX - 511) * 0.40;
  iRightMotorPosition = 511 + (iTempY - 511) + (iTempX - 511) * 0.40;
  // Sensor Variables
  iFloorDistance = ssFloorSensor.getDistance();
  if (FRONT_SENSOR_STOP_DISTANCE < iFloorDistance)bThereIsNoFloor = true;
  else bThereIsNoFloor = false;
}
/******************************************************/
/* Method name:        initWiFi                       */
/* Method description: Set up Wifi connection, to the */
/*                     desired WiFi Network, and print*/
/*                     the current link for the       */
/*                     streaming.                     */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void initWiFi(void)
{
  IPAddress ip;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }
  ip = WiFi.localIP();
  Serial.println(F("WiFi connected"));
  Serial.println("");
  Serial.print("Stream Link: http://");
  Serial.print(ip);
  Serial.println("/mjpeg/1");
  server.on("/mjpeg/1", HTTP_GET, handleJpegStream);
  server.onNotFound(handleNotFound);
  server.begin();
}

/******************************************************/
/* Method name:        setup                          */
/* Method description: Setup function of Arduino, used*/
/*                     only once at the start.        */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void setup()
{
  Serial.begin(115200);
  initCamera();
  initWiFi();
  httpClientPanTilt.begin(panTiltServerName);
  httpClientMovement.begin(movementServerName);
  initTimerAlarm(1, 80, 1000); // Create Timer of 1 MHz with an alarm of 1 ms
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
void loop()
{
  server.handleClient();
}

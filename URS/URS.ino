
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
#include "src/OV2640.h"
#include "camera_pins.h"

// Defines
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER
#define SSID1 "Quarto"
#define PWD1 "Netto2014"

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

/******************************************************/
/* Method name:        handleJpegStream                 */
/* Method description: Callback function for the      */
/*                     Alarm Timer.                   */
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

  while (true)
  {
    if (!client.connected()) break;
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
/* Method name:        updateFunction                 */
/* Method description: Callback function for the      */
/*                     Alarm Timer.                   */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void handle_jpg(void)
{
  WiFiClient client = server.client();

  cam.run();
  if (!client.connected()) return;

  client.write(JHEADER, jhdLen);
  client.write((char *)cam.getfb(), cam.getSize());
}

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
/* Method name:        setup                          */
/* Method description: Setup function of Arduino, used*/
/*                     only once on start.            */
/*                                                    */
/* Input params:                                      */
/* Output params:                                     */
/******************************************************/
void setup()
{

  Serial.begin(115200);
  //while (!Serial);            //wait for serial connection.

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
  //  config.frame_size = FRAMESIZE_UXGA;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 63;
  config.fb_count = 2;

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  cam.init(config);

  IPAddress ip;

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID1, PWD1);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }
  ip = WiFi.localIP();
  Serial.println(F("WiFi connected"));
  Serial.println("");
  Serial.println(ip);
  Serial.print("Stream Link: http://");
  Serial.print(ip);
  Serial.println("/mjpeg/1");
  server.on("/mjpeg/1", HTTP_GET, handle_jpg_stream);
  server.on("/jpg", HTTP_GET, handle_jpg);
  server.onNotFound(handleNotFound);
  server.begin();
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


/**************************************************/
/* File name:        URS.ino                      */
/* File description: Main functon implementation  */
/*                   for the URS project in the   */
/*                   ESP32-CAM micro controller.  */
/* Author name:      Richard Netto                */
/* Creation date:    20/11/2020                   */
/* Revision date:    20/11/2020                   */
/**************************************************/

/*
  1 - Criar funções básicas de servo motores e variáveis
  2 - Preparar para receber valores de 10 bits do Blynk com direção
  3 - Criar função de update com Timer
  4 - Fazer funções Teste
*/

// Library Includes
#include "CameraPanTiltControl.h"
// Variables
CameraPanTiltControl cameraControl(15,14);

// Setup Function
void setup() {
  
}

// Loop Function
void loop() {
  cameraControl.updatePosition(90,90);
}

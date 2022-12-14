#include "Server.h"
//Retroalimentacion
const int ret1 = 4;  //4
const int ret2 = 16;  //16
const int ret3 = 17;  //17
const int ret4 = 5;  //5


int val1aux;
int val2aux;
int val3aux;
int val4aux;
int firstMessage = 0;

/////////// SETUP ////////////////
void setup()
{
  Serial.begin(115200);
  //Inicia los recursos necesarios para hacer llamadas al servidor
  ServerSetup();
  botSetup();
  //Retroalimentacion
  pinMode(ret1, INPUT);
  pinMode(ret2, INPUT);
  pinMode(ret3, INPUT);
  pinMode(ret4, INPUT);
}
/////////// LOOP ////////////////
void loop ()
{
  //Importamos las funciones necesarias para escuchar y emitir datos del servidor
  ServerLoop();

  //Hacemos la retroalimentacion de los pines
  val1 = digitalRead(ret1);
  val2 = digitalRead(ret2);
  val3 = digitalRead(ret3);
  val4 = digitalRead(ret4);

  if (firstMessage == 0)
  {
    val1aux = val1;
    val2aux = val2;
    val3aux = val3;
    val4aux = val4;
    firstMessage++;
  }
  if (val1 != val1aux || val2 !=  val2aux || val3 != val3aux || val4 != val4aux)
  {
    ChangeEvent();
  }
  getMessages();
  ///////////////////////////////////////////////////////////////////////////////
}

///////////Retroalimentacion del server////////////////
void ChangeEvent()
{
  val1aux = val1;
  val2aux = val2;
  val3aux = val3;
  val4aux = val4;
  SendFeedBack(val1aux, val2aux, val3aux, val4aux);
}

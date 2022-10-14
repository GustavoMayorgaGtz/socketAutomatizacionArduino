#include "Actions.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#define BOT_TOKEN "5598092541:AAFghLtCUvLHS_Wr8lSP2Gc8HRudCRude_E"
const unsigned long tiempo = 1000;
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long tiempoAnterior;

int estadoLed12 = 0;
int estadoLed14 = 0;
int estadoLed27 = 0;
int estadoLed26 = 0;
String chat_id;

int val1;
int val2;
int val3;
int val4;

#define ID_Chat "636820507"
void mensajesNuevos(int numerosMensajes)
{
  for (int i = 0; i < numerosMensajes; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
   //////////Luz 1 en el pin 12//////
    if (text == "/Luz1on")
    {
      switchMenu("D1", true);
      estadoLed12 = 1;
      bot.sendMessage(chat_id, "Luz 1 encendida", "");
    }

    if (text == "/SwitchL1")
    {
      estadoLed12 = 0;
      switchMenu("D1", false);
      bot.sendMessage(chat_id, "Luz 1 apagada", "");
    }
    //////////Luz 2 en el pin 14//////
    if (text == "/Luz2on")
    {
     switchMenu("D2", true);
      estadoLed14 = 1;
      bot.sendMessage(chat_id, "Luz 2 encendida", "");
    }

    if (text == "/SwitchL2")
    {
      estadoLed14 = 0;
      switchMenu("D2", false);
      bot.sendMessage(chat_id, "Luz 2 apagada", "");
    }
    //////////Luz 3 en el pin 27//////
    if (text == "/Luz3on")
    {
      switchMenu("D3", true);
      estadoLed27 = 1;
      bot.sendMessage(chat_id, "Luz 3 encendida", "");
    }

    if (text == "/SwitchL3")
    {
      estadoLed27 = 0;
      switchMenu("D3", false);
      bot.sendMessage(chat_id, "Luz 3 apagada", "");
    }
    //////////Luz 4 en el pin 26//////
    if (text == "/Luz4on")
    {
      switchMenu("D4", true);
      estadoLed26 = 1;
      bot.sendMessage(chat_id, "Luz 4 encendida", "");
    }

    if (text == "/SwitchL4")
    {
      estadoLed26 = 0;
      switchMenu("D4", false);
      bot.sendMessage(chat_id, "Luz 4 apagada", "");
    }
    ////////Estado de las luces ///////

    if (text == "/Estado")
    {
      ////Estado luz 1////
      if  (val1 == HIGH)
      {
        bot.sendMessage(chat_id, "Luz 1 encendida", "");
      }
      else
      {
        bot.sendMessage(chat_id, "Luz 1 apagada", "");
      }
      ////Estado luz 2////
      if (val2 == HIGH)
      {
        bot.sendMessage(chat_id, "Luz 2 encendida", "");
      }
      else
      {
        bot.sendMessage(chat_id, "Luz 2 apagada", "");
      }
      ////Estado luz 3////
      if (val3 == HIGH)
      {
        bot.sendMessage(chat_id, "Luz 3 encendida", "");
      }
      else
      {
        bot.sendMessage(chat_id, "Luz 3 apagada", "");
      }
      ////Estado luz 4////
      if (val4 == HIGH)
      {
        bot.sendMessage(chat_id, "Luz 4 encendida", "");
      }
      else
      {
        bot.sendMessage(chat_id, "Luz 4 apagada", "");
      }
    }

    if (text == "/Ayuda")
    {
      String ayuda = "Bienvenido al sistema de control luces en Controler con Esp32, " ".\n";
      ayuda += "Estas son tus opciones.\n\n";
      ayuda += "/Luz1on: para encender la Luz 1 \n";
      ayuda += "/SwitchL1: para apagar la luz 1 \n";
      ayuda += "/Luz2on: para encender la Luz 2 \n";
      ayuda += "/SwitchL2: para apagar la luz 2 \n";
      ayuda += "/Luz3on: para encender la Luz 3 \n";
      ayuda += "/SwitchL3: para apagar la luz 3 \n";
      ayuda += "/Luz4on: para encender la Luz 4 \n";
      ayuda += "/SwitchL4: para apagar la luz 4 \n";
      ayuda += "/Estado : devuelve el estado actual de las luces\n";
      ayuda += "/Ayuda: Imprime este menú \n";
      ayuda += "Recuerda el sistema distingue entre mayuculas y minusculas \n\n";
      ayuda += "Controler MX \n";
      bot.sendMessage(chat_id, ayuda, "");
    }

  }
}

int iterator1 = 1;
void botSetup()
{
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
     if(iterator1 == 1)
  {
   Serial.println("Sistema preparado");
   bot.sendMessage(ID_Chat, "Sistema preparado!!!, escribe /Ayuda para ver las opciones", "");//Enviamos un mensaje a telegram para informar que el sistema está listo
   iterator1 = 0;
  }
}

void getMessages()
{
    //Verifica si hay datos nuevos en telegram cada 1 segundo
  if (millis() - tiempoAnterior > tiempo)
  {
    int numerosMensajes = bot.getUpdates(bot.last_message_received + 1);

    while (numerosMensajes)
    {
      Serial.println("Comando recibido");
      mensajesNuevos(numerosMensajes);
      numerosMensajes = bot.getUpdates(bot.last_message_received + 1);
    }

    tiempoAnterior = millis();
  }

}

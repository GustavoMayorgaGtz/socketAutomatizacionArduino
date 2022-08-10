#include <WiFi.h>
#include <WebSocketClient.h>
#include <ArduinoJson.h>
#include "Actions.h"

WebSocketClient socket;
WiFiClient client;
char path[] = "/message";
char host[] = "192.168.100.129";

#define ssid "ECLAutomatizacion_2.4Gnormal"
#define pass "KD6JVB8kmCRe"
int iterator = 0;

#define light1 12
#define light2 14
#define air1 27

/***********************************************************/
/***********************************************************/
void connectSocket()
{
  Serial.println("Conectando a servidor");
  if (client.connect("192.168.100.129", 3000))
  {
    Serial.println("Conectado a servidor");
  } else
  {
    Serial.println("No se pudo establecer comunicacion con el servidor");
  }

  socket.path = path;
  socket.host = host;
  if (socket.handshake(client))
  {
    Serial.println("Conectado a Grupo!");
  } else
  {
    Serial.println("No se pudo conectar al Grupo :(");
  }
}
/***********************************************************/
/***********************************************************/
void ServerSetup()
{
  Serial.begin(115200);
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(air1, OUTPUT);

  WiFi.begin(ssid, pass);
  Serial.println("Conectando a la red");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println("Conectado");
  connectSocket();

}
/***********************************************************/
/***********************************************************/
StaticJsonDocument<200> getData(String data)
{
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, data);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }
  return  doc;
}
/***********************************************************/
/***********************************************************/
void ServerLoop()
{
  String data;
  if (client.connected())
  {
    if (iterator == 0)
    {
      Serial.println("Canal abierto");
      iterator++;
    }
    socket.getData(data);
    if (data.length() > 0)
    {
      Serial.println("Datos recibidos:");
      Serial.println(data);
      StaticJsonDocument<200> doc = getData(data);
      String  option = doc["option"];
      bool message = doc["message"];
      if(option == "Light1" || option == "Light2" || option == "Air1")
      {
        switchMenu(option, message);  
      } 
    }
  } else
  {
    Serial.println("Usuario Desconectado");
    connectSocket();
  }
}
/***********************************************************/
/***********************************************************/

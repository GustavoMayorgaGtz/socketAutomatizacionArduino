#include <WiFi.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <ArduinoJson.h>
#include "Telegram.h"

SocketIOclient socketIO;
WiFiClient client;

#define ssid "ECLAutomatizacion_2.4Gnormal"
#define pass "KD6JVB8kmCRe"
int iterator = 0;

#define light1 12
#define light2 14

#define light3 25
#define light4 27
#define air1 27

long tiempo1 = 0;
long tiempo2 = 0;
long tiempoSegundos = 0;
long tiempoReset = 0;

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
void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
  switch (type) {


    case sIOtype_DISCONNECT:
      Serial.printf("[IOc] Disconnected!\n");
      socketIO.onEvent(socketIOEvent);
      break;


    case sIOtype_CONNECT:
      Serial.printf("[IOc] Connected to url: %s\n", payload);
      socketIO.send(sIOtype_CONNECT, "/");
      break;


    case sIOtype_EVENT:
      {
        char * sptr = NULL;
        int id = strtol((char *)payload, &sptr, 10);
        Serial.printf("[IOc] get event: %s id: %d\n", payload, id);

        String str = (char*)payload;
        char L = str.length();
        String new_str = str.substring(9, L - 1);
        StaticJsonDocument<200> recive = getData(new_str);
        String  option = recive["Device"];
        bool message = recive["isOn"];
        Serial.println(option);
        Serial.println(message);
        switchMenu(option, message);
        if (option == "Acuse")
        {
          digitalWrite(light1, HIGH);
          delay(1000);
          digitalWrite(light1, LOW);
          tiempoReset = 0;
        }
        Serial.println(new_str);
        if (id) {
          payload = (uint8_t *)sptr;
        }
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload, length);
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str());
          return;
        }
        String eventName = doc[0];
        if (id) {
          DynamicJsonDocument docOut(1024);
          JsonArray array = docOut.to<JsonArray>();
          JsonObject param1 = array.createNestedObject();
          param1["now"] = millis();
          String output;
          output += id;
          serializeJson(docOut, output);
          socketIO.send(sIOtype_ACK, output);
        }
      }
      break;


    case sIOtype_ACK:
      Serial.printf("[IOc] get ack: %u\n", length);
      break;


    case sIOtype_ERROR:
      Serial.printf("[IOc] get error: %u\n", length);
      break;


    case sIOtype_BINARY_EVENT:
      Serial.printf("[IOc] get binary: %u\n", length);
      break;


    case sIOtype_BINARY_ACK:
      Serial.printf("[IOc] get binary ack: %u\n", length);
      break;
  }
}

/***********************************************************/
/***********************************************************/
void ServerSetup()
{
  Serial.begin(115200);

  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);
  pinMode(light4, OUTPUT);
  pinMode(air1, OUTPUT);

  WiFi.begin(ssid, pass);

  Serial.println("Conectando a la red");

  int iterator = 0;

  while (WiFi.status() != WL_CONNECTED)
  {
    iterator++;
    Serial.print(".");
    delay(300);
    if (iterator >= 10)
    {
      ESP.restart();
    }
  }
  Serial.println("Conectado");
  socketIO.begin("192.168.100.142", 3000, "/socket.io/?EIO=4");
  socketIO.onEvent(socketIOEvent);
  tiempo1 = millis();
}

/***********************************************************/
//Mandar acuse de conexion con servidor
void Acuse()
{
  // creat JSON message for Socket.IO (event)
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();

  // add evnet name
  // Hint: socket.on('Light', ....
  array.add("Light");
  // add payload (parameters) for the event
  JsonObject param1 = array.createNestedObject();
  param1["Acuse"] = true;

  // JSON to String (serializion)
  String output;
  serializeJson(doc, output);

  // Send event
  socketIO.sendEVENT(output);

  // Print JSON for debugging
  Serial.println(output);
}
/***********************************************************/

void ServerLoop()
{
  socketIO.loop();
  tiempo2 = millis();

  if (tiempo2 > (tiempo1 + 1000)) {
    tiempo1 = millis();
    //Este tiempo es utilizado para el envio de acuse hacia el servidor cada 30 sg
    tiempoSegundos++;
    //Este tiempo se utiliza para reiniciar el microcontrolador si no recibe una respuesta del servidor cada 60 sg
    tiempoReset++;
    //Funcion para establecer la salida de los pines cada segundo
    setState();
    if (tiempoReset >= 60)
    {
      ESP.restart();
    }
    if (tiempoSegundos >= 30)
    {
      tiempoSegundos = 0;
      Acuse();
    }
  }
}

/***********************************************************/
void SendFeedBack(int l1, int l2, int l3, int l4)
{

  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();

  array.add("Light");

  JsonObject param1 = array.createNestedObject();
  param1["feedback"] = true;
  param1["Light1"] = l1;
  param1["Light2"] = l2;
  param1["Light3"] = l3;
  param1["Light4"] = l4;

  String output;

  serializeJson(doc, output);

  socketIO.sendEVENT(output);

  Serial.println(output);
}

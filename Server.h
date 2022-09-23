#include <WiFi.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <ArduinoJson.h>
#include "Actions.h"

SocketIOclient socketIO;
WiFiClient client;

#define ssid "ECLAutomatizacion_2.4Gnormal"
#define pass "KD6JVB8kmCRe"
int iterator = 0;

#define light1 12
#define light2 14
#define air1 27

long time1 = 0;
#define interval 60000
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
/***********************************************************/
/***********************************************************/
void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      Serial.printf("[IOc] Disconnected!\n");
      break;
    case sIOtype_CONNECT:
      Serial.printf("[IOc] Connected to url: %s\n", payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");
      break;
    case sIOtype_EVENT:
      {
        char * sptr = NULL;
        int id = strtol((char *)payload, &sptr, 10);
        Serial.printf("[IOc] get event: %s id: %d\n", payload, id);

        String str = (char*)payload;
        char L = str.length();
        String new_str = str.substring(42, L - 2);
        StaticJsonDocument<200> recive = getData(new_str);
        String  option = recive["nombre"];
        bool message = recive["isOn"];
        Serial.println(option);
        Serial.println(message);
        switchMenu(option, message);
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
        Serial.printf("[IOc] event name: %s\n", eventName.c_str());

        // Message Includes a ID for a ACK (callback)
        if (id) {
          // creat JSON message for Socket.IO (ack)
          DynamicJsonDocument docOut(1024);
          JsonArray array = docOut.to<JsonArray>();

          // add payload (parameters) for the ack (callback function)
          JsonObject param1 = array.createNestedObject();
          param1["now"] = millis();

          // JSON to String (serializion)
          String output;
          output += id;
          serializeJson(docOut, output);

          // Send event
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
  // server address, port and URL
  socketIO.begin("192.168.100.142", 3000, "/socket.io/?EIO=4");

  // event handler
  socketIO.onEvent(socketIOEvent);

}
/***********************************************************/
/***********************************************************/
void ServerLoop()
{
  socketIO.loop();
}
/***********************************************************/
/***********************************************************/
void SendFeedBack(int l1, int l2, int l3, int l4)
{
  // creat JSON message for Socket.IO (event)
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();

  // add evnet name
  // Hint: socket.on('Light', ....
  array.add("Light");
  // add payload (parameters) for the event
  JsonObject param1 = array.createNestedObject();
  param1["feedback"] = true;
  param1["Light1"] = l1;
  param1["Light2"] = l2;
  param1["Light3"] = l3;
  param1["Light4"] = l4;

  // JSON to String (serializion)
  String output;
  serializeJson(doc, output);

  // Send event
  socketIO.sendEVENT(output);

  // Print JSON for debugging
  Serial.println(output);
}
/***********************************************************/
/***********************************************************/

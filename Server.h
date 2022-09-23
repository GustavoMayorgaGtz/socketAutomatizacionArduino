#include <WiFi.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <ArduinoJson.h>
#include "Actions.h"

//WebSocketClient socket;
SocketIOclient socketIO;
WiFiClient client;
//char path[] = "message";
//char host[] = "192.168.100.142/socket.io/?EIO=3";

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
/*void connectSocket()
{
  Serial.println("Conectando a servidor");
  if (client.connect("192.168.100.142/socket.io/?EIO=3", 3000)) //Conectando a servidor
  {
    Serial.println("Conectado a servidor");

  } else
  {
    //Si no se puede conectar a servidor, reiniciar el servidor
    Serial.println("No se pudo establecer comunicacion con el servidor");
    ESP.restart();
  }
  
    socket.path = path;
    socket.host = host;
    if (socket.handshake(client)) //Conectando al grupo
    {
    Serial.println("Conectado a Grupo!");
    } else
    {
    Serial.println("No se pudo conectar al Grupo :(");
    ESP.restart();
    }
}*/
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
unsigned long messageTimestamp = 0;
void ServerLoop()
{


    socketIO.loop();
    
    uint64_t now = millis();

    if(now - messageTimestamp > 2000) {
        messageTimestamp = now;

        // creat JSON message for Socket.IO (event)
        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();

        // add evnet name
        // Hint: socket.on('event_name', ....
        array.add("Light");

        // add payload (parameters) for the event
        JsonObject param1 = array.createNestedObject();
        param1["feedback"] = true;
        param1["Light1"] = true;
        param1["Light2"] = false;
        param1["Light3"] = true;
        param1["Light4"] = false;
        
        // JSON to String (serializion)
        String output;
        serializeJson(doc, output);

        // Send event
        socketIO.sendEVENT(output);

        // Print JSON for debugging
        Serial.println(output);
    }
/*  long time2 = millis();
  if (time2 - time1 >= interval)
  {
    time1 = time2;
    ESP.restart();
  }
  //Entrada a Servidor
  String data;
  if (client.connected()) //Verificar conexion a servidor
  {
    if (iterator == 0)
    {
      Serial.println("Canal abierto");
      iterator++;
    }*/
    /* socket.getData(data); //Obtenemos informacion del servidor
      socket.sendData("hOLA MICRO");
      if (data.length() > 0) //Si tiene caracteres significa que hay respuesta y la procesaremos
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
      }*/
}
/***********************************************************/
/***********************************************************/

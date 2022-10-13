#include <Arduino.h>
#define light1 12
#define light2 14
#define light3 26
#define light4 27
#define air1 28

//Retardo de accion para el estado de las luces
boolean stateLight1 = false;
boolean stateLight2 = false;
boolean stateLight3 = false;
boolean stateLight4 = false;
boolean stateAir1 = false;

void setState(){
  digitalWrite(light1, stateLight1);
  digitalWrite(light2, stateLight2);
  digitalWrite(light3, stateLight3);
  digitalWrite(light4, stateLight4);
  digitalWrite(air1, stateAir1);
}

void switchMenu(String option, bool message)
{
  if (option == "Acuse")
  {
     Serial.println("Llego el acuse de servidor");
  }
  
  if (option == "D1")
  {
    Serial.println("Turn Light1");
    stateLight1 = message;
  }
  
  if (option == "D2")
  {
    Serial.println("Turn Light2");
    stateLight2 = message;
  }
  
  if (option == "D3")
  {
    Serial.println("Turn Light3");
    stateLight3 = message;
  }
  
  if (option == "D4")
  {
    Serial.println("Turn Light4");
    stateLight4 = message;
  }

  if (option == "D5")
  {
    Serial.println("Turn Air");
    stateAir1 = message;
  }  
}

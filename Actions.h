#include <Arduino.h>
#define L1 12
#define L2 14
#define L3 26
#define L4 27
#define A1 28

//Retardo de accion para el estado de las luces
boolean estadosLight1 = false;
boolean estadosLight2 = false;
boolean estadosLight3 = false;
boolean estadosLight4 = false;
boolean stateAir1 = false;

void setState(){
  digitalWrite(L1, estadosLight1);
  digitalWrite(L2, estadosLight2);
  digitalWrite(L3, estadosLight3);
  digitalWrite(L4, estadosLight4);
  digitalWrite(A1, stateAir1);
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
    estadosLight1 = message;
  }
  
  if (option == "D2")
  {
    Serial.println("Turn Light2");
    estadosLight2 = message;
  }
  
  if (option == "D3")
  {
    Serial.println("Turn Light3");
    estadosLight3 = message;
  }
  
  if (option == "D4")
  {
    Serial.println("Turn Light4");
    estadosLight4 = message;
  }

  if (option == "D5")
  {
    Serial.println("Turn Air");
    stateAir1 = message;
  }  
}

#include <Arduino.h>
#define light1 12
#define light2 14
#define light3 25
#define light4 27
#define air1 28

void switchMenu(String option, bool message)
{
    if(option == "Light1")
    {
        digitalWrite(light1, message);
        Serial.println("Turn Light1");
    }
    if(option == "Light2")
    {
      digitalWrite(light2, message); 
      Serial.println("Turn Light2");
    }
    if(option == "Light3")
    {
      digitalWrite(light3, message); 
      Serial.println("Turn Light3");
    }
    if(option == "Light4")
    {
      digitalWrite(light4, message); 
      Serial.println("Turn Light4");
    }    

    if(option == "Air1")
    {
      digitalWrite(air1, message); 
      Serial.println("Turn Air");
    }
        
}

#include <Arduino.h>
#define light1 12
#define light2 14
#define air1 27

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
      digitalWrite(air1, message); 
      Serial.println("Turn Light3");
    }
    if(option == "Light4")
    {
      digitalWrite(air1, message); 
      Serial.println("Turn Light4");
    }    

    if(option == "Air1")
    {
      digitalWrite(air1, message); 
      Serial.println("Turn Light");
    }
        
}

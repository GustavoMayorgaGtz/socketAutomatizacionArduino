#include <Arduino.h>
#define light1 12
#define light2 14
#define air1 27

void switchMenu(String option, bool message)
{
    if(option == "Light1")
        digitalWrite(light1, message);
        
    if(option == "Light2")
        digitalWrite(light2, message); 

    if(option == "Air1")
        digitalWrite(air1, message); 
}

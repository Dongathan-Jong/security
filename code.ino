#include <IRremote.hpp>

#include <LiquidCrystal_I2C.h>

#define pirSensor 3
#define rightButton 4
#define leftButton 5
#define led 7
#define buzzer 8
#define irRecieve 6

IRrecv irrecv(irRecieve);

boolean alarmTriggered = false;

boolean firstEntry = true;
boolean firstDigit = false;
boolean secondDigit = false;
boolean thirdDigit = false;

String currentDigit;

int password = 0;

LiquidCrystal_I2C lcd(0x20, 16, 2);

void setup()
{
  irrecv.enableIRIn();

  pinMode(pirSensor, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  lcd.init(); 
  lcd.backlight();

  Serial.begin(9600);
  
  IrReceiver.begin(irRecieve, true);
}

void loop()
{
    if(IrReceiver.decode()){
        auto value = IrReceiver.decodedIRData.decodedRawData;

        switch(value)
        {
            case 4010852096: // keypad no '1'
                digitalWrite(led, HIGH);
                Serial.println("Key 1 pressed");
                break;
            case 3994140416: // keypad no '2'
                digitalWrite(led, HIGH);
                Serial.println("Key 2 pressed");
                break;
            case 3977428736: // keypad no '3'
                digitalWrite(led, HIGH);
                Serial.println("Key 3 pressed");
                break;
            case 4278238976: // keypad no 'Power'
                digitalWrite(led, LOW);
                Serial.println("Power button pressed");
                break;
            default:
                Serial.println("Unknown key");
                break;
        }
        IrReceiver.resume();
    }

    if(!alarmTriggered)
    {
        lcd.setCursor(0, 0);
        lcd.print("System: Armed");
    }
    else
    {
        lcd.setCursor(0, 0);
        lcd.print("Alert! Triggered!");
    }


    if(digitalRead(pirSensor) == 1)
    {
        alarmTriggered = true;
    }

    alarmTrigger();

    if(firstEntry)
    {
        if(digitalRead(rightButton) == 1)
        {
            password += 1000;
            delay(500);
            currentDigit = "1";
            firstDigit = true;
            firstEntry = false;
        }
        if(digitalRead(leftButton) == 1)
        {
            delay(500);
            currentDigit = "0";
            firstDigit = true;
            firstEntry = false;
        }
    }

    if(firstDigit)
    {
        if(digitalRead(rightButton) == 1)
        {
            password += 100;
            delay(500);
            currentDigit = "1";
            firstDigit = false;
            secondDigit = true;
        }
        if(digitalRead(leftButton) == 1)
        {
            delay(500);
            currentDigit = "0";
            firstDigit = false;
            secondDigit = true;
        }
    }

}

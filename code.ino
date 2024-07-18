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
    if(IrReceiver.decode())
    {
        auto value = IrReceiver.decodedIRData.decodedRawData;

        switch(value)
        {
            case 4010852096:
                digitalWrite(led, HIGH);
                Serial.println(1);
                break;
            case 3994140416:
                digitalWrite(led, HIGH);
                Serial.println(2);
                break;
            case 3977428736:
                digitalWrite(led, HIGH);
                Serial.println(3);
                break;
            case 4278238976:
                digitalWrite(led, LOW);
                Serial.println("power");
                break;
        }
        IrReceiver.resume();
    }

    if(!alarmTriggered)
    {
        lcd.setCursor(0, 0);
        lcd.print("Alarm: Armed");
    }
    else
    {
        lcd.setCursor(0, 0);
        lcd.print("Alarm Triggered!!");
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

    if(secondDigit)
    {
        if(digitalRead(rightButton) == 1)
        {
            password += 10;
            delay(500);
            currentDigit = "1";
            secondDigit = false;
            thirdDigit = true;
        }
        if(digitalRead(leftButton) == 1)
        {
            delay(500);
            currentDigit = "0";
            secondDigit = false;
            thirdDigit = true;
        }
    }

    if(thirdDigit)
    {
        if(digitalRead(rightButton) == 1)
        {
            password += 1;
            delay(500);
            currentDigit = "1";
            thirdDigit = false;
            firstEntry = true;
        }
        if(digitalRead(leftButton) == 1)
        {
            delay(500);
            currentDigit = "0";
            thirdDigit = false;
            firstEntry = true;
        }
    }

    if(password == 1101)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alarm Disarmed!");
        delay(2000);
        lcd.clear();
        alarmTriggered = false;
        noTone(buzzer);
        password = 0;
    }
    else if(firstEntry && password > 0)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wrong Password!!");
        password = 0;
        delay(2000);
        currentDigit = " ";
    }

    if(IrReceiver.decode())
    {
        password = 0;
        firstEntry = true;
        firstDigit = false;
        secondDigit = false;
        thirdDigit = false;
        alarmTriggered = false;
        noTone(buzzer);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alarm Disarmed");
        lcd.setCursor(0, 1);
        lcd.print("By IR Remote");
        delay(1000);
        lcd.clear();
    }
}

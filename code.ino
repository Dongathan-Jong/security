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

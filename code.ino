#include <IRremote.hpp>
/*
Author: Jonathan Dong

Purpose: This program is code for a security system
         that works with a buzzer, led, 2 buttons, and PIR sensor.
         
*/

// include liquid crystal display library
#include <LiquidCrystal_I2C.h>

// define all pins 
#define pirSensor 3
#define rightButton 4
#define leftButton 5
#define led 7
#define buzzer 8
#define irRecieve 6

IRrecv irrecv(irRecieve);

// alarm triggered state
boolean alarmTriggered = false;

// digit counters 
boolean firstEntry = true;
boolean firstDigit = false;
boolean secondDigit = false;
boolean thirdDigit = false;

// current digit of the password
String currentDigit;

// whole password
int password = 0;

// state liquid crystal display
LiquidCrystal_I2C lcd(0x20, 16, 2);

/*

void setup()

Use: Setup program for use, state pinmodes for each pin and set lcd display

*/

void setup()
{
  irrecv.enableIRIn();

  // state pinmodes 
  pinMode(pirSensor, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  //initialize display and turn on backlight
  lcd.init(); 
  lcd.backlight();

  // start serial port for debug
  Serial.begin(9600);
  
  IrReceiver.begin(irRecieve, true);
}

/*

void loop()

Use: Main loop, will detect motion and hold password system

*/

void loop()
{
   if(IrReceiver.decode()){
    auto value= IrReceiver.decodedIRData.decodedRawData;
    
    switch(value)
    {
      	case 4010852096: //keypad no '1'
        digitalWrite(led,HIGH);
        Serial.println(1);
    }
    switch(value)
    {
      	case 3994140416: //keypad no '2'
        digitalWrite(led,HIGH);
      	Serial.println(2);
    }
    switch(value)
    {
      	case 3977428736: //keypad no '3'
        digitalWrite(led,HIGH);
      	Serial.println(3);
    }
    switch(value)
    {
      	case 4278238976: //keypad no 'Power'
        digitalWrite(led,LOW);
      	Serial.println("power");
    }
    IrReceiver.resume();
  }


  
  
  // the state of the lcd display when alarm is tripped or not
  if(alarmTriggered == false)
  {
    // set cursor to top left 
    lcd.setCursor(0,0);
    lcd.print("Alarm: Armed");
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Alarm Triggered!!");
  }
  
  
  // check if alarm is triggered
  if(digitalRead(pirSensor) == 1)
  {
    // set alarm state to triggered
    alarmTriggered = true;
  }
  
  // enter alarm trigger method
  alarmTrigger();
  
  // check if it is the first digit
  if(firstEntry)
  {
    // check if the right button is pressed
    if(digitalRead(rightButton) == 1)
    {
      // add 1000 to the password
      password = password + 1000;
      delay(500);
      
      // put current digit as 1
      currentDigit = "1";
      
      //move to next digit
      firstDigit = true;
      firstEntry = false;
    }
  
    // check if left button is pressed
    if(digitalRead(leftButton) == 1)
    {
      delay(500);
      
      // put current digit as 0
      currentDigit = "O";
      
      // move to next digit
      firstDigit = true;
      firstEntry = false;
    }
  }
  
  // check if first digit is done
  if(firstDigit)
  {
    // check if right button is pressed
    if(digitalRead(rightButton) == 1)
    {
      // add 100 to the password
      password = password + 100;
      delay(500);
      
      // set current digit to 1
      currentDigit = "1";
      
      // move to next digit
      firstDigit = false;
      secondDigit = true;
    }
    
    // check if left button is pressed 
    if(digitalRead(leftButton) == 1)
    {
      delay(500);
      
      // set current digit to 0
      currentDigit = "O";
      
      // move to next digit
      firstDigit = false;
      secondDigit = true;
    }
  }
  
  // check if second digit is done
  if(secondDigit)
  {
    // check if right button is pressed
    if(digitalRead(rightButton) == 1)
    {
      // add 10 to the password
      password = password + 10;
      delay(500);
      
      // set current digit to 1
      currentDigit = "1";
      
      // move to next digit
      secondDigit = false;
      thirdDigit = true;
    }
    
    // check if left button is pressed
    if(digitalRead(leftButton) == 1)
    {
      delay(500);
      
      // set current digit to 0
      currentDigit = "O";
      
      // move to next digit
      thirdDigit = true;
      secondDigit = false;
    }
  }
  
  // check if third digit is done
  if(thirdDigit)
  {
    // check if right button is pressed
    if(digitalRead(rightButton) == 1)
    {
      // add 1 to the password
      password = password + 1;
      delay(500);
      
      // set current digit to 1
      currentDigit = "1";
      
      // move to next digit
      thirdDigit = false;
      firstEntry = true;
    }
    
    // check if left button is pressed
    if(digitalRead(leftButton) == 1)
    {
      delay(500);
      
      // set current digit to 0
      currentDigit = "O";
      
      // move to check password
      thirdDigit = false;
      firstEntry = true;
    }
  }
  
  // check if password is correct
  if(password == 1101)
  {
    // clear the lcd and print message
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm Disarmed!");
    
    delay(2000);
    
    // clear lcd again
    lcd.clear();
    
    // set alarm off and turn off buzzer and reset password
    alarmTriggered = false;
    noTone(buzzer);
    password = 0;
    
  }
  else if(firstEntry == true && password > 0)
  {
    // clear the lcd and tell user wrong password is inputted 
    lcd.clear();
    lcd.setCursor(0,0);
    
    //print message and reset password
    lcd.print("Wrong Password!!");
    password = 0;
    
    delay(2000);
    
    // set the current digit as empty
    currentDigit = " ";
  }
  
  
  // check if ir siginal is recieved
  if(IrReceiver.decode())
  {
    
    // set password to 0 and all digits to default
    password = 0;
    firstEntry = true;
    firstDigit = false;
    secondDigit = false;
    thirdDigit = false;
      
    // disarm alarm and stop buzzer
    alarmTriggered = false;
    noTone(buzzer);
      
    // clear lcd and tell user message
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm Disarmed");
    lcd.setCursor(0,1);
    lcd.print("By IR Remote");
      
    delay(1000);
      
    // clear lcd again
    lcd.clear();
    
  }
  
}

/*

void alarmTrigger()

Use: Will sound alarm and led when motion is detected

*/

void alarmTrigger()
{
  // check if alarm is triggered
  if(alarmTriggered)
  {
    // buzzer and led turns on
    digitalWrite(led, HIGH);
    tone(buzzer, 440,500);
  	delay(10);
  	digitalWrite(led, LOW);
    tone(buzzer, 440);
  	delay(10);	
    
    // prompt user for the password
    lcd.setCursor(0,1);
    lcd.print("Pass: ");
    
    // check if user is on first digit and print user selection
    if(firstEntry)
    {
      lcd.setCursor(7,1);
      lcd.print(currentDigit);
    }
    // check if user is on second digit and print user selection
    if(firstDigit)
    {
      lcd.setCursor(8,1);
      lcd.print(currentDigit);
    }
    // check if user is on third digit and print user selection
    if(secondDigit)
    {
      lcd.setCursor(9,1);
      lcd.print(currentDigit);
    }
    // check if user is on fourth digit and print user selection
    if(thirdDigit)
    {
      lcd.setCursor(10,1);
      lcd.print(currentDigit);
    }
  }
}


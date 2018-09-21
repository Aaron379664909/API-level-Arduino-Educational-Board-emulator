#include "test.h"

#include <ISADefinitions.h>
#include <ISALiquidCrystal.h>
#include <isaoled.h>
#include <isa7segmentdisplay.h>
#include <isabuttons.h>
#include <isaledcontrol.h>
ISALiquidCrystal lcd;
ISAOLED oled;
ISA7SegmentDisplay sseg;

int s1=0;//single-digit digits in seconds
int s2=0;//10 digits in seconds
int m1=0;//single-digit digits in minutes
int m2=0;//10 digits in minutes
int i=0;
int lights = 0;
bool up = false;//up key state
bool down = false;//down key state
bool rightk = true;// right key state
bool leftk = true;//left key state
bool backupR = true;//backup for the right key, last time right key's value
bool backupL = true;//backup for the left key, last time left key's value
bool turnOn = false;//using up key to turn on, and down key to turn off
Test::Test(){

}

void Test::setup(){
    for(int i=0;i<8;++i){
      pinMode(LEDS[i], OUTPUT);
    }
    pinMode(KEY_UP,INPUT);
    pinMode(KEY_DOWN,INPUT);
    pinMode(KEY_LEFT, INPUT);
    pinMode(KEY_RIGHT, INPUT);


    lcd.begin();
    oled.begin();
    sseg.init();
    sseg.displayDigit(s1,0);
    sseg.displayDigit(s2,1);
    sseg.displayDigit(m1,2,true);
    sseg.displayDigit(m2,3);

    oled.print("I am here! Using print() and renderAll()");
    oled.renderAll();

}
void Test::loop(){
    rightk = digitalRead(KEY_RIGHT);
     leftk = digitalRead(KEY_LEFT);
     up = !digitalRead(KEY_UP);
     down = !digitalRead(KEY_DOWN);
     if(up){
         digitalWrite(LEDS[lights],LOW);
         digitalWrite(LED1,HIGH);
         lights=0;
         turnOn=true;
     }
     if(down){
         digitalWrite(LEDS[lights],LOW);
         lights=0;
         turnOn=false;
     }
     if(turnOn){
         //rightk key
         if(rightk != backupR){
           delay(20);
           if(rightk == digitalRead(KEY_RIGHT)){
             if(backupR == 0){
               digitalWrite(LEDS[lights], LOW);
               if(lights==7){
                 lights=0;
                 digitalWrite(LEDS[lights], HIGH);
                }
                else{
                   digitalWrite(LEDS[lights+1], HIGH);
                   ++lights;
                 }
             }
             backupR=rightk;
           }
         }
         //left key
           if(leftk != backupL){
             delay(20);
             if(leftk == digitalRead(KEY_LEFT)){
               if(backupL == 0){
                 digitalWrite(LEDS[lights], 0);
                 if(lights==0){
                   lights=7;
                   digitalWrite(LEDS[lights], 1);
                 }
                 else{
                   digitalWrite(LEDS[lights-1], 1);
                   --lights;
                 }
               }
               backupL=leftk;
             }
           }
     }

     lcd.clear();
     int potx = analogRead(JOY1X);
     int poty = analogRead(JOY1Y);
     int pot2x =analogRead(JOY2X);
     int pot2y=analogRead(JOY2Y);
     int pot =analogRead(POT);
     lcd.print("JOY1:");
     lcd.print(potx);
     lcd.print(",");
     lcd.print(poty);
     lcd.setCursor(0,1);
     lcd.print("JOY2:");
     lcd.print(pot2x);
     lcd.print(",");
     lcd.print(pot2y);

//     oled.clear();
//     oled.print("I am here! Using print() and renderAll()");
//     oled.gotoXY(4,0);
//     oled.print("Potentiometer:");
//     oled.print(pot);
//     oled.renderAll();
}

//test1 16 number buttons state
//test1();

//test2 using key-left and key-right to control LEDs turn left or right.
//test2();

//test3 display the potentiometer value on LCD
//test3();

//test4 Using the potentiometer, move the diode to the left or right
//test4();

//test5 using potentiometer control the LED's brightness.
//test5();

//test6 test unuse pinMode INPUT and use pinMode INPUT
//test6();

//test7();

//test8();

//test9();

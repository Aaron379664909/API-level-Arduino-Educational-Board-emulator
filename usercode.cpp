#include "usercode.h"

#include <ISADefinitions.h>
#include <ISALiquidCrystal.h>
#include <isaoled.h>
#include <isa7segmentdisplay.h>
#include <isabuttons.h>
#include <isaledcontrol.h>
ISALiquidCrystal lcd;
ISAOLED oled;
ISA7SegmentDisplay sseg;
ISALedControl lcdcon;

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
UserCode::UserCode(){

}

void UserCode::setup(){
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
    lcdcon.init();


    oled.print(2.315);
    oled.writeRect(90,36,70,10);
    oled.renderAll();
    lcd.print(2.315);
}
void UserCode::loop(){
     testcode();
}
void UserCode::testcode(){
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

     lcd.print("JOY1:");
     lcd.print(potx);
     lcd.print(",");
     lcd.print(poty);
     lcd.setCursor(0,1);
     lcd.print("JOY2:");
     lcd.print(pot2x);
     lcd.print(",");
     lcd.print(pot2y);

     int pot =analogRead(POT);
     oled.clear();
     oled.print("Potentiometer:");
     oled.print(pot);
     oled.setPixel(33,0,true);
     oled.setPixel(31,1,false);
     oled.setPixel(31,2,true);
     oled.writeLine(31,41,30,90);
     oled.writeRect(90,36,70,10,true);
     oled.renderAll();

    lcdcon.setColumn(3,0b11111111);
}

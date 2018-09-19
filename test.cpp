#include "test.h"
#include "mainwindow.h"
#include <ISADefinitions.h>
#include <ISALiquidCrystal.h>
ISALiquidCrystal lcd;

int lights = 0;
bool up = false;//up key state
bool down = false;//down key state
bool right = true;// right key state
bool left = true;//left key state
bool backupR = true;//backup for the right key, last time right key's value
bool backupL = true;//backup for the left key, last time left key's value
bool turnOn = false;//using up key to turn on, and down key to turn off
void Test::setup(){
    for(int i=0;i<8;++i){
      pinMode(LEDS[i], OUTPUT);
    }
    lcd.begin();
    pinMode(KEY_UP,INPUT);
    pinMode(KEY_DOWN,INPUT);
    pinMode(KEY_LEFT, INPUT);
    pinMode(KEY_RIGHT, INPUT);
}
void Test::loop(){

    for(int i=0;i<5000;++i){
        right = digitalRead(KEY_RIGHT);
           left = digitalRead(KEY_LEFT);
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
               //right key
               if(right != backupR){
                 delay(20);
                 if(right == digitalRead(KEY_RIGHT)){
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
                   backupR=right;
                 }
               }
               //left key
                 if(left != backupL){
                   delay(20);
                   if(left == digitalRead(KEY_LEFT)){
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
                     backupL=left;
                   }
                 }
           }


  delay(30);
}

}


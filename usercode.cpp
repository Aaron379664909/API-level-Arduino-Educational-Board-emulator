#include "usercode.h"

#include <ISADefinitions.h>
#include <ISALiquidCrystal.h>
#include <ISAOLED.h>

ISALiquidCrystal lcd;
ISAOLED oled;
int i=0;
UserCode::UserCode(){

}

void UserCode::setup(){
    lcd.begin();
    lcd.print("Hello");
    oled.begin();
    delay(1000);
}
void UserCode::loop(){
    lcd.clear();
    ++i;
    lcd.print(i);
    printOLED(i);
    delay(1000);
}
void UserCode::printOLED(int i){
    oled.clear();
    oled.print(i);
    oled.renderAll();
}

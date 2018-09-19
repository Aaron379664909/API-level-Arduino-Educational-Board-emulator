#ifndef TEST_H
#define TEST_H
#include "mainwindow.h"
class Test{

public:
    void setup();
    void loop();
    void delay(int x){
        MainWindow m;
        m.delay(x);
    }
    void pinMode(int i,int mode){
        MainWindow m;
        m.pinMode(i,mode);
    }

    void digitalWrite(int i, bool value){
        MainWindow m;
        m.digitalWrite(i,value);
    }

    bool digitalRead(int i){
        MainWindow m;
        m.digitalRead(i);
    }

    int  analogRead(int i){
        MainWindow m;
        m.analogRead(i);
    }

    void analogWrite(int i, int value){
        MainWindow m;
        m.analogWrite(i,value);
    }
};
#endif // TEST_H

#ifndef USERCODE_H
#define USERCODE_H
#include <QObject>

class UserCode : public QObject
{
    Q_OBJECT
public:
    UserCode();
    void setup();
    void loop();
signals:
    void delay(int ms);
    void pinMode(int i,int mode);
    void digitalWrite(int i, bool value);
    bool digitalRead(int i);
    int  analogRead(int i);
    void analogWrite(int i, int value);
public:
    void printOLED(int m);
};
#endif // USERCODE_H

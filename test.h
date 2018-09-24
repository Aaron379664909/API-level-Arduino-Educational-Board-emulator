#ifndef TEST_H
#define TEST_H
#include <QObject>

class Test : public QObject
{
    Q_OBJECT
public:
    Test();
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
    void testcode();
    void testcode1();
};
#endif // TEST_H

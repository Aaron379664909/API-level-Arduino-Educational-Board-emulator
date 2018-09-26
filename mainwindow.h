#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QBitmap>
#include <QLabel>
#include <QLCDNumber>
#include <QVector>
#include "isadefinitions.h"
#include "qbuttongroup.h"
#include "qelapsedtimer.h"
#include "qcoreapplication.h"
#include "qtablewidget.h"
#include "isabuttons.h"
#include "isaledcontrol.h"
#include "isa7segmentdisplay.h"
#include <QGroupBox>
#include "usercode.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_button_Start_clicked();

    void on_button_Stop_clicked();

private:
    Ui::MainWindow *ui;
    UserCode* my_usercode;

public:
    QButtonGroup *buttonGroup;//16 number buttons
    QButtonGroup *keyArrowGroup;//4 keyarrow buttons
    QLabel *ledLight[8];//8 LEDs
    QLabel *ledDot[8][8];//8*8 LED display

    QTableWidget *lcdTable;//LCD
    QLCDNumber *lcdNum[4];//4 digit display
    QBitmap bitmapWW;//bitmap 0 used for OLED
    QBitmap bitmapBB;//bitmap 1 used for OLED
    QLabel *oledDot[OLED_Y][OLED_X];//128*64 OLED display
    QBitmap bitmap[OLED_Y][OLED_X];//record the bitmap, used for OLED.renderALL()
    int oledNow[OLED_Y][OLED_X];//0-oled label is bitmap 0; 1-oled label is bitmap 1
    int oledLast[OLED_Y][OLED_X];//0-oled label is bitmap 0; 1-oled label is bitmap 1

    QPixmap PixLedOff;
    QPixmap PixLedOnLow;
    QPixmap PixLedOnHigh;
    QPixmap PixWhiteDot;
    QPixmap PixRedDot;

    //16 number buttons states
    bool numbuttonsPressed[16];//bool ISAButtons::buttonState
    bool numbuttonsReleased[16];//not used
    bool numbuttonsPreStates[16];//bool ISAButtons::buttonPressed(int i)
    bool numbuttonsPreLastStates[16];//bool ISAButtons::buttonPressed(int i)
    bool numbuttonsRelStates[16];//bool ISAButtons::buttonReleased(int i)
    bool numbuttonsRelLastStates[16];//bool ISAButtons::buttonReleased(int i)

    //4 keyarrow buttons states
    bool keyArrowPressed[4];//digitalRead
    bool keyArrowReleased[4];//not used
    bool keyArrowPreStates[4];//not used
    bool keyArrowPreLastStates[4];//not used
    bool keyArrowRelStates[4];//not used
    bool keyArrowRelLastStates[4];//not used

    //state of Pins to work
    bool turnOnButtons;//16 unmber button, true-pressed work
    bool turnOnKeyArrows[4];//4 key-arrow, true-pressed work
    bool turnOnLedDisplay;//8*8 led display,true-work
    bool turnOnLCD;//the LCD display work while true
    bool turnOn7Seg;//the 7 SegmentDisplay work while true


    int ledLightMode[8];//Pin mode, INPUT, OUTPUT, INPUT_PULLUP

    int joy1rx, joy1ry, joy2rx, joy2ry;//2 joysticks

    QElapsedTimer ttimer;


public slots:
    void numButtonPressed();//16 number buttons on press slot
    void numButtonReleased();//16 number buttons on release slot
    void numButtonClicked();//16 number buttons on click slot
    void keyArrowButPressed();//4 key arrow buttons on press slot
    void keyArrowButReleased();//4 key arrow buttons on release slot
    void keyArrowButClicked();//4 key arrow buttons on click slot

    /**
     * @brief delay. using Timer to implement the delay() function.
     * @param ms. milliseconds to be delayed.
     */
    void delay(int ms){
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<ms)
            QCoreApplication::processEvents();
    }
    void pinMode(int i,int mode);//i pin-number; mode INPUT, OUTPUT, INPUT_PULLUP
    void digitalWrite(int i, bool value);//i pin-number; value HIGH, LOW
    bool digitalRead(int i);//i pin-number, return HIHG, LOW
    int  analogRead(int i);//i pin-number, return 0-1023
    void analogWrite(int i, int value);//i pin-number, value from 0 to 255
public:
        void quitExe();//used in stop-close button to quit application.
        void keyPressEvent(QKeyEvent *key);//key_W,key_A,key_S,key_D,key_UP,key_DOWN,key_LEFT,key_RIGHT
        void keyReleaseEvent(QKeyEvent *key);//key_W,key_A,key_S,key_D,key_UP,key_DOWN,key_LEFT,key_RIGHT

//        void paintEvent(QPaintEvent *e);//this is another way to emulate OLED.
};

#endif // MAINWINDOW_H

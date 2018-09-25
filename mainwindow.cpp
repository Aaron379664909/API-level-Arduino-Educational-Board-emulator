#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "isaliquidcrystal.h"
#include "isaoled.h"
#include "isabuttons.h"
#include "LeftJoy.h"
#include "rightjoy.h"
#include "qstring.h"
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>
#include <QBitmap>
#include <QGridLayout>
#include <QMessageBox>
#include "qmath.h"



MainWindow *m;

bool loop;//while true emulate Arduino loop

QPoint windowPointL;
QPoint centerPointL;     //left joy

QPoint windowPointR;
QPoint centerPointR;     //right joy
double r;
double joy1r;
double joy2r;
//
QImage imgLedOff(":/img/ledoff.png");
QImage imgLedOnLow(":/img/ledonlow.png");
QImage imgLedOnHigh(":/img/ledonhigh.png");
QImage imgWhiteDot(":/img/whitedot.png");
QImage imgRedDot(":/img/reddot.png");


//LCD control======================
int lcdRow;//LCD row number
int lcdCol;//LCD column number
QString msg;

//OLED control======================
int oledRow;
int oledCol;

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m=this;
    ui->setupUi(this);

    loop=true;

    PixLedOff=QPixmap::fromImage(imgLedOff);
    PixLedOnLow=QPixmap::fromImage(imgLedOnLow);
    PixLedOnHigh=QPixmap::fromImage(imgLedOnHigh);
    PixWhiteDot=QPixmap::fromImage(imgWhiteDot);
    PixRedDot=QPixmap::fromImage(imgRedDot);

    //keep the potion of joysticks
    int lx=ui->joyLabel1->pos().rx()+ui->joyLabel1->width()/2-ui->JOY1->width()/2;
    int ly=ui->joyLabel1->pos().ry()+ui->joyLabel1->height()/2-ui->JOY1->height()/2;
    int rx=ui->joyLabel2->pos().rx()+ui->joyLabel2->width()/2-ui->JOY2->width()/2;
    int ry=ui->joyLabel2->pos().ry()+ui->joyLabel2->height()/2-ui->JOY2->height()/2;
    ui->JOY1->move(lx,ly);
    ui->JOY2->move(rx,ry);


    //8 LED Lights====================================================
    ledLight[0] = ui->pin0;
    ledLight[1] = ui->pin1;
    ledLight[2] = ui->pin2;
    ledLight[3] = ui->pin3;
    ledLight[4] = ui->pin4;
    ledLight[5] = ui->pin5;
    ledLight[6] = ui->pin6;
    ledLight[7] = ui->pin7;
    for(int i=0;i<8;++i){
        ledLightMode[i]=INPUT;
    }

    //8*8 LED display==================================================
    for(int i=1;i<9;++i)
        for(int j=1;j<9;++j){
            QString index = QString::number(j+8*(i-1));
            QString name = "led_"+index;
            ledDot[i-1][j-1]=m->findChild<QLabel *>(name);
        }
    turnOnLedDisplay=false;

    //16 buttons on numeric keypad===========================================================
    turnOnButtons=false;//16 number buttons must be using ISAButton::init() to set value true
    buttonGroup = new QButtonGroup;
    buttonGroup->addButton(ui->button_0,0);
    buttonGroup->addButton(ui->button_1,1);
    buttonGroup->addButton(ui->button_2,2);
    buttonGroup->addButton(ui->button_3,3);
    buttonGroup->addButton(ui->button_4,4);
    buttonGroup->addButton(ui->button_5,5);
    buttonGroup->addButton(ui->button_6,6);
    buttonGroup->addButton(ui->button_7,7);
    buttonGroup->addButton(ui->button_8,8);
    buttonGroup->addButton(ui->button_9,9);
    buttonGroup->addButton(ui->button_10,10);
    buttonGroup->addButton(ui->button_11,11);
    buttonGroup->addButton(ui->button_12,12);
    buttonGroup->addButton(ui->button_13,13);
    buttonGroup->addButton(ui->button_14,14);
    buttonGroup->addButton(ui->button_15,15);
    //add connect slots
    for(int i=0;i<16;++i){
        connect(buttonGroup->button(i), SIGNAL(pressed()), this, SLOT(numButtonPressed()));
        connect(buttonGroup->button(i), SIGNAL(released()), this, SLOT(numButtonReleased()));
        connect(buttonGroup->button(i), SIGNAL(clicked()), this, SLOT(numButtonClicked()));
    }
    //init mumber buttons state;
    for(int i=0;i<16;++i){
        numbuttonsPressed[i]=false;
        numbuttonsPreStates[i]=false;
        numbuttonsPreLastStates[i]=false;
        numbuttonsRelStates[i]=false;
        numbuttonsRelLastStates[i]=false;
    }

    //4 key-arrows==============================================================================
    keyArrowGroup = new QButtonGroup;
    keyArrowGroup->addButton(ui->pin20,0);
    keyArrowGroup->addButton(ui->pin22,1);
    keyArrowGroup->addButton(ui->pin21,2);
    keyArrowGroup->addButton(ui->pin23,3);
    //add connect slots
    for(int i=0;i<4;++i){
        connect(keyArrowGroup->button(i), SIGNAL(pressed()), this, SLOT(keyArrowButPressed()));
        connect(keyArrowGroup->button(i), SIGNAL(released()), this, SLOT(keyArrowButReleased()));
        connect(keyArrowGroup->button(i), SIGNAL(clicked()), this, SLOT(keyArrowButClicked()));
        //when turnOnKeyArrows value is true, digitalRead from key-arrow pins in reverse logic
        //init value is false
        turnOnKeyArrows[i]=false;
    }
    //init key-arrow buttons state
    for(int i=0;i<4;++i){
        keyArrowPressed[i]=false;
        keyArrowPreStates[i]=false;
        keyArrowPreLastStates[i]=false;
        keyArrowRelStates[i]=false;
        keyArrowRelLastStates[i]=false;
    }

    //LCD===============================================================================
    lcdTable = ui->lcdTableWidget;
    //init LCD display state
    turnOnLCD=false;
    msg="To display the LCD, you must to init using the begin() function. App will quit.";

    //7 SegmentDisplay==================================================================
    lcdNum[0]=ui->lcdNumber_0;
    lcdNum[1]=ui->lcdNumber_1;
    lcdNum[2]=ui->lcdNumber_2;
    lcdNum[3]=ui->lcdNumber_3;
    //init LCDNumber state
    turnOn7Seg=false;

    //Joystick init====================================================================
    joy1rx=512;
    joy1ry=512;
    joy2rx=512;
    joy2ry=512;
    joy1r=ui->joyLabel1->width()/2-10;
    joy2r=ui->joyLabel2->width()/2-10;

    //65*128 OLED init================================================================
    //QGroupBox* gBox = new QGroupBox(this);
    //gBox->setGeometry(30,0,270,150);

    QBitmap bitmapW(2,2);
    QBitmap bitmapB(2,2);

    QPainter painter(&bitmapB);
    QPainter painterw(&bitmapW);

    painter.fillRect(0,0,2,2,Qt::black);
    painterw.fillRect(0,0,2,2,Qt::white);


    bitmapWW=bitmapW;
    bitmapBB=bitmapB;
    QGridLayout *pLayout = new QGridLayout();
    for(int i=0;i<OLED_Y;++i){
        for(int j=0;j<OLED_X;++j){
            QLabel *label = new QLabel(this);
            label->setFixedSize(2,2);
            label->setPixmap(bitmapBB);
            oledDot[i][j]=label;
            bitmap[i][j]=bitmapBB;
            oledNow[i][j]=1;
            oledLast[i][j]=1;
            pLayout->addWidget(label,i,j);
        }
    }
    pLayout->setSpacing(0);
    //gBox->setLayout(pLayout);
    //gBox->show();
    ui->groupBoxOLED->setLayout(pLayout);


    //init setup
    my_usercode=new UserCode();
    connect(my_usercode,SIGNAL(delay(int)),this,SLOT(delay(int)));
    connect(my_usercode,SIGNAL(pinMode(int,int)),this,SLOT(pinMode(int,int)));
    connect(my_usercode,SIGNAL(digitalWrite(int,bool)),this,SLOT(digitalWrite(int,bool)));
    connect(my_usercode,SIGNAL(digitalRead(int)),this,SLOT(digitalRead(int)));
    connect(my_usercode,SIGNAL(analogRead(int)),this,SLOT(analogRead(int)));
    connect(my_usercode,SIGNAL(analogWrite(int,int)),this,SLOT(analogWrite(int,int)));

//    my_usercode->setup();

}

MainWindow::~MainWindow()
{

    delete ui;
}

//Below are functions of isaliquidcrystal.h==============================================
//ISALiquidCrystal.h
/**
 * @brief ISALiquidCrystal::begin
 * set the QTableWidget item(0,0)
 * Be notice when programing with Arduino Educational board and not called this function,
 * the LCD will work only in the first row with black backgrand. and the function setCursor()
 * will work only for the first param Col, the row will be const 0.
 * So, in this program, this function must be called, while not, tell user to call it.(Using QMessageBox).
 */
void ISALiquidCrystal::begin(){
    lcdRow = 0;
    lcdCol = 0;
    m->turnOnLCD=true;
}
/**
 * @brief ISALiquidCrystal::print
 * @param data
 * String data
 */
void ISALiquidCrystal::print(QString data){
    if(!m->turnOnLCD){
        QMessageBox::critical(0 ,
        "critical message" , msg,
        QMessageBox::Ok , 	0 );
        m->quitExe();
        return;
    }
    for(int i=0;i<data.length();++i){
        m->lcdTable->setItem(lcdRow,lcdCol,new QTableWidgetItem(data.at(i)));
        ++lcdCol;
    }
}
/**
 * @brief ISALiquidCrystal::print
 * @param data
 * char data
 */
void ISALiquidCrystal::print(char data){
    if(!m->turnOnLCD){
        QMessageBox::critical(0 ,
        "critical message" , msg,
        QMessageBox::Ok , 	0 );
        m->quitExe();
    }
    m->lcdTable->setItem(lcdRow,lcdCol,new QTableWidgetItem(QString(data)));
    ++lcdCol;
}
/**
 * @brief ISALiquidCrystal::print
 * @param data
 * int data
 */
void ISALiquidCrystal::print(int data){
    if(!m->turnOnLCD){
        QMessageBox::critical(0 ,
        "critical message" , msg,
        QMessageBox::Ok , 	0 );
        m->quitExe();
        return;
    }
    QString str=QString::number(data);
    print(str);
}
void ISALiquidCrystal::print(double data){
    if(!m->turnOnLCD){
        QMessageBox::critical(0 ,
        "critical message" , msg,
        QMessageBox::Ok , 	0 );
        m->quitExe();
        return;
    }
    QString str=QString::number(data,'f',2);
    print(str);
}
/**
 * @brief ISALiquidCrystal::clear. clear all the contents in the lcdQtableWidget
 * and set row and col 0
 */
void ISALiquidCrystal::clear(){
    if(!m->turnOnLCD){
        QMessageBox::critical(0 ,
        "critical message" , msg,
        QMessageBox::Ok , 	0 );
        m->quitExe();
        return;
    }
    m->lcdTable->clearContents();
    lcdRow = 0;
    lcdCol = 0;
}
/**
 * @brief ISALiquidCrystal::setCursor. sets cursor in a given position
 * @param col - the column at which to position the cursor
 * @param row - the row at which to position the cursor
 * Rows and columns are numbered from 0
 */
void ISALiquidCrystal::setCursor(int col,int row){
    if(!m->turnOnLCD){
        QMessageBox::critical(0 ,
        "critical message" , msg,
        QMessageBox::Ok , 	0 );
        m->quitExe();
        return;
    }
    lcdRow = row;
    lcdCol = col;
}
//End functions of isaliquidcrystal.h====================================================

//Below are functions of  .h=================================================
//ISALedControl.h
/**
 * @brief ISALedControl::init. turn on, set value true.
 */
void ISALedControl::init(){
    m->turnOnLedDisplay=true;
}

/**
 * @brief ISALedControl::clearDisplay
 * turn off all the leds using set pixmap with whitedot.png
 */
void ISALedControl::clearDisplay(){
    for(int i=0;i<8;++i)
        for(int j=0;j<8;++j){
            m->ledDot[i][j]->setPixmap(m->PixWhiteDot);
        }
}

/**
 * @brief ISALedControl::setColumn
 * @param col
 * @param value
 */
void ISALedControl::setColumn(int col, byte value){
    if(m->turnOnLedDisplay){
        if((0<=col)&(col<8)){
            for(int row=0;row<8;++row){
                if(value&(1<<row)){
                    m->ledDot[row][col]->setPixmap(m->PixRedDot);
                }
                else{
                    m->ledDot[row][col]->setPixmap(m->PixWhiteDot);
                }
            }
        }
    }
}

/**
 * @brief ISALedControl::setRow
 * @param row
 * @param value
 */
void ISALedControl::setRow(int row, byte value){
    if(m->turnOnLedDisplay){
        if((0<=row)&(row<8)){
            for(int col=0;col<8;++col){
                if(value&(1<<col)){
                    m->ledDot[row][col]->setPixmap(m->PixRedDot);
                }
                else{
                    m->ledDot[row][col]->setPixmap(m->PixWhiteDot);
                }
            }
        }
    }
}

/**
 * @brief ISALedControl::setLed
 * @param row
 * @param col
 * @param value
 */
void ISALedControl::setLed(int row, int col, bool value){
    if(m->turnOnLedDisplay){
        if((0<=row)&(row<8)&(0<=col)&(col<8)){
            if(value){
                m->ledDot[row][col]->setPixmap(m->PixRedDot);
            }
            else{
                m->ledDot[row][col]->setPixmap(m->PixWhiteDot);
            }
        }
    }
}

//End functions of ISALedControl.h=======================================================

//Below are functions of ISAButtons.h=================================================
//ISAButtons.h
/**
 * @brief ISAButtons::init
 */
void ISAButtons::init(){
    m->turnOnButtons=true;
}
/**
 * @brief ISAButtons::buttonPressed
 * @param i. pin number
 * @return true, when buttons state change from released to pressed. otherwise return false.
 */
bool ISAButtons::buttonPressed(int i){
    if(i>=numberOfButtons)
        return false;
    if(m->numbuttonsPreLastStates[i]!=m->numbuttonsPreStates[i]){
        m->numbuttonsPreLastStates[i]=m->numbuttonsPreStates[i];
        return true;
    }
    else
        return false;
}
/**
 * @brief ISAButtons::buttonReleased
 * @param i. pin number
 * @returntrue, when buttons state change from pressed to released. otherwise return false.
 */
bool ISAButtons::buttonReleased(int i){
    if(i>=numberOfButtons)
        return false;
    if(m->numbuttonsRelLastStates[i]!=m->numbuttonsRelStates[i]){
        m->numbuttonsRelLastStates[i]=m->numbuttonsRelStates[i];
        return true;
    }
    else
        return false;
}
/**
 * @brief ISAButtons::buttonState
 * @param i. pin number
 * @return true,when buttons pressed; otherwise return false.
 */
bool ISAButtons::buttonState(int i){
    if(i>=numberOfButtons)
        return false;
    return m->numbuttonsPressed[i];
}
//End functions of ISAButtons.h=====================================================


//Begin functions of LeftJoy.h=====================================================
LeftJoy::LeftJoy(QWidget *parent):QLabel(parent)
{
}

void LeftJoy::mousePressEvent(QMouseEvent*e)
{
    int x=this->pos().rx()+this->width()/2;
    int y=this->pos().ry()+this->height()/2;
    centerPointL.setX(x);   //Circle center rx
    centerPointL.setY(y);   //Circle center ry
    //r=60;
    r=joy1r;
    windowPointL = e->globalPos() - this->pos();
}
void LeftJoy::mouseReleaseEvent(QMouseEvent *e){

    QPoint pointr=m->findChild<QLabel *>("joyLabel1")->pos();
    int x=pointr.rx()+m->findChild<QLabel *>("joyLabel1")->width()/2-this->width()/2;
    int y=pointr.ry()+m->findChild<QLabel *>("joyLabel1")->height()/2-this->height()/2;
    QPoint bpoint(x,y);
    move(bpoint);
    m->joy1rx=512;
    m->joy1ry=512;

}

void LeftJoy::mouseMoveEvent(QMouseEvent*e)
{
    int redR=this->width()/2;
     QPoint bpoint=e->globalPos()-windowPointL;
         if(sqrt(pow(abs(bpoint.rx()-centerPointL.rx()+redR),2) + pow(abs(bpoint.ry()-centerPointL.ry()+redR),2)) <=r)
         {
             move(bpoint);
             m->joy1rx=1023*(centerPointL.rx()+r-bpoint.rx()-redR)/(2*r);
             m->joy1ry=1023*(centerPointL.ry()+r-bpoint.ry()-redR)/(2*r);

         }else
         {
             if((bpoint.rx()+redR)>centerPointL.rx() && (bpoint.ry()+redR)<centerPointL.ry())
             {
                 double k = (double)(bpoint.rx()+redR-centerPointL.rx())/(centerPointL.ry()+redR-bpoint.ry());
                 double angle= qAtan(k)/3.1415*180;
                 bpoint.setX(centerPointL.rx() + r*qCos((angle-90.0)*3.1415/180)-redR);
                 bpoint.setY(centerPointL.ry() + r*qSin((angle-90.0)*3.1415/180)-redR);
                 move(bpoint);

                 m->joy1rx=1023*(centerPointL.rx()+r-bpoint.rx()-redR)/(2*r);
                 m->joy1ry=1023*(centerPointL.ry()+r-bpoint.ry()-redR)/(2*r);

             }

             else if((bpoint.rx()+redR)>centerPointL.rx() && (bpoint.ry()+redR)>centerPointL.ry())
             {
                 double k = (double)(bpoint.rx()+redR-centerPointL.rx())/(bpoint.ry()+redR-centerPointL.ry());
                 double angle= 180-qAtan(k)/3.1415*180;
                 bpoint.setX(centerPointL.rx() + r*qCos((angle-90.0)*3.1415/180)-redR);
                 bpoint.setY(centerPointL.ry() + r*qSin((angle-90.0)*3.1415/180)-redR);
                 move(bpoint);

                 m->joy1rx=1023*(centerPointL.rx()+r-bpoint.rx()-redR)/(2*r);
                 m->joy1ry=1023*(centerPointL.ry()+r-bpoint.ry()-redR)/(2*r);

             }
             else if((bpoint.rx()+redR)<centerPointL.rx() && (bpoint.ry()+redR)>centerPointL.ry())
             {
                 double k = (double)(centerPointL.rx()-bpoint.rx()-redR)/(bpoint.ry()+redR-centerPointL.ry());
                 double angle= qAtan(k)/3.1415*180;
                 bpoint.setX(centerPointL.rx() - r*qCos((angle-90.0)*3.1415/180)-redR);
                 bpoint.setY(centerPointL.ry() - r*qSin((angle-90.0)*3.1415/180)-redR);
                 move(bpoint);

                 m->joy1rx=1023*(centerPointL.rx()+r-bpoint.rx()-redR)/(2*r);
                 m->joy1ry=1023*(centerPointL.ry()+r-bpoint.ry()-redR)/(2*r);

             }
             else if((bpoint.rx()+redR)<centerPointL.rx() && (bpoint.ry()+redR)<centerPointL.ry())
             {
                 double k = (double)(centerPointL.rx()-bpoint.rx()-redR)/(centerPointL.ry()-bpoint.ry()-redR);
                 double angle= 180-qAtan(k)/3.1415*180;
                 bpoint.setX(centerPointL.rx() - r*qCos((angle-90.0)*3.1415/180)-redR);
                 bpoint.setY(centerPointL.ry() - r*qSin((angle-90.0)*3.1415/180)-redR);
                 move(bpoint);

                 m->joy1rx=1023*(centerPointL.rx()+r-bpoint.rx()-redR)/(2*r);
                 m->joy1ry=1023*(centerPointL.ry()+r-bpoint.ry()-redR)/(2*r);

             }
         }
}

//End functions of LeftJoy.h=====================================================


//Begin functions of RihgtJoy.h=====================================================
RightJoy::RightJoy(QWidget *parent):QLabel(parent)
{
}

void RightJoy::mousePressEvent(QMouseEvent*e)
{
    int x=this->pos().rx()+this->width()/2;
    int y=this->pos().ry()+this->height()/2;
    centerPointR.setX(x);   //Circle center rx
    centerPointR.setY(y);   //Circle center ry
    //r=60;
    r=joy2r;
    windowPointR = e->globalPos() - this->pos();
}
void RightJoy::mouseReleaseEvent(QMouseEvent *e){
    QPoint pointr=m->findChild<QLabel *>("joyLabel2")->pos();
    int x=pointr.rx()+m->findChild<QLabel *>("joyLabel2")->width()/2-this->width()/2;
    int y=pointr.ry()+m->findChild<QLabel *>("joyLabel2")->height()/2-this->height()/2;
    QPoint bpoint(x,y);

    move(bpoint);
    m->joy2rx=512;
    m->joy2ry=512;

}

void RightJoy::mouseMoveEvent(QMouseEvent*e)
{
    int redR=this->width()/2;
     QPoint bpoint=e->globalPos()-windowPointR;
     if(sqrt(pow(abs(bpoint.rx()-centerPointR.rx()+redR),2) + pow(abs(bpoint.ry()-centerPointR.ry()+redR),2)) <=r)
     {
         move(bpoint);
         m->joy2rx=1023*(bpoint.rx()-centerPointR.rx()+r+redR)/(2*r);
         m->joy2ry=1023*(centerPointR.ry()+r-bpoint.ry()-redR)/(2*r);

     }else
     {
         if((bpoint.rx()+redR)>centerPointR.rx() && (bpoint.ry()+redR)<centerPointR.ry())
         {
             double k = (double)(bpoint.rx()+redR-centerPointR.rx())/(centerPointR.ry()-bpoint.ry()-redR);
             double angle= qAtan(k)/3.1415*180;
             bpoint.setX(centerPointR.rx() + r*qCos((angle-90.0)*3.1415/180)-redR);
             bpoint.setY(centerPointR.ry() + r*qSin((angle-90.0)*3.1415/180)-redR);
             move(bpoint);

             m->joy2rx=1023*(bpoint.rx()-centerPointR.rx()+r+redR)/(2*r);
             m->joy2ry=1023*(centerPointR.ry()+r-bpoint.ry()-redR)/(2*r);

         }

         else if((bpoint.rx()+redR)>centerPointR.rx() && (bpoint.ry()+redR)>centerPointR.ry())
         {
             double k = (double)(bpoint.rx()+redR-centerPointR.rx())/(bpoint.ry()+redR-centerPointR.ry());
             double angle= 180-qAtan(k)/3.1415*180;
             bpoint.setX(centerPointR.rx() + r*qCos((angle-90.0)*3.1415/180)-redR);
             bpoint.setY(centerPointR.ry() + r*qSin((angle-90.0)*3.1415/180)-redR);
             move(bpoint);

             m->joy2rx=1023*(bpoint.rx()-centerPointR.rx()+r+redR)/(2*r);
             m->joy2ry=1023*(centerPointR.ry()+r-bpoint.ry()-redR)/(2*r);

         }
         else if((bpoint.rx()+redR)<centerPointR.rx() && (bpoint.ry()+redR)>centerPointR.ry())
         {
             double k = (double)(centerPointR.rx()-bpoint.rx()-redR)/(bpoint.ry()+redR-centerPointR.ry());
             double angle= qAtan(k)/3.1415*180;
             bpoint.setX(centerPointR.rx() - r*qCos((angle-90.0)*3.1415/180)-redR);
             bpoint.setY(centerPointR.ry() - r*qSin((angle-90.0)*3.1415/180)-redR);
             move(bpoint);

             m->joy2rx=1023*(bpoint.rx()-centerPointR.rx()+r+redR)/(2*r);
             m->joy2ry=1023*(centerPointR.ry()+r-bpoint.ry()-redR)/(2*r);

         }
         else if((bpoint.rx()+redR)<centerPointR.rx() && (bpoint.ry()+redR)<centerPointR.ry())
         {
             double k = (double)(centerPointR.rx()-bpoint.rx()-redR)/(centerPointR.ry()-bpoint.ry()-redR);
             double angle= 180-qAtan(k)/3.1415*180;
             bpoint.setX(centerPointR.rx() - r*qCos((angle-90.0)*3.1415/180)-redR);
             bpoint.setY(centerPointR.ry() - r*qSin((angle-90.0)*3.1415/180)-redR);
             move(bpoint);

             m->joy2rx=1023*(bpoint.rx()-centerPointR.rx()+r+redR)/(2*r);
             m->joy2ry=1023*(centerPointR.ry()+r-bpoint.ry()-redR)/(2*r);

         }
     }
}
//End functions of RihgtJoy.h=====================================================


//Begin of functions of ISA7SegmentDisplay.h=====================================================
void ISA7SegmentDisplay::init(){
    m->turnOn7Seg=true;
}

void ISA7SegmentDisplay::displayDigit(byte digit, int dispID, bool dot){
    if (digit > 9 || dispID > 3)
      return;
    if(m->turnOn7Seg){
        if(dot){
            QString str=QString::number(digit)+".";
            m->lcdNum[dispID]->display(str);
        }
        else{
            QString str=QString::number(digit)+" ";
            m->lcdNum[dispID]->display(str);
        }
    }
}

void ISA7SegmentDisplay::setLed(byte values, int dispID){
    //do nothing
}

//End of functions of ISA7SegmentDisplay.h=====================================================


//Below are functions of MainWindow==================================================
//mainwindow.h
void MainWindow::quitExe(){
    exit(0);
}

/**
 * @brief MainWindow::digitalRead
 * @param i. pin number
 * @return false, when key-arrow buttons pressed; otherwise return true.
 */
bool MainWindow::digitalRead(int i){
    if((i>=20) & (i<=23)){
        int j=i-20;
        if(turnOnKeyArrows[j]){
            return !keyArrowPressed[j];
        }
        else{
            return keyArrowPressed[j];
        }

    }
    else
        return true;
}
/**
 * @brief MainWindow::digitalWrite
 * @param i
 * @param value  HIGH, LOW
 */
void MainWindow::digitalWrite(int i, bool value){
    if((i>=0) & (i<8)){//8 LEDs
        if(value==HIGH){// turn on leds
            if(ledLightMode[i]==OUTPUT){//OUTPUT Mode turn on
                //ledLight[i]->setPixmap(QPixmap::fromImage(imgLedOnHigh));
                ledLight[i]->setPixmap(PixLedOnHigh);
            }
            else{
                ledLight[i]->setPixmap(PixLedOnLow);
            }
        }
        else if(value==LOW){// turn off leds
            ledLight[i]->setPixmap(PixLedOff);
        }
    }
    if(i==13){
        if(value==HIGH){
            ui->pin13->setPixmap(PixLedOnHigh);
        }else{
            ui->pin13->setPixmap(PixLedOff);
        }

    }
}

/**
 * @brief MainWindow::pinMode
 * @param i. pin number
 * @param mode. OUTPUT=1, INPUT=0, INPUT_PULLUP=2
 */
void MainWindow::pinMode(int i, int mode){
    if((i>=0) & (i<8)){
            ledLightMode[i]=mode;
    }
    if((i>=20) & (i<=23)){
        int j=i-20;
        //key-arrow, if not INPUT mode, it works only once when using digitalRead and value is not in reverse logic
        if(mode==INPUT){//INPUT mode,when using digitalRead the value will be in reverse logic
            turnOnKeyArrows[j]=true;
        }
        else{
             turnOnKeyArrows[j]=false;
        }
    }
}

/**
 * @brief MainWindow::analogRead
 * @param i. pin number
 * @return value from 0 to 1023
 */
int MainWindow::analogRead(int i){
    if(i==30){// POT, Potentiometer
        return m->ui->pin30->value();
    }
    if(i==40){
        return m->joy1rx;
    }
    if(i==41){
        return m->joy1ry;
    }
    if(i==42){
        return m->joy2rx;
    }
    if(i==43){
        return m->joy2ry;
    }
    return 0;
}

/**
 * @brief MainWindow::analogWrite
 * @param i. pin number
 * @param value.  from 0 to 255
 */
void MainWindow::analogWrite(int i, int value){
    if((i>=0) & (i<8)){
        if(value<=0){
            digitalWrite(i,LOW);
        }
        if(value>=255){
            pinMode(i,OUTPUT);
            digitalWrite(i,HIGH);
        }
        if((0<value) & (value<128)){
            pinMode(i,INPUT);
            digitalWrite(i,HIGH);
        }
        if((127<value) & (value<255)){
            pinMode(i,OUTPUT);
            digitalWrite(i,HIGH);
        }
    }
}

//16 number buttons events====================================
/**
 * @brief MainWindow::numButtonPressed
 *
 */
void MainWindow::numButtonPressed(){
    if(turnOnButtons){
        QPushButton* btn = qobject_cast<QPushButton*>(sender());
        QString str = btn->objectName().mid(7);
        int index = str.toInt();
        numbuttonsPressed[index]=true;
        numbuttonsReleased[index]=false;
        numbuttonsPreStates[index]=!numbuttonsPreStates[index];
    }
}
/**
 * @brief MainWindow::numButtonReleased
 */
void MainWindow::numButtonReleased(){
    if(turnOnButtons){
        QPushButton* btn = qobject_cast<QPushButton*>(sender());
        QString str = btn->objectName().mid(7);
        int index = str.toInt();
        numbuttonsPressed[index]=false;
        numbuttonsReleased[index]=true;
        numbuttonsRelStates[index]=!numbuttonsRelStates[index];
    }
}

/**
 * @brief MainWindow::numButtonClicked
 * do nothing
 */
void MainWindow::numButtonClicked(){
    if(turnOnButtons){

    }
}
//end 16 number buttons events==================================

//4 key-arrow buttons events====================================
/**
 * @brief MainWindow::keyArrowPressed
 */
void MainWindow::keyArrowButPressed(){
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if("pin20"==btn->objectName()){
        if(turnOnKeyArrows[0]){//INPUT state works
            keyArrowPressed[0]=true;
            keyArrowReleased[0]=false;
            keyArrowPreStates[0]=!keyArrowPreStates[0];
        }
    }
    if("pin21"==btn->objectName()){
        if(turnOnKeyArrows[1]){//INPUT state works
            keyArrowPressed[1]=true;
            keyArrowReleased[1]=false;
            keyArrowPreStates[1]=!keyArrowPreStates[1];
        }
    }
    if("pin22"==btn->objectName()){
        if(turnOnKeyArrows[2]){//INPUT state works
            keyArrowPressed[2]=true;
            keyArrowReleased[2]=false;
            keyArrowPreStates[2]=!keyArrowPreStates[2];
        }
    }
    if("pin23"==btn->objectName()){
        if(turnOnKeyArrows[3]){//INPUT state works
            keyArrowPressed[3]=true;
            keyArrowReleased[3]=false;
            keyArrowPreStates[3]=!keyArrowPreStates[3];
        }
    }
}
/**
 * @brief MainWindow::keyArrowReleased
 */
void MainWindow::keyArrowButReleased(){
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if("pin20"==btn->objectName()){
        if(turnOnKeyArrows[0]){//INPUT state works
            keyArrowPressed[0]=false;
            keyArrowReleased[0]=true;
            keyArrowRelStates[0]=!keyArrowRelStates[0];
        }

    }
    if("pin21"==btn->objectName()){
        if(turnOnKeyArrows[1]){//INPUT state works
            keyArrowPressed[1]=false;
            keyArrowReleased[1]=true;
            keyArrowRelStates[1]=!keyArrowRelStates[1];
        }

    }
    if("pin22"==btn->objectName()){
        if(turnOnKeyArrows[2]){//INPUT state works
            keyArrowPressed[2]=false;
            keyArrowReleased[2]=true;
            keyArrowRelStates[2]=!keyArrowRelStates[2];
        }

    }
    if("pin23"==btn->objectName()){
        if(turnOnKeyArrows[3]){//INPUT state works
            keyArrowPressed[3]=false;
            keyArrowReleased[3]=true;
            keyArrowRelStates[3]=!keyArrowRelStates[3];
        }

    }
}
/**
 * @brief MainWindow::keyArrowClicked
 * do nothing
 */
void MainWindow::keyArrowButClicked(){

}
/**
 * @brief MainWindow::keyPressEvent
 * @param key
 */
void MainWindow::keyPressEvent(QKeyEvent *key){
    switch(key->key()){
    case Qt::Key_Up:emit ui->pin20->pressed();break;
    case Qt::Key_Down:emit ui->pin21->pressed();break;
    case Qt::Key_Left:emit ui->pin22->pressed();break;
    case Qt::Key_Right:emit ui->pin23->pressed();break;
    case Qt::Key_W:emit ui->pin20->pressed();break;
    case Qt::Key_S:emit ui->pin21->pressed();break;
    case Qt::Key_A:emit ui->pin22->pressed();break;
    case Qt::Key_D:emit ui->pin23->pressed();break;

    default: break;
    }
}
/**
 * @brief MainWindow::keyReleaseEvent
 * @param key
 */
void MainWindow::keyReleaseEvent(QKeyEvent *key){
    switch(key->key()){
    case Qt::Key_Up:emit ui->pin20->released();break;
    case Qt::Key_Down:emit ui->pin21->released();break;
    case Qt::Key_Left:emit ui->pin22->released();break;
    case Qt::Key_Right:emit ui->pin23->released();break;
    case Qt::Key_W:emit ui->pin20->released();break;
    case Qt::Key_S:emit ui->pin21->released();break;
    case Qt::Key_A:emit ui->pin22->released();break;
    case Qt::Key_D:emit ui->pin23->released();break;

    default: break;
    }
}

//end 4 key-arrow buttons events====================================


// Below are test cases==========================================
/**
 * @brief MainWindow::test1.
 * 16 number buttons state
 */
//void MainWindow::test1(){
//    digitalWrite(LED1, !button.buttonState(0));

//    if (button.buttonReleased(1)) {
//    state = !state;
//    digitalWrite(LED2, state);
//    }
//    if(button.buttonPressed(2)){
//    state1 = !state1;
//    digitalWrite(LED3, state1);
//    }
//    delay(30);
//}
///**
// * @brief MainWindow::test2
// * using key-left and key-right to control LEDs turn left or right.
// */
//void MainWindow::test2(){
//    rightKey = digitalRead(KEY_RIGHT);
//    leftKey = digitalRead(KEY_LEFT);
//    up = !digitalRead(KEY_UP);
//    down = !digitalRead(KEY_DOWN);
//    if(up){
//        digitalWrite(LEDS[lights],LOW);
//        digitalWrite(LED1,HIGH);
//        lights=0;
//        turnOn=true;
//    }
//    if(down){
//        digitalWrite(LEDS[lights],LOW);
//        lights=0;
//        turnOn=false;
//    }
//    if(turnOn){
//        //right key
//        if(rightKey != backupR){
//          delay(20);
//          if(rightKey == digitalRead(KEY_RIGHT)){
//            if(backupR == 0){
//              digitalWrite(LEDS[lights], LOW);
//              if(lights==7){
//                lights=0;
//                digitalWrite(LEDS[lights], HIGH);
//               }
//               else{
//                  digitalWrite(LEDS[lights+1], HIGH);
//                  ++lights;
//                }
//            }
//            backupR=rightKey;
//          }
//        }
//        //left key
//          if(leftKey != backupL){
//            delay(20);
//            if(leftKey == digitalRead(KEY_LEFT)){
//              if(backupL == 0){
//                digitalWrite(LEDS[lights], 0);
//                if(lights==0){
//                  lights=7;
//                  digitalWrite(LEDS[lights], 1);
//                }
//                else{
//                  digitalWrite(LEDS[lights-1], 1);
//                  --lights;
//                }
//              }
//              backupL=leftKey;
//            }
//          }
//    }
//}
///**
// * @brief MainWindow::test3
// * display the potentiometer value on LCD
// */
//void MainWindow::test3(){
//    lcd.clear();
//    int pot = analogRead(POT);
//    lcd.print(pot);
//    delay(250);
//}

///**
// * @brief MainWindow::test4
// * Using the potentiometer, move the diode to the left or right
// */
//void MainWindow::test4(){
//    int pot = analogRead(POT);  light = pot/128;
//    digitalWrite(LEDS[light], HIGH);
//    if(light != lastLight){
//        digitalWrite(LEDS[lastLight], LOW);
//        lastLight = light;
//    }
//}

///**
// * @brief MainWindow::test5
// * using potentiometer control the LED's brightness.
// */
//void MainWindow::test5(){
//    lcd.clear();
//    int pot = analogRead(POT);

//    int i = pot/4;
//    lcd.print(pot);
//    analogWrite(LED7,i);
//    analogWrite(LED6,1000);
//    delay(250);
//}

//void MainWindow::test6(){
//    //
//    bool up =digitalRead(KEY_UP);
//    digitalWrite(LED7,up);
//    bool down = !digitalRead(KEY_DOWN);
//    digitalWrite(LED8,down);
//    digitalWrite(LED6,true);
//    digitalWrite(LED5,false);
//}

//void MainWindow::test7(){
//    //
//    for(int i=0;i<8;++i){
//      for(int j=0;j<8;++j){
//        if(leftKey){
//          led.setLed(i,j,true);
//          delay(500);
//        }
//        else{
//          led.setLed(i,7-j,true);
//          delay(500);
//        }
//      }
//      leftKey=!leftKey;
//    }
//    led.clearDisplay();
//    delay(500);
//}
//void MainWindow::test8(){
//    lcd.clear();
//    int potx = analogRead(JOY2X);
//    int poty = analogRead(JOY2Y);
//    lcd.print(potx);
//    lcd.print(":");
//    lcd.print(poty);
//    delay(250);
//}
//void MainWindow::test9(){
//    delay(1000);
//    ++s1;
//    if(s1==10){
//      s1=0;
//      ++s2;
//      if(s2==6){
//        s2=0;
//        ++m1;
//        if(m1==10){
//          m1=0;
//          ++m2;
//        }
//      }
//    }
//    sseg.displayDigit(s1,0);
//    sseg.displayDigit(s2,1);
//    sseg.displayDigit(m1,2,true);
//    sseg.displayDigit(m2,3);
//}

//Begin function of ISAOLED.h=======================================
/**
 * @brief ISAOLED::begin
 */
void ISAOLED::begin(){
    oledCol=0;
    oledRow=0;
}

/**
 * @brief ISAOLED::write
 * @param data
 * Be noticed the Arduino educational board OLED will over-write from (0,0),
 * when ptrint full screen at(63,127).
 * This code dos not allow over screen.
 */
void ISAOLED::write(byte data){

    //ASCII[data - 0x20]
    for(int j=0;j<5;++j){
        for(int i=0;i<8;++i){
            if(ASCII[data - 0x20][j]&(1<<i)){
                if((oledCol+j)>=OLED_X){
                    oledRow +=8;
                    oledCol =-j;
                    m->bitmap[oledRow+i+1][0]=m->bitmapWW;
                    m->oledNow[oledRow+i+1][0]=0;
                }else if((oledRow+i+1)<OLED_Y){
                    m->bitmap[oledRow+i+1][oledCol+j]=m->bitmapWW;
                    m->oledNow[oledRow+i+1][oledCol+j]=0;
                }else{
                    return;
                }
            }
        }

    }
    oledCol +=6;
}
/**
 * @brief ISAOLED::print
 * @param text
 */
void ISAOLED::print(QString text){
    for(int i=0;i<text.length();++i){
        byte temp=(uint8_t)text.at(i).toLatin1();
        write(temp);
    }
}
void ISAOLED::print(int x){
    QString str=QString::number(x,10);
    print(str);
}
void ISAOLED::print(double x){
    QString str=QString::number(x,'f',2);
    print(str);
}
/**
 * @brief ISAOLED::clear
 * @param render
 */
void ISAOLED::clear(bool render){
    for(int i=0;i<OLED_Y;++i){
        for(int j=0;j<OLED_X;++j){
            m->bitmap[i][j]=m->bitmapBB;
            m->oledNow[i][j]=1;
        }
    }
    gotoXY(0,0);
}
/**
 * @brief ISAOLED::gotoXY
 * @param cx
 * @param cy
 */
void ISAOLED::gotoXY(int cx, int cy){
    if(cx<0 || cx>=OLED_X || cy<0 ||cy>=OLED_Y)
        return;
    oledRow=cy*8;
    oledCol=cx;
}
/**
 * @brief ISAOLED::renderAll
 */
void ISAOLED::renderAll(){
    while(m->ttimer.elapsed()<100){
        QCoreApplication::processEvents();
        return;
    }
    if(m->ttimer.elapsed()>100){
        for(int x=0;x<OLED_Y;++x){
            for(int y=0;y<OLED_X;++y){
                if(m->oledNow[x][y]!=m->oledLast[x][y]){
                    m->oledDot[x][y]->setPixmap(m->bitmap[x][y]);
                    m->oledLast[x][y]=m->oledNow[x][y];
                }

            }
        }
       m->ttimer.restart();
       gotoXY(0,0);
   }

}
/**
 * @brief ISAOLED::setPixel
 * @param x
 * @param y
 * @param v
 */
void ISAOLED::setPixel(int x, int y, bool v){
    if(v){
        //m->oledDot[x][y]->setPixmap(m->bitmapWW);
        m->bitmap[y][x]=m->bitmapWW;
        m->oledNow[y][x]=0;
    }else{
        //m->oledDot[x][y]->setPixmap(m->bitmapBB);
        m->bitmap[y][x]=m->bitmapBB;
        m->oledNow[y][x]=1;
    }
}
/**
 * @brief ISAOLED::writeLine
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void ISAOLED::writeLine(int x1, int y1, int x2, int y2){
    if (x1 == x2 || y1 == y2){
        if (y1 > y2){
            int temp =y2;
            y2=y1;
            y1=temp;
        }
        if (x1 > x2){
            int temp =x2;
            x2=x1;
            x1=temp;
        }
        writeRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1,true);
    }else
        return;
}
/**
 * @brief ISAOLED::writeRect
 * @param x
 * @param y
 * @param w
 * @param h
 * @param fill
 */
void ISAOLED::writeRect(int x, int y, int w, int h, bool fill){
    if (x >= OLED_X || y >= OLED_Y || w == 0 || h == 0) return;
    for(int i =0;i<w;++i){
        for(int j=0;j<h;++j){
            if((x+i)<OLED_X&&(y+j)<OLED_Y){
                m->bitmap[y+j][x+i]=m->bitmapWW;
                m->oledNow[y+j][x+i]=0;
            }
        }
    }
    if(!fill){
        if(w>2 && h>2){
            for(int i=0;i<w-2;++i){
                for(int j=0;j<h-2;++j){
                    if((x+i+1)<OLED_X&&(y+j+1)<OLED_Y){
                        m->bitmap[y+j+1][x+i+1]=m->bitmapBB;
                        m->oledNow[y+j+1][x+i+1]=1;
                    }
                }
            }
        }
    }
}
//End function of ISAOLED.h=======================================

//Start-button code here =========================================
/**
 * @brief MainWindow::on_button_Start_clicked
 * emulate Arduino loop()
 */
void MainWindow::on_button_Start_clicked()
{
    ui->button_Start->setText("Running");

    my_usercode->setup();
    while(loop){
        my_usercode->loop();
        QCoreApplication::processEvents();
    }
}
//Stop-button code here =========================================
void MainWindow::on_button_Stop_clicked()
{
    loop=false;
    delay(100);
    exit(0);
}

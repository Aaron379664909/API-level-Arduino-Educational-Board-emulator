#ifndef ISALEDCONTROL_H
#define ISALEDCONTROL_H
#include "isadefinitions.h"
class ISALedControl
{
  public:
    //ISALedControl();
    void init();
    void clearDisplay( );
    void setRow( int row, byte value);
    void setColumn(int col, byte value);
    void setLed(int row, int col, bool value);
};
#endif // ISALEDCONTROL_H

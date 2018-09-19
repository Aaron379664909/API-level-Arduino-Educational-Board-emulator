#ifndef ISALIQUIDCRYSTAL_H
#define ISALIQUIDCRYSTAL_H

#include "QString"

class ISALiquidCrystal {
public:
    ISALiquidCrystal(void)
    {
    }

    void begin();
    void print(QString data);
    void print(int data);
    void print(char data);
    void print(long data);
    void clear();
    void setCursor(int col,int row);

    //using
};
#endif // ISALIQUIDCRYSTAL_H

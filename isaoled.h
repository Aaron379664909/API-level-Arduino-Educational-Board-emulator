#ifndef ISAOLED_H
#define ISAOLED_H
#include "isadefinitions.h"
#include "QString"

class ISAOLED {
public:
    ISAOLED(void)
    {
    }

    void begin();
    void clear(bool render = true);
    void write(byte data);
    void gotoXY(int cx,int cy);
    void setPixel(int x,int y,bool v);
    void writeLine(int x1,int y1,int x2,int y2);
    void writeRect(int x, int y, int w, int h, bool fill=false);
    void print(QString text);
    void print(int x);
    void print(double x);
    void renderAll();

};
#endif // ISAOLED_H

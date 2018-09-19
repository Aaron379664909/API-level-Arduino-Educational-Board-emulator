#ifndef ISA7SEGMENTDISPLAY_H
#define ISA7SEGMENTDISPLAY_H

#include "isadefinitions.h"
class ISA7SegmentDisplay {

  public:
    ISA7SegmentDisplay() {}
    void init();
    void displayDigit(byte digit, int dispID, bool dot = false);
    void setLed(byte values, int dispID);
};
#endif // ISA7SEGMENTDISPLAY_H

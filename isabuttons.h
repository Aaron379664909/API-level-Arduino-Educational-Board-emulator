#ifndef ISABUTTONS_H
#define ISABUTTONS_H
class ISAButtons{
public:
  ISAButtons(){

  }

  void init();
  bool buttonPressed( int );
  bool buttonReleased( int );
  bool buttonState( int );
private:
#define numberOfButtons 16
};

#endif // ISABUTTONS_H

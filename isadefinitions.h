#ifndef ISADEFINITIONS_H
#define ISADEFINITIONS_H

//#include <stdint.h>

const bool HIGH = true;
const bool LOW = false;

const int INPUT = 0;
const int OUTPUT = 1;
const int INPUT_PULLUP = 2;

#define LED8		7
#define LED7		6
#define LED6		5
#define LED5		4
#define LED4		3
#define LED3		2
#define LED2		1
#define LED1		0

const static int LEDS[] = {LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8};

#define KEY_UP		20
#define KEY_DOWN	21
#define KEY_LEFT	22
#define KEY_RIGHT	23

const static int KEY_ARROWS[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};

#define POT         30

// joystick (X, Y)
#define JOY1X		40
#define JOY1Y		41
#define JOY2X		42
#define JOY2Y		43

typedef uint8_t byte;

#endif // ISADEFINITIONS_H

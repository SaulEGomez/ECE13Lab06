// ECE 13 Lab06 Leds_Lab06.c Saul Gomez 05.12.24

#include <stdio.h>

#include "Leds_Lab06.h"

#include <xc.h>

#include <stdlib.h>

#include "BOARD.h"

// Initializes the LEDs
void LEDS_INIT(void) {
    TRISE = 0x00; // Sets pins on port E as output
    LATE = 0x00; // Turns off LEDS in the beginning 
}

// Sets LED pattern
void LEDS_SET(char newPattern) {
    LATE = newPattern; // Updates port E with new pattern
}

//  Retrieves current LED pattern
char LEDS_GET(void) {
    return LATE; // Returns the current state of port E
}


// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
					
//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

#include "Leds_Lab06.h"

// **** Declare any datatypes here ****

struct Timer { // This struct manages timer events
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****

static struct Timer Timer1; //Timer1 struct instance 

enum direction {
    LEFT,
    RIGHT
};

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescalar
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to saegomez's lab6 part2 (bounce_switch).  Compiled on %s %s.\n",__TIME__,__DATE__);
    
    Timer1.event = FALSE; // Initializes event flag to FALSE
    
    // Initial state setup
    int state = RIGHT; // Initial state is moving right
    char currentLED = 0x01; // Start with the LED on the rightmost position
    char leftMost = 0x80; // LED on the leftmost position
    char rightMost = 0x01; // LED on the rightmost position
    
    
    LEDS_INIT(); // To initialize LEDs
							 
	while (1) {
        if (Timer1.event == TRUE) { // To see if Timer1 event has occured
            LEDS_SET(currentLED); // Set LEDs to current LED pattern
            if (currentLED == leftMost) {
                if (currentLED == 0x80) {
                    state = RIGHT; // Set state to RIGHT
                } else {
                    state = RIGHT;
                }
            } else if (currentLED == rightMost) { // Checks to see if current LED is rightmost LED
                if (currentLED == 0x01) {
                    state = LEFT;
                } else {
                    state = LEFT;
                }
            }
            if (state == LEFT) { // Checks if state is LEFT
                if (state == LEFT) {
                    currentLED <<= 1; // Shifts current LED pattern by 1 bit
                } else {
                    currentLED <<= 1;
                }
            } else {
                if (currentLED == leftMost) { // Checks if current LED is leftmost LED
                    if (currentLED == 0x80) {
                        currentLED = (currentLED >> 1) ^ currentLED;
                    } else {
                        currentLED = (currentLED >> 1) ^ currentLED;
                    }
                } else { // If current LED is not the leftmost LED
                    if (currentLED != leftMost) {
                        currentLED >>= 1; // Shifts current LED pattern by 1 bit
                    } else {
                        currentLED >>= 1;
                    }
                }
            }
        Timer1.event = FALSE;
    }
}


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}


/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    int scale = 3; // Scaling factor for the switch count to adjust speed
    Timer1.timeRemaining--; // Decrement the time remaining until the next LED change
    
    // If it's time to change the LED
    if (Timer1.timeRemaining <= 0) {
        // Set the event flag to indicate it's time
        Timer1.event = TRUE;
        
        // Check the switches to determine the new speed
        uint8_t switchCount = 0x00; // Variable to hold the count of active switches
        
        // Get the current state of the switches
        uint8_t switchesState = SWITCH_STATES();
        
        // Check each switch and update the switch count
        switchCount = __builtin_popcount(switchesState); // Count the number of set bits (active switches)
        
        switchCount *= scale; // Scale the switch count to make the speed difference more noticeable
        
        // Set the time remaining for the next LED change based on the switch count
        Timer1.timeRemaining = switchCount;
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/									
	 
}


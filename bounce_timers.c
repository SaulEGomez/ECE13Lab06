// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"


// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****

static struct Timer TimerA;
static struct Timer TimerB;
static struct Timer TimerC;

// **** Declare function prototypes ****

#define TIMER1_PERIOD 60
#define TIMER2_PERIOD 80
#define TIMER3_PERIOD 100

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
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
    printf("Welcome to saegomez's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);
    
    // Initializes the LED lights
    LEDS_INIT();
   
    // Initializes the event flags to false, which illustrates zero event progress 
    
    TimerA.event = FALSE;
    TimerB.event = FALSE;
    TimerC.event = FALSE;
    
    // Sets the initial time remaining for each timer
    
    TimerA.timeRemaining = TIMER1_PERIOD;
    TimerB.timeRemaining = TIMER2_PERIOD;
    TimerC.timeRemaining = TIMER3_PERIOD;
    
    // Char variable to hold current LED state
    
    char value = 0x00;
    
    while (1) {
        //poll timer A
        //react to timer A events
        //clear timer A event flag 
        if (TimerA.event == TRUE) {
            printf("A");
            TimerA.event = FALSE; // To clear event flag (3x for each timer))
            value ^= 0x01; // Toggle to corresponding LED (3x for each timer)
        }
        // poll timer B
        if (TimerB.event == TRUE) {
            printf("B");
            TimerB.event = FALSE;
            value ^= 0x02;
        }
        // poll timer C
        if (TimerC.event == TRUE) {
            printf("C");
            TimerC.event = FALSE;
            value ^= 0x04;
        }
        LEDS_SET(value); // Set the LED state based on value variable
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
    
    // update timerA
    // if timerA has counted down,
        // generate timerA event
        // reset timerA
    
    TimerA.timeRemaining--; // Decrements the remaining time
    if (!TimerA.timeRemaining) { // To check if timer ceased
        TimerA.event = TRUE; //Sets event flag (Happens 3x)
        TimerA.timeRemaining = TIMER1_PERIOD; // Resets timer (Happens 3x)
    }
    
    TimerB.timeRemaining--;
    if (!TimerB.timeRemaining) {
        TimerB.event = TRUE;
        TimerB.timeRemaining = TIMER2_PERIOD;
    }
    
    TimerC.timeRemaining--;
    if (!TimerC.timeRemaining) {
        TimerC.event = TRUE;
        TimerC.timeRemaining = TIMER3_PERIOD;
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}
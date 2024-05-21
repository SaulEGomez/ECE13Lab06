// ECE 13 Saul Gomez Lab06 ButtonsTest.c
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
static uint8_t timerEvent;

// **** Define global, module-level, or external variables here ****
volatile uint8_t buttonEvents = BUTTON_EVENT_NONE; // Module-level variable to store button events

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 2; // 1:64 prescaler
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
    printf("Welcome to saegomez's lab6 part4 (ButtonsTest).  Compiled on %s %s.\n", __TIME__, __DATE__);
    
    ButtonsInit(); // Initialize the buttons

    printf("Please press some buttons!\n");
    
    while (1) {
        if (buttonEvents != BUTTON_EVENT_NONE) { // Check if a button event has occured
            uint8_t events = buttonEvents;
            buttonEvents = BUTTON_EVENT_NONE; // Reset the event flag

            printf("EVENT: ");
            // Check each button event and corresponding message
            if (events & BUTTON_EVENT_4DOWN) {
                printf("4:DOWN ");
            }
            if (events & BUTTON_EVENT_4UP) {
                printf("4: UP ");
            }
            if (events & BUTTON_EVENT_3DOWN) {
                printf("3:DOWN ");
            }
            if (events & BUTTON_EVENT_3UP) {
                printf("3: UP ");
            }
            if (events & BUTTON_EVENT_2DOWN) {
                printf("2:DOWN ");
            }
            if (events & BUTTON_EVENT_2UP) {
                printf("2: UP ");
            }
            if (events & BUTTON_EVENT_1DOWN) {
                printf("1:DOWN ");
            }
            if (events & BUTTON_EVENT_1UP) {
                printf("1: UP ");
            }
            printf("\n"); // Print a newline after listing events
        }
    }
    
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    buttonEvents = ButtonsCheckEvents(); // Check for button events and store the result in the module-level variable
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}
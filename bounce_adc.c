// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>
#include <string.h>

// User libraries
#include "Oled.h"
#include "OledDriver.h"
#include "Leds_Lab06.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

struct AdcResult {
    uint8_t event;
    uint16_t voltage;
};

// **** Define global, module-level, or external variables here ****

static struct AdcResult AdcResult1;

#define SIZE 5

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

// Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
     AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time
    
    AD1CON1bits.ADON = 1; // and finally turn it on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
  printf("Welcome to saegomez's lab6 part3 (bounce_adc).  Compiled on %s %s.\n",__TIME__,__DATE__);

  OledInit(); // Initialize the OLED display
AdcResult1.event = TRUE; // Set initial ADC event flag
char stringToDisp[50]; // Buffer to hold the string to be displayed

while (1) {
    if (AdcResult1.event) { // Checks if there is a new ADC event
        // Calculates the percentage of the voltage out of 1023 using integer arithmetic
        int percent = (AdcResult1.voltage * 100) / 1023;
        
        // Formats string to display ADC percentage and voltage
        snprintf(stringToDisp, sizeof(stringToDisp), "Potentiometer value:\n   %d\n   %d%%", AdcResult1.voltage, percent);
        
        OledClear(OLED_COLOR_BLACK); // Clears OLED display
        OledDrawString(stringToDisp); // Draws formatted string onto OLED
        OledUpdate(); // Updates display with new info
        AdcResult1.event = FALSE; // Resets the ADC event flag
    }
}


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new ADC reading is available
 * in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_ADC_VECTOR, ipl2auto) AdcHandler(void)
{
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;
    
    // Calculate the new average reading from the ADC buffer
    uint16_t sum = ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7;
    uint16_t average = sum / 8;

    // If the difference between the average and the current voltage is greater than the size
    // set an event and update the voltage to the new average
    if (abs((int)average - (int)AdcResult1.voltage) >= SIZE) {
        AdcResult1.event = TRUE;
        // Update the voltage to the new average with edge case handling
        if (average > 1023 - SIZE) {
            AdcResult1.voltage = 1023;
        } else if (average < SIZE) {
            AdcResult1.voltage = 0;
        } else {
            AdcResult1.voltage = average;
        }
    }

}
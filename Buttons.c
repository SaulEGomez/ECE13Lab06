// ECE13 Saul Gomez Lab06 Buttons.c
#include <stdint.h>
#include <genericTypeDefs.h>

#include "BOARD.h"
#include "Buttons.h"

static uint8_t debounce;
static uint8_t prev;

// Macros for setting button states
#define BUTTON_INPUT_MASK (0x00E2)
#define BUTTON_DEBOUNCE_COUNT BUTTONS_DEBOUNCE_PERIOD

void ButtonsInit(void) {
    // Set the buttons to input mode
    TRISD |= BUTTON_INPUT_MASK;
    debounce = 0;
    prev = 0x00;
}

uint8_t ButtonsCheckEvents(void) {
    // Each call sets buttonStatus to no event
    uint8_t buttonStatus = BUTTON_EVENT_NONE;
    uint8_t status = BUTTON_STATES();
    
    // If debouncing, decrement debounce and return no event
    if (debounce > 0) {
        debounce--;
        return buttonStatus;
    }
    
    // Check if button states have changed
    if (status != prev) {
        // Check each button for changes
        if ((status & BUTTON_STATE_4) && !(prev & BUTTON_STATE_4)) {
            buttonStatus |= BUTTON_EVENT_4DOWN;
        } else if (!(status & BUTTON_STATE_4) && (prev & BUTTON_STATE_4)) {
            buttonStatus |= BUTTON_EVENT_4UP;
        }
        if ((status & BUTTON_STATE_3) && !(prev & BUTTON_STATE_3)) {
            buttonStatus |= BUTTON_EVENT_3DOWN;
        } else if (!(status & BUTTON_STATE_3) && (prev & BUTTON_STATE_3)) {
            buttonStatus |= BUTTON_EVENT_3UP;
        }
        if ((status & BUTTON_STATE_2) && !(prev & BUTTON_STATE_2)) {
            buttonStatus |= BUTTON_EVENT_2DOWN;
        } else if (!(status & BUTTON_STATE_2) && (prev & BUTTON_STATE_2)) {
            buttonStatus |= BUTTON_EVENT_2UP;
        }
        if ((status & BUTTON_STATE_1) && !(prev & BUTTON_STATE_1)) {
            buttonStatus |= BUTTON_EVENT_1DOWN;
        } else if (!(status & BUTTON_STATE_1) && (prev & BUTTON_STATE_1)) {
            buttonStatus |= BUTTON_EVENT_1UP;
        }

        // Set debounce period and update previous state
        debounce = BUTTON_DEBOUNCE_COUNT;
        prev = status;
    }
    
    return buttonStatus;
}

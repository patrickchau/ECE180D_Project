/*************************************************************************/
/*                            hardware.h                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                          Hardware Token                               */
/*           https://github.com/patrickchau/ECE180D_Project              */
/*************************************************************************/
/*                Copyright  10-31-2019 Joseph Miller.                   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef HARDWARE_DEFINED
#define HARDWARE_DEFINED

// GPIO Pins
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define S1 7
#define S2 21
#define SW 22
#define BN1 23
#define BN2 25
#define LED 26
#define S3 27

#define NUM_SEGS 7

/******************************************************
 * Function: clear_pins
 *-----------------------------------------------------
 * Sets all GPIO pins low.
 *-----------------------------------------------------
 * Return Values:
 * None.
 * ----------------------------------------------------
 * Inputs:
 * None.
******************************************************/
void clear_pins();

/******************************************************
 * Function: init_pins
 *-----------------------------------------------------
 * Initializes GPIO pins with wiringPi.
 *-----------------------------------------------------
 * Return Values:
 * None.
 * ----------------------------------------------------
 * Inputs:
 * None.
******************************************************/
void init_pins();

/******************************************************
 * Function: run_display
 *-----------------------------------------------------
 * Manages timing for 7_seg display, button debouncing,
 * LED timing and switch status
 *-----------------------------------------------------
 * Return Values:
 * None.
 * ----------------------------------------------------
 * Inputs:
 * None.
 * ----------------------------------------------------
 * Note:
 * Modifies global variables corresponding to rows and
 * cols.
******************************************************/
void* run_display(void* arg);

/******************************************************
 * Function: init_pins
 *-----------------------------------------------------
 * Turns on and off the LED for the segment.
 *-----------------------------------------------------
 * Return Values:
 * None.
 * ----------------------------------------------------
 * Inputs:
 * seg is the GPIO pin corresponding to the common
 * cathode of the 7_seg display.
 * display_Char is an unsigned char that corresponds to
 * the bitmap of LEDs for the 7_seg that are on and off
 * time_delay is the time in usec that will be used in
 * between blinking LEDs of 7_seg.
******************************************************/
void blink_segment(const int seg, const unsigned char display_char, int time_delay);

/******************************************************
 * Function: integer_to_display
 *-----------------------------------------------------
 * Converts an int to an unsigned char bitmap used to
 * blink the LEDS of the segment to write the int.
 *-----------------------------------------------------
 * Return Value:
 * Returns an unsigned char that is properly aligned to
 * be used for blink segment
 * ----------------------------------------------------
 * Inputs:
 * Takes as input an integer that will be converted to
 * a bitmap for the LEDs of the seven segment display
******************************************************/
unsigned char integer_to_display(int num_to_display);

/******************************************************
 * Function: character_to_display
 *-----------------------------------------------------
 * Converts a char to an unsigned char bitmap used to
 * blink the LEDS of the segment to write the int.
 *-----------------------------------------------------
 * Return Value:
 * Returns an unsigned char that is properly aligned to
 * be used for blink segment
 * ----------------------------------------------------
 * Inputs:
 * Takes as input a char that will be converted to
 * a bitmap for the LEDs of the seven segment display
******************************************************/
unsigned char character_to_display(char char_to_display);

/******************************************************
 * Function: display_connecting
 *-----------------------------------------------------
 * Scrolls connecting across the 7_segments
 *-----------------------------------------------------
 * Return Value:
 * None.
 * ----------------------------------------------------
 * Inputs:
 * None.
******************************************************/
void display_connecting();

#endif //HARDWARE_DEFINED
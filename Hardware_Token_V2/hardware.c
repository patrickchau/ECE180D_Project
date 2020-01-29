/*************************************************************************/
/*                            hardware.c                                 */
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

#include "hardware.h"
#include "globals.h"
#include "signal_handler.h"

// Provides GPIO Pin Mappings
#include <wiringPi.h>

// STD libraries
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <netdb.h> 
#include <string.h>

// Multithreading
#include <pthread.h> 

// Rows and Cols Definition
int row_pos_ones = 0;
int row_pos_tens = 0;
int col_pos_ones = 0;
int col_pos_tens = 0;
int msg_to_display = 0;

void* run_display(void* arg) {

    // Remove compiler warning
    (void)arg;

    // Mask sigpipe on this thread.
    mask_sigpipe();
    
    int switch_on = 0;
    int button1_off = 1;
    int button1_held = 0;
    int button2_held = 0;
    int button2_off = 1;

    while (!program_end) {
            // Write out message from buffer if we have one
            if(msg_to_display == 1) {
                display_message(display_msg, strlen(display_msg));
                msg_to_display = 0;
            }

            // Run Normal Hardware code.
            switch_on = digitalRead(SW);
            button1_off = digitalRead(BN1);
            button2_off = digitalRead(BN2);

            if(switch_on) {
                if(!button1_off && !button1_held) {
                    pthread_mutex_lock(&lock);
                    row_pos_ones = row_pos_ones + 1;
                    if(row_pos_ones > 9) {
                        row_pos_tens = row_pos_tens + 1;
                        row_pos_ones = 0;
                        if(row_pos_tens > 9) {
                            row_pos_tens = 0;
                        } 
                    }
                    pthread_mutex_unlock(&lock);
                    button1_held = 1;
                } else if(button1_off) {
                    button1_held = 0;
                }
                    if(!button2_off && !button2_held) {
                        pthread_mutex_lock(&lock);
                        row_pos_ones = row_pos_ones - 1;
                        if(row_pos_ones < 0){
                            row_pos_tens = row_pos_tens - 1;
                            row_pos_ones = 9;
                            if(row_pos_tens < 0){
                                row_pos_tens = 9;
                            }
                        }
                        pthread_mutex_unlock(&lock);
                        button2_held = 1;
                    } else if(button2_off){
                        button2_held = 0;
                }

                // Other thread only reads from globals so no need for lock
                blink_segment(S2, integer_to_display(row_pos_tens) , hun_usec_delay);
                blink_segment(S3, integer_to_display(row_pos_ones) , hun_usec_delay);
                blink_segment(S1, character_to_display('r'), hun_usec_delay); 

            }
            else{
                if(!button1_off && !button1_held){
                    pthread_mutex_lock(&lock);
                    col_pos_ones = col_pos_ones + 1;
                    if(col_pos_ones > 9) {
                        col_pos_tens = col_pos_tens + 1;
                        col_pos_ones = 0;
                        if(col_pos_tens > 9) {
                            col_pos_tens = 0;
                        } 
                    }
                    pthread_mutex_unlock(&lock);
                    button1_held = 1;
                    } else if(button1_off){
                        button1_held = 0;
                    }
                    if(!button2_off && !button2_held){
                        pthread_mutex_lock(&lock);
                        col_pos_ones = col_pos_ones - 1;
                        if(col_pos_ones < 0){
                            col_pos_tens = col_pos_tens - 1;
                            col_pos_ones = 9;
                            if(col_pos_tens < 0){
                                col_pos_tens = 9;
                            }
                        }
                        pthread_mutex_unlock(&lock);
                    button2_held = 1;
                    } else if(button2_off){
                        button2_held = 0;
                }

                // Other thread only reads from globals so no need for lock
                blink_segment(S2, integer_to_display(col_pos_tens), hun_usec_delay);
                blink_segment(S3, integer_to_display(col_pos_ones), hun_usec_delay);
                blink_segment(S1, character_to_display('c'), hun_usec_delay);
            }    
    }

    fprintf(stdout, "Hardware thread exiting...\n");
    return NULL;
}

void clear_pins() {
    
    // Set outputs low before beginning
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);
    digitalWrite(C, LOW);
    digitalWrite(D, LOW);
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
    digitalWrite(S1, LOW);
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
}

void init_pins() {

    // Use default wiringPiSetup
    wiringPiSetup();

    // Set I/O status for pins
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(F, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(SW, INPUT);
    pinMode(BN1, INPUT);
    pinMode(BN2, INPUT);

    // Set outputs low before beginning
    clear_pins();
}

void blink_segment(const int seg, const unsigned char display_char, int time_delay) {

    const unsigned char mask = 0x01;

    // In case pin mappings are changed.
    unsigned int led[NUM_SEGS] = {A,B,C,D,E,F,G};

    // Turn Segment on (Common Cathode display).
    digitalWrite(seg, LOW);

    for (int i = 0; i < NUM_SEGS; i++) {
        unsigned char tmp = (display_char >> i) & mask;
        if(tmp == 0x01) {
            //Blink this LED
            digitalWrite(led[i], HIGH);
            delayMicroseconds(time_delay);
            digitalWrite(led[i], LOW);
        } else {
            //Leave LED off
            delayMicroseconds(time_delay);
            digitalWrite(led[i], LOW);
        }
    }

    // Turn Segment off (Common Cathode display).
    digitalWrite(seg, HIGH);
}

unsigned char integer_to_display(int num_to_display) {

    switch(num_to_display) {
        case 0:
                return 0x3F;
                break;
        case 1:
                return 0x06;
                break;
        case 2:
                return 0x5B;
                break;
        case 3:
                return 0x4F;
                break;
        case 4:
                return 0x66;
                break;
        case 5:
                return 0x6D;
                break;
        case 6:
                return 0x7D;
                break;
        case 7:
                return 0x07;
                break;
        case 8:
                return 0x7F;
                break;
        case 9:
                return 0x6F;
                break;
        default:
                return 0x00;
                break;
    }
}

unsigned char character_to_display(char char_to_display) {

    switch(char_to_display) {
        case 'r':
                return 0x50;
                break;
        case 'c':
                return 0x58;
                break;
        case 'o':
                return 0x5C;
                break;
        case 'n':
                return 0x54;
                break;
        case 'e':
                return 0x79;
                break;
        case 't':
                return 0x78;
                break;
        case 'i':
                return 0x04;
                break;
        case 'g':
                return 0x6F;
                break;
        case 's':
                return 0x6D;
                break;
        case 'u':
                return 0x1C;
                break;
        default:
                return 0x00;
                break;
    }
}

void display_connecting(char* msg, int len) {

    int init_time = millis();
    int i = -2;
    while(!server_connected && !program_end) {
        int curr_time = millis();

        // If 400 msec have passed scroll over 1.
        if(curr_time - init_time > 400) {
            i += 1;
            if(i == len) {
                break;
            }
            init_time = curr_time;
        }
     
        // Allow it to scroll starting from the right.
        if(i < len && i >= 0) {
            blink_segment(S1, character_to_display(msg[i]), hun_usec_delay);
        }
        else {
            blink_segment(S1, character_to_display(msg[' ']), hun_usec_delay);
        }

        if(i + 1 < len && i + 1 >= 0) {
            blink_segment(S2, character_to_display(msg[i+1]), hun_usec_delay);
        }
        else {
            blink_segment(S2, character_to_display(msg[' ']), hun_usec_delay);
        }
        
        if(i + 2 < len && i + 2 >= 0) {
            blink_segment(S3, character_to_display(msg[i+2]), hun_usec_delay);
        }
        else {
            blink_segment(S3, character_to_display(msg[' ']), hun_usec_delay);
        }
    }
}

void display_message(char* msg, int len) {

    int init_time = millis();
    int i = -2;
    while(!program_end) {
        int curr_time = millis();

        // If 400 msec have passed scroll over 1.
        if(curr_time - init_time > 400) {
            i += 1;
            if(i == len) {
                break;
            }
            init_time = curr_time;
        }
     
        // Allow it to scroll starting from the right.
        if(i < len && i >= 0) {
            blink_segment(S1, character_to_display(msg[i]), hun_usec_delay);
        }
        else {
            blink_segment(S1, character_to_display(msg[' ']), hun_usec_delay);
        }

        if(i + 1 < len && i + 1 >= 0) {
            blink_segment(S2, character_to_display(msg[i+1]), hun_usec_delay);
        }
        else {
            blink_segment(S2, character_to_display(msg[' ']), hun_usec_delay);
        }
        
        if(i + 2 < len && i + 2 >= 0) {
            blink_segment(S3, character_to_display(msg[i+2]), hun_usec_delay);
        }
        else {
            blink_segment(S3, character_to_display(msg[' ']), hun_usec_delay);
        }
    }
}
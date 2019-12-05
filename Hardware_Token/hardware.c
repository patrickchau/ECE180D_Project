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

// Provides GPIO Pin Mappings
#include <wiringPi.h>

// STD libraries
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <netdb.h> 

// Multithreading
#include <pthread.h> 

// Rows and Cols Definition
int row_pos_ones = 0;
int row_pos_tens = 0;
int col_pos_ones = 0;
int col_pos_tens = 0;   

void* run_display(void* arg) {

    // Remove compiler warning
    (void)arg;
    
    int switch_on = 0;
    int button1_off = 1;
    int button1_held = 0;
    int button2_held = 0;
    int button2_off = 1;

    while (FOREVER) {
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
            blink_segment(S2, row_pos_tens , hun_usec_delay);
            blink_segment(S3, row_pos_ones , hun_usec_delay);
            blink_segment(S1, LOWERCASE_R, hun_usec_delay);
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
            blink_segment(S2, col_pos_tens , hun_usec_delay);
            blink_segment(S3, col_pos_ones , hun_usec_delay);
            blink_segment(S1, LOWERCASE_C, hun_usec_delay);
        }    
    }

    return NULL;
}

void init_pins(){

    // Use default wiringPiSetup
    wiringPiSetup();

    //Set I/O status for pins
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

    //Set outputs low before beginning
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


void blink_segment(const int seg, int num_to_display, int time_delay) {

    switch(num_to_display) {
        case 0:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(E, LOW);

                digitalWrite(F, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, LOW);
                break;

        case 1:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, LOW);

                digitalWrite(F, LOW);

                digitalWrite(G, LOW);
                break;

        case 2:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(E, LOW);

                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(G, LOW);
                break;

        case 3:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, LOW);

                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(G, LOW);
                break;

        case 4:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(G, LOW);
                break;
        case 5:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(G, LOW);
                break;
        case 6:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(D, LOW);


                digitalWrite(E, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(G, LOW);
                break;
        case 7:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, LOW);

                digitalWrite(G, LOW);
                break;
        case 8:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(G, LOW);
                break;
        case 9:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(G, LOW);
                break;
        case 10:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(G, LOW);
                break;
        case 11:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(time_delay);
                digitalWrite(G, LOW);
                break;
        default:
                digitalWrite(seg, LOW);
                digitalWrite(A, LOW);
                digitalWrite(B, LOW);
                digitalWrite(C, LOW);
                digitalWrite(D, LOW);
                digitalWrite(E, LOW);
                digitalWrite(F, LOW);
                digitalWrite(G, LOW);
                break;
    }
    digitalWrite(seg, HIGH);
}
#include <wiringPi.h>

const int A = 0;
const int B = 1;
const int C = 2;
const int D = 3;
const int E = 4;
const int F = 5;
const int G = 6;
const int S1 = 7;
const int S2 = 21;
const int SW = 22;
const int BN1 = 23;
const int BN2 = 25;
const int LED = 26;
const int S3 = 27;
const int LOWERCASE_C = 10;
const int LOWERCASE_R = 11;


void blink_segment(const int seg, int num_to_display, int delay) {

    switch(num_to_display) {
        case 0:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(delay);
                digitalWrite(E, LOW);

                digitalWrite(F, HIGH);
                delayMicroseconds(delay);
                digitalWrite(F, LOW);

                digitalWrite(G, LOW);
                break;

        case 1:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(delay);
                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, LOW);

                digitalWrite(F, LOW);

                digitalWrite(G, LOW);
                break;

        case 2:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(delay);
                digitalWrite(B, LOW);

                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(delay);
                digitalWrite(E, LOW);

                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(delay);
                digitalWrite(G, LOW);
                break;

        case 3:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(delay);
                digitalWrite(D, LOW);

                digitalWrite(E, LOW);

                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(delay);
                digitalWrite(G, LOW);
                break;

        case 4:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(delay);
                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(delay);
                digitalWrite(G, LOW);
                break;
        case 5:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(delay);
                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(delay);
                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(delay);
                digitalWrite(G, LOW);
                break;
        case 6:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(delay);
                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(delay);
                digitalWrite(D, LOW);


                digitalWrite(E, HIGH);
                delayMicroseconds(delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(delay);
                digitalWrite(G, LOW);
                break;
        case 7:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(delay);
                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, LOW);

                digitalWrite(G, LOW);
                break;
        case 8:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(delay);
                digitalWrite(G, LOW);
                break;
        case 9:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(delay);
                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(delay);
                digitalWrite(G, LOW);
                break;
        case 10:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(delay);
                digitalWrite(G, LOW);
                break;
        case 11:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(delay);
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

int main(void)
{
    const int delay = 100; //usec
    //Use default wiringPiSetup
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

    int row_pos_ones = 0;
    int row_pos_tens = 0;
    int col_pos_ones = 0;
    int col_pos_tens = 0;   
    int switch_on = 0;
    int button1_off = 1;
    int button1_held = 0;
    int button2_held = 0;
    int button2_off = 1;

    while (1) {
        switch_on = digitalRead(SW);
        button1_off = digitalRead(BN1);
        button2_off = digitalRead(BN2);

        if(switch_on){
            if(!button1_off && !button1_held){
                row_pos_ones = row_pos_ones + 1;
                if(row_pos_ones > 9) {
                row_pos_tens = row_pos_tens + 1;
                row_pos_ones = 0;
                if(row_pos_tens > 9){
                    row_pos_tens = 0;
                } 
                }
                button1_held = 1;
                } else if(button1_off){
                    button1_held = 0;
                }
                if(!button2_off && !button2_held){
                    row_pos_ones = row_pos_ones - 1;
                    if(row_pos_ones < 0){
                        row_pos_tens = row_pos_tens - 1;
                        row_pos_ones = 9;
                        if(row_pos_tens < 0){
                            row_pos_tens = 9;
                        }
                    }
                button2_held = 1;
                } else if(button2_off){
                    button2_held = 0;
            }
            blink_segment(S2, row_pos_tens , delay);
            blink_segment(S3, row_pos_ones , delay);
            blink_segment(S1, LOWERCASE_R, delay);
        }
        else{
            if(!button1_off && !button1_held){
                col_pos_ones = col_pos_ones + 1;
                if(col_pos_ones > 9) {
                col_pos_tens = col_pos_tens + 1;
                col_pos_ones = 0;
                if(col_pos_tens > 9){
                    col_pos_tens = 0;
                } 
                }
                button1_held = 1;
                } else if(button1_off){
                    button1_held = 0;
                }
                if(!button2_off && !button2_held){
                    col_pos_ones = col_pos_ones - 1;
                    if(col_pos_ones < 0){
                        col_pos_tens = col_pos_tens - 1;
                        col_pos_ones = 9;
                        if(col_pos_tens < 0){
                            col_pos_tens = 9;
                        }
                    }
                button2_held = 1;
                } else if(button2_off){
                    button2_held = 0;
            }
            blink_segment(S2, col_pos_tens , delay);
            blink_segment(S3, col_pos_ones , delay);
            blink_segment(S1, LOWERCASE_C, delay);
        }    
    }

    return 0;
}

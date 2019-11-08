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
//const int S3 = 9;

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
    //pinMode(S3, OUTPUT);

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
    //digitalWrite(S3, LOW);
    int cur_sec = 8;
    int cur_dec_sec = 9;

    while (1) {
    blink_segment(S1, cur_sec , delay);
    blink_segment(S2, cur_dec_sec , delay);
    
    }

    return 0;
}

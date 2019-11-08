#include <wiringPi.h>

int main(void)
{
    //LED initialization
    const int led0 = 0;
    const int led1 = 1;
    const int led2 = 2;
    const int led3 = 3;
    const int led4 = 4;
    const int led5 = 5;
    const int led6 = 6;
    const int led7 = 7;

    const int delay = 100000;
    //Use default wiringPiSetup
    wiringPiSetup();

    //Set I/O status for pins
    pinMode(led0, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(led5, OUTPUT);
    pinMode(led6, OUTPUT);
    pinMode(led7, OUTPUT);

    //Set outputs low before beginning
    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);


    while (1) {
        digitalWrite(led0, HIGH);
        delayMicroseconds(delay);
        digitalWrite(led0, LOW);
        delayMicroseconds(delay);

        digitalWrite(led1, HIGH);
        delayMicroseconds(delay);
        digitalWrite(led1, LOW);
        delayMicroseconds(delay);

        digitalWrite(led2, HIGH);
        delayMicroseconds(delay);
        digitalWrite(led2, LOW);
        delayMicroseconds(delay);

        digitalWrite(led3, HIGH);
        delayMicroseconds(delay);
        digitalWrite(led3, LOW);
        delayMicroseconds(delay);

        digitalWrite(led4, HIGH);
        delayMicroseconds(delay);
        digitalWrite(led4, LOW);
        delayMicroseconds(delay);

        digitalWrite(led5, HIGH);
        delayMicroseconds(delay);
        digitalWrite(led5, LOW);
        delayMicroseconds(delay);

        digitalWrite(led6, HIGH);
        delayMicroseconds(delay);
        digitalWrite(led6, LOW);
        delayMicroseconds(delay);

        digitalWrite(led7, HIGH);
        delayMicroseconds(delay);
        digitalWrite(led7, LOW);
        delayMicroseconds(delay);
    }

    return 0;
}

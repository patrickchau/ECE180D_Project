/*************************************************************************/
/*                               7_seg.c                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                          Hardware Token                                */
/*           https://github.com/patrickchau/ECE180D_Project              */
/*************************************************************************/
/*                 Copyright  10-31-2019 Joseph Miller.                  */
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

#include <wiringPi.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <netdb.h> 
#include <string.h>

// Multithreading
#include <pthread.h> 

//Sockets
#include <sys/socket.h>
#include <arpa/inet.h>

// Allows for error codes
#include <errno.h>

// For Open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Socket Params
#define BUFFER_MAX 128 
#define PORT 8080 
#define SA struct sockaddr 
#define BAUDRATE 4096

// Seven Seg Params
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

// Return Codes
const int SUCCESSFUL_CONN = 0;
const int FAILED_CONN = 1;

// Other Params
#define FOREVER 1
const int usec_delay = 100; //usec

// Threads
pthread_mutex_t lock;

// Rows and Cols
int row_pos_ones = 0;
int row_pos_tens = 0;
int col_pos_ones = 0;
int col_pos_tens = 0;   

//Function Declarations
void blink_segment(const int seg, int num_to_display, int usec_delay);
void init_pins();
void* run_display(void* arg);
int attempt_connection(int* sockfd);
void* server_communication(void* sockfd);

int main(void)
{
    // Use default wiringPiSetup
    wiringPiSetup();
    init_pins();

    int sockfd; 

    // Attempt communication with server
    while(attempt_connection(&sockfd));

    // Init threads
    pthread_t network_thread = 0;
    pthread_t hardware_thread = 1; 

    // Init mutex
    pthread_mutex_init(&lock, NULL);
     
    // Communicate with server
    pthread_create(&network_thread, NULL, server_communication, &sockfd);
  
    // Run the display
    pthread_create(&hardware_thread, NULL, run_display, NULL); 

    // Wait for threads to finish
    pthread_join(hardware_thread, NULL);
    pthread_join(network_thread, NULL);

    // Close threads
    pthread_exit(NULL); 

    // Close the socket 
    close(sockfd); 

    return 0;
}

void init_pins(){

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

void* run_display(void* arg) {
    
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
            blink_segment(S2, row_pos_tens , usec_delay);
            blink_segment(S3, row_pos_ones , usec_delay);
            blink_segment(S1, LOWERCASE_R, usec_delay);
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
            blink_segment(S2, col_pos_tens , usec_delay);
            blink_segment(S3, col_pos_ones , usec_delay);
            blink_segment(S1, LOWERCASE_C, usec_delay);
        }    
    }

    return NULL;
}

int attempt_connection(int* sockfd){
    
    struct sockaddr_in servaddr;

    // Generate Socket File Descriptor
    int attempted_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Verify Socket Was Created
    if (attempted_sockfd == -1) { 
        fprintf(stderr, "Error Creating Socket: %s\n", strerror(errno));

        // Wait one second before reattempting
        delay(1000);
        return FAILED_CONN; 
    }

    // Setup Server Info
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.43.190");
    servaddr.sin_port = htons(PORT); 
  
    // Connect the client socket to server socket 
    if (connect(attempted_sockfd, (SA*)&servaddr, sizeof(servaddr)) == -1) { 
        fprintf(stderr, "Error Connecting to Server: %s\n", strerror(errno));

        // Close failed socket
        close(attempted_sockfd);  

        // Wait one second before reattempting
        delay(1000);
        return FAILED_CONN;
    } 
    else{
        // Set sockfd to succesfully connected socket
        *sockfd = attempted_sockfd;
    }
    
    return SUCCESSFUL_CONN;
}

void* server_communication(void* arg) 
{ 
    // Get sockfd from pthread
    int sockfd = *(int *)arg;

    // Strings
    char msg[BUFFER_MAX];
    char MAC_ADDR_PATH[BUFFER_MAX];
    char MAC_ADDR[18];
    char interface[6] = "wlan0";
    char msg_nothing[8] = "nothing";
    
    // Zero out buffers
    bzero(msg, sizeof(msg));
    bzero(MAC_ADDR_PATH, sizeof(BUFFER_MAX));
    bzero(MAC_ADDR, sizeof(MAC_ADDR));

    // Write filepath to file containing MAC ADDR
    sprintf(MAC_ADDR_PATH, "/sys/class/net/%s/address", interface);

    // Open file containing MAC ADDR
    int MACfd;
    MACfd = open(MAC_ADDR_PATH, O_RDONLY);
    if(MACfd != -1) {
        // Read the 17 characters corresponding to MAC ADDR
        read(MACfd, MAC_ADDR, sizeof(char)*17);
    } else {
        // Use a hopefully unique identifier for MAC ADDR
        sprintf(MAC_ADDR, "83:13:71:76:13:98");
    }

    // Write filepath to file containing MAC ADDR
    sprintf(msg, "start,%s", MAC_ADDR);
    fprintf(stdout, "Token sent: %s\n", msg);

    // Deliver MAC_ADDR to server
    write(sockfd, msg, sizeof(msg));
    
    while (FOREVER) { 
        
        // Give server a chance to read and send messages
        delay(1000);

        // Read data from server
        bzero(msg, sizeof(msg));
        read(sockfd, msg, sizeof(msg));
        fprintf(stdout, "Server Returned: %s\n", msg);

        if((strncmp(msg, "position", 8)) == 0) { 
            // Calculate row and col for token
            pthread_mutex_lock(&lock);
            int row = row_pos_tens*10 + row_pos_ones;
            int col = col_pos_tens*10 + col_pos_ones;
            pthread_mutex_unlock(&lock);

            // Format string for delivery
            bzero(msg, sizeof(msg));
            sprintf(msg, "Token sent: position,%d.%d", row, col);
            write(sockfd, msg, sizeof(msg));
            fprintf(stdout, "%s\n", msg);
        }
        else {
            // Ping server to let it know we are still connected
            write(sockfd, msg_nothing, sizeof(msg_nothing));
            fprintf(stdout, "Nothing to send, pinging server\n");
        }
    }

    return NULL;
} 

void blink_segment(const int seg, int num_to_display, int usec_delay) {

    switch(num_to_display) {
        case 0:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(E, LOW);

                digitalWrite(F, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, LOW);
                break;

        case 1:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, LOW);

                digitalWrite(F, LOW);

                digitalWrite(G, LOW);
                break;

        case 2:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(E, LOW);

                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(G, LOW);
                break;

        case 3:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, LOW);

                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(G, LOW);
                break;

        case 4:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(G, LOW);
                break;
        case 5:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(G, LOW);
                break;
        case 6:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(D, LOW);


                digitalWrite(E, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(G, LOW);
                break;
        case 7:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, LOW);

                digitalWrite(G, LOW);
                break;
        case 8:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(G, LOW);
                break;
        case 9:
                digitalWrite(seg, LOW);

                digitalWrite(A, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(A, LOW);

                digitalWrite(B, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(B, LOW);

                digitalWrite(C, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, LOW);
               
                digitalWrite(F, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(G, LOW);
                break;
        case 10:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, LOW);

                digitalWrite(D, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(G, LOW);
                break;
        case 11:
                digitalWrite(seg, LOW);

                digitalWrite(A, LOW);

                digitalWrite(B, LOW);

                digitalWrite(C, LOW);

                digitalWrite(D, LOW);

                digitalWrite(E, HIGH);
                delayMicroseconds(usec_delay);
                digitalWrite(E, LOW);
               
                digitalWrite(F, LOW);

                digitalWrite(G, HIGH);
                delayMicroseconds(usec_delay);
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
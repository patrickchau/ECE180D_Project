/*************************************************************************/
/*                          hardware_token.c                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                          Hardware Token                               */
/*           https://github.com/patrickchau/ECE180D_Project              */
/*************************************************************************/
/*                 Copyright  12-5-2019 Joseph Miller.                   */
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

#include "globals.h"
#include "network.h"
#include "hardware.h"
#include "signal_handler.h"

// STD libraries
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

// Lock Declaration
pthread_mutex_t lock;

int program_end = 0;
int main(void) {
    
    // Init GPIO pin mappings and set signal handlers.
    init_pins();
    set_sig_handlers();
    
    // Init threads
    pthread_t network_thread = 0;
    pthread_t hardware_thread = 1; 

    // Init mutex
    pthread_mutex_init(&lock, NULL);

    // Run the display
    pthread_create(&hardware_thread, NULL, run_display, NULL);

    // Attempt communication with server
    int sockfd; 
    while(attempt_connection(&sockfd) && !program_end);
     
    // Communicate with server
    pthread_create(&network_thread, NULL, server_communication, &sockfd);
  
    // Wait for threads to finish
    pthread_join(hardware_thread, NULL);
    pthread_join(network_thread, NULL);

    // Close threads
    pthread_exit(NULL); 

    // Close the socket 
    close(sockfd);

    return 0;
}
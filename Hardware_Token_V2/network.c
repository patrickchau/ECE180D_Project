/*************************************************************************/
/*                             network.c                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                          Hardware Token                               */
/*           https://github.com/patrickchau/ECE180D_Project              */
/*************************************************************************/
/*                Copyright  11-14-2019 Joseph Miller.                   */
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
#include "signal_handler.h"

// STD libraries
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <netdb.h> 
#include <string.h>

// For delay
#include <wiringPi.h>

// Sockets
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <fcntl.h>

// Allows for error codes
#include <errno.h>

// For Open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int server_connected = 0;
char display_msg[20] = "";
int client_sockfd = -1;

int attempt_connection(int* sockfd){

    // Mask SIGTERM on this thread.
    mask_sigterm();

    // Generate Socket File Descriptor
    int attempted_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Verify Socket Was Created
    if (attempted_sockfd == -1) { 
        fprintf(stderr, "Error Creating Socket: %s\n", strerror(errno));

        // Wait one second before reattempting
        delay(sec_delay);
        return FAILED_CONN; 
    }

    // Forcefully attaching socket to the port 8080 
    int opt = 1;
    if (setsockopt(attempted_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        fprintf(stderr, "Error Setting Sockopt: %s\n", strerror(errno));

        // Wait one second before reattempting
        delay(sec_delay);
        return FAILED_CONN; 
    } 

    // Setup Server Info
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT); 

    // Forcefully attaching socket to the port 8080 
    if (bind(attempted_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
    { 
        fprintf(stderr, "Failed to bind addr: %s\n", strerror(errno));

        // Wait one second before reattempting
        delay(sec_delay);
        return FAILED_CONN; 
    } 

    int max_connections = 1;
    if (listen(attempted_sockfd, max_connections) < 0)
    { 
        fprintf(stderr, "Error Detecting Client: %s\n", strerror(errno));

        // Wait one second before reattempting
        delay(sec_delay);
        return FAILED_CONN; 
    }

    int addrlen = sizeof(servaddr);
    if ((client_sockfd = accept(attempted_sockfd, (struct sockaddr *)&servaddr, (socklen_t*)&addrlen)) < 0) 
    { 
        fprintf(stderr, "Error Accepting Client: %s\n", strerror(errno));

        // Wait one second before reattempting
        delay(sec_delay);
        return FAILED_CONN; 
    }

    // Set sockfd to succesfully connected socket
    *sockfd = attempted_sockfd;
    server_connected = 1;

    return SUCCESSFUL_CONN;
}

void* server_communication(void* arg) 
{ 
    // Get sockfd from pthread
    int server_sockfd = *(int *)arg;

    // Don't allow this thread to handle these
    mask_sigtstp();
    mask_sigterm();

    // Strings
    char msg[BUFFER_MAX];
    
    while (!program_end) { 
        if(server_connected == 1) {
            // Give server a chance to read and send messages
            delay(sec_delay);

            // Read msg from client
            bzero(msg, sizeof(msg));
            read(client_sockfd , msg, 1024); 
            printf("received: %s\n", msg);
            
            // Calculate row and col for token
            pthread_mutex_lock(&lock);
            int row = row_pos_tens*10 + row_pos_ones;
            int col = col_pos_tens*10 + col_pos_ones;
            pthread_mutex_unlock(&lock);

            // Format string for delivery
            bzero(msg, sizeof(msg));
            sprintf(msg, "position,%d.%d", row, col);
            send(client_sockfd, msg, strlen(msg), 0 ); 
            fprintf(stdout, "Token sent: %s\n", msg);

            // Allow hardware thread to write out success
            bzero(display_msg, sizeof(display_msg));
            sprintf(display_msg, "success");
            msg_to_display = 1;

            // Disconnect from client
            server_connected = 0;
            close(client_sockfd);
        } else{
            // Connect to new client.
            // TODO: Check if client has already been talked with.
            // Attempt connection to new client.
            while(attempt_connection(&server_sockfd)) {
                // Check if program is still running.
                if(program_end) {
                    fprintf(stdout, "Network thread exiting...\n");
                    return NULL;
                }
            }
            server_connected = 1;
        }
    }

    fprintf(stdout, "Network thread exiting...\n");
    return NULL;
} 
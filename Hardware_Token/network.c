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
#include <fcntl.h>

// Allows for error codes
#include <errno.h>

// For Open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int server_connected = 0;
char display_msg[20] = "";

int attempt_connection(int* sockfd){

    // Mask SIGTERM on this thread.
    mask_sigterm();
    
    struct sockaddr_in servaddr;

    // Generate Socket File Descriptor
    int attempted_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Verify Socket Was Created
    if (attempted_sockfd == -1) { 
        fprintf(stderr, "Error Creating Socket: %s\n", strerror(errno));

        // Wait one second before reattempting
        delay(sec_delay);
        return FAILED_CONN; 
    }

    // Setup Server Info
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons(PORT); 

    // Set socket to non-blocking.
    long sockfd_flags = fcntl(attempted_sockfd, F_GETFL, NULL);
    if(sockfd_flags < 0) {
        fprintf(stderr, "Error obtaining sockfd flags: %s\n", strerror(errno));
        return FAILED_CONN;
    }
    sockfd_flags |= O_NONBLOCK; 
    if(fcntl(attempted_sockfd, F_SETFL, sockfd_flags) < 0) { 
        fprintf(stderr, "Error setting sockfd flags: %s\n", strerror(errno)); 
        return FAILED_CONN;
    }

    // Connect the client socket to server socket 
    if (connect(attempted_sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0) { 
        if (errno == EINPROGRESS) {
            // Set timeout struct for select.
            fd_set fdset;
            struct timeval tv;
            FD_ZERO(&fdset);
            FD_SET(attempted_sockfd, &fdset);
            tv.tv_sec = 2;             /* 2 second timeout */
            tv.tv_usec = 0;

            int retcode = select(attempted_sockfd+1, NULL, &fdset, NULL, &tv);
            if(retcode < 0 && errno != EINTR) {
                fprintf(stderr, "Error connecting to server: %s\n", strerror(errno));

                // Close failed socket
                close(attempted_sockfd);
                return FAILED_CONN;
            }
            else if (retcode > 0) { 
                // Socket selected for write 
                socklen_t lon = sizeof(int);
                int valopt;
                if (getsockopt(attempted_sockfd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) {
                    fprintf(stderr, "Error in getsockopt: %s\n", strerror(errno));

                    // Close failed socket
                    close(attempted_sockfd);
                    return FAILED_CONN;
                }
                // Check the value returned... 
                if (valopt != 0) { 
                    fprintf(stderr, "Error in delayed connection: %s\n", strerror(valopt)); 

                    // Close failed socket
                    close(attempted_sockfd);
                    return FAILED_CONN;
                } 
           }
           else {
               fprintf(stderr, "Select timed out\n"); 

                // Close failed socket
                close(attempted_sockfd);
                return FAILED_CONN;
           }
        }
    }

    // Set back to blocking mode
    sockfd_flags = fcntl(attempted_sockfd, F_GETFL, NULL);
    if(sockfd_flags < 0) { 
        fprintf(stderr, "Error obtaining sockfd flags: %s\n", strerror(errno)); 
        exit(0); 
    } 
    sockfd_flags &= (~O_NONBLOCK); 
    if(fcntl(attempted_sockfd, F_SETFL, sockfd_flags) < 0) { 
        fprintf(stderr, "Error setting sockfd flags: %s\n", strerror(errno)); 
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
    int sockfd = *(int *)arg;

    // Don't allow this thread to handle these
    mask_sigtstp();
    mask_sigterm();

    // Strings
    char msg[BUFFER_MAX];
    char MAC_ADDR_PATH[BUFFER_MAX];
    char MAC_ADDR[18];
    char interface[6] = "wlan0";
    
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
    sprintf(msg, "hwstart,%s", MAC_ADDR);
    fprintf(stdout, "Token sent: %s\n", msg);

    // Deliver MAC_ADDR to server
    write(sockfd, msg, sizeof(msg));
    
    while (!program_end) { 
        if(server_connected == 1){
            // Give server a chance to read and send messages
            delay(sec_delay);

            // Read data from server
            bzero(msg, sizeof(msg));
            read(sockfd, msg, sizeof(msg));
            fprintf(stdout, "Server Returned: %s\n", msg);

            if((strncmp(msg, "success", 7)) == 0) { 
                fprintf(stdout, "client added successfully!");

                // Allow hardware thread to write out success
                bzero(display_msg, sizeof(display_msg));
                sprintf(display_msg, "success");
                msg_to_display = 1;
            }
            else {
                // Calculate row and col for token
                pthread_mutex_lock(&lock);
                int row = row_pos_tens*10 + row_pos_ones;
                int col = col_pos_tens*10 + col_pos_ones;
                pthread_mutex_unlock(&lock);

                // Format string for delivery
                bzero(msg, sizeof(msg));
                sprintf(msg, "position,%d.%d", row, col);
                write(sockfd, msg, sizeof(msg));
                fprintf(stdout, "Token sent: %s\n", msg);
            }
        } else{
            // Reconnect to server.
            while(attempt_connection(&sockfd)) {
                // Check if program is still running.
                if(program_end) {
                    fprintf(stdout, "Network thread exiting...\n");
                    return NULL;
                }
            }
            bzero(msg, sizeof(msg));
            sprintf(msg, "hwstart,%s", MAC_ADDR);
            write(sockfd, msg, sizeof(msg));
        }
    }

    fprintf(stdout, "Network thread exiting...\n");
    return NULL;
} 
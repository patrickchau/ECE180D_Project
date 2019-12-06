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

// Allows for error codes
#include <errno.h>

// For Open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int server_connected = 0;

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
        server_connected = 1;
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
        if(server_connected == 1){
            // Give server a chance to read and send messages
            delay(1000);

            // Read data from server
            bzero(msg, sizeof(msg));
            read(sockfd, msg, sizeof(msg));
            fprintf(stdout, "Server Returned: %s\n", msg);

            if((strncmp(msg, "position", 8)) != 0) { 
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
        } else{
            // Reconnect to server.
            while(attempt_connection(&sockfd));
            bzero(msg, sizeof(msg));
            sprintf(msg, "start,%s", MAC_ADDR);
            write(sockfd, msg, sizeof(msg));
        }
    }

    return NULL;
} 
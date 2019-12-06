/*************************************************************************/
/*                          signal_handler.c                             */
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

#include "hardware.h"
#include "signal_handler.h"
#include "globals.h"

#include<pthread.h>

// STD libraries
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <netdb.h> 
#include <string.h>
#include <errno.h>

void mask_sigpipe() {
	sigset_t mask;
	sigemptyset(&mask); 
    sigaddset(&mask, SIGPIPE);
                
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
}

void mask_sigterm() {
	sigset_t mask;
	sigemptyset(&mask); 
    sigaddset(&mask, SIGTERM); 
                
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
}

void sigterm_handler(int signo)  {
  
    // Remove compiler warnings on unused variables.
    (void)signo;

    fprintf(stdout, "SIGINT Occured! Closing Program.\n");
    clear_pins();
    _Exit(0);

}

void sigpipe_handler(int signo)  {
    // Remove compiler warnings on unused variables.
    (void)signo;
    //Set server as disconnected
    server_connected = 0;
    fprintf(stdout, "SIGPIPE OCCURED! Reattempting server communication.\n");
}

void set_sig_handlers() {

        if (signal(SIGINT, sigterm_handler) == SIG_ERR) { 
            fprintf(stderr, "sigaction failed with error: %s\n", strerror(errno));
        }

        if (signal(SIGPIPE, sigpipe_handler) == SIG_ERR) { 
            fprintf(stderr, "sigaction failed with error: %s\n", strerror(errno));
        }
}
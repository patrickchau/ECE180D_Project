/*************************************************************************/
/*                          signal_handler.h                             */
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

#ifndef SIGNAL_HANDLER_DEFINED
#define SIGNAL_HANDLER_DEFINED

// Signal headers
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h> 

/******************************************************
 * Functions: mask_signal
 *-----------------------------------------------------
 * Masks signal sent to the thread that calls this
 * function
 *-----------------------------------------------------
 * Return Values:
 * None.
 * ----------------------------------------------------
 * Inputs:
 * None.
******************************************************/
void mask_sigterm();
void mask_sigtstp();
void mask_sigpipe();

/******************************************************
 * Functions: signal_handler
 *-----------------------------------------------------
 * Functions called by POSIX signal library to handle
 * the respective signal. 
 *-----------------------------------------------------
 * Return Values:
 * None.
 * ----------------------------------------------------
 * Inputs:
 * None.
******************************************************/
void sigterm_handler(int signo);
void sigtstp_handler(int signo);
void sigpipe_handler(int signo);

/******************************************************
 * Functions: set_sig_handlers
 *-----------------------------------------------------
 * Sets function pointers to the signal_handler 
 * corresponding to the signal needing to be handled
 *-----------------------------------------------------
 * Return Values:
 * None.
 * ----------------------------------------------------
 * Inputs:
 * None.
******************************************************/
void set_sig_handlers();

#endif //SIGNAL_HANDLER_DEFINED
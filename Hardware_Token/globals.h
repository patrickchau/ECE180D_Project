/*************************************************************************/
/*                             globals.h                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                          Hardware Token                               */
/*           https://github.com/patrickchau/ECE180D_Project              */
/*************************************************************************/
/*                Copyright  12-5-2019 Joseph Miller.                   */
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

#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED

// Multithreading
#include <pthread.h> 

/*****************************************************
 * Global Constants
*****************************************************/

// Magic Numbers
#define FOREVER 1
#define usec_delay 1 
#define ten_usec_delay 10
#define hun_usec_delay 100
#define msec_delay 1
#define ten_msec_delay 10
#define hun_msec_delay 100
#define sec_delay 1000

/*****************************************************
 * Modifiable Globals
*****************************************************/

// Threads
extern pthread_mutex_t lock;
extern int program_end;

// Rows and Cols
extern int row_pos_ones;
extern int row_pos_tens;
extern int col_pos_ones;
extern int col_pos_tens;

// Server
extern int server_connected;
extern int msg_to_display;
extern char display_msg[20];

#endif //GLOBALS_DEFINED
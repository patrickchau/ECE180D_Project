/*************************************************************************/
/*                             network.h                                 */
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

#ifndef NETWORK_DEFINED
#define NETWORK_DEFINED

/*****************************************************
 * Global Constants
*****************************************************/
// Return Codes
#define SUCCESSFUL_CONN 0
#define FAILED_CONN 1

// Socket Params
#define BUFFER_MAX 128 
#define PORT 8080 
#define SERVER_IP "192.168.43.190"
#define SA struct sockaddr

/******************************************************
 * Function: attempt_connection
 *-----------------------------------------------------
 * Attempts to generate a socket fd to the server
 *-----------------------------------------------------
 * Return Values:
 * SUCCESSFUL_CONN is returned when connection succeeds
 * FAILED_CONN is returned when connection fails
 * ----------------------------------------------------
 * Inputs:
 * sockfd will be set to the correct value for the
 * network file descriptor on success. On fail will be
 * set to NULL.
******************************************************/
int attempt_connection(int* sockfd);

/******************************************************
 * Function: server_communication
 *-----------------------------------------------------
 * Continuously runs as long as server is connected to
 *-----------------------------------------------------
 * Return Values:
 * None
 * ----------------------------------------------------
 * Inputs:
 * Casts void* provided from pthread to int*. Should be
 * a valid file descriptor for the network.
******************************************************/
void* server_communication(void* sockfd);

#endif //NETWORK_DEFINED
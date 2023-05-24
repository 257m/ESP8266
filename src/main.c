/// Proprietary SDK includes
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"
#include "cont.h"

/// custom function for the web server
#include "serial_io.h"

/* 
*	The SDK calls user_init as the entry point function for the user
*	The SDK runs a watchdog timer on the function and we don't return in
*	around 3.2 seconds we get reset (not good) so we have to be quick
*/
void user_init()
{
	uart_init(115200);

	/*
	*	Since we only have 3.2 seconds you might wonder how we
	*	are going to run an entire program in the that time frame.
	*	Not even mentioning the fact that we have to have a constantly
	*	running web server. Well lucky user_init is not the only function
	*	of ours the SDK will call. user_init is only to setup the actual
	*	program. The SDK has a function that we can pass a function pointer
	*	to and it will call that function in a loop. Very similar to the arduino
	*	loop. Here is the function:
	*	bool ets_task(ETSTask task, uint8 prio, ETSEvent *queue, uint8 qlen);
	*/
}
/// Proprietary SDK includes
#include "ets_sys.h"
#include "espconn.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"

/// Custom libraries for uart_init, printf, gets, etc...
#include "uart_io.h"
#include "serial_io.h"
#include "web_server.h"

// Is used for the ets_task which is explained inside of user_init
#define LOOP_TASK_PRIORITY 1
#define LOOP_QUEUE_LENGTH 1
// Honestly probably never gonna to use this queue but I don't know if
// the sdk even accesses the pointer I pass. Better safe than sorry
static ETSEvent loop_queue[LOOP_QUEUE_LENGTH];

static void loop_task(ETSEvent* events)
{
	/*
	*	Non web server code will go in here.
	*	Sensors readers, pin flashers, etc...
	*	It will loop continually
	*/
	printf("Hello from loop\n");
}

#define TIMER_ON 1
#define TIMER_INTERVAL 500
#define TIMER_ARGS NULL

#if TIMER_ON

#define ONCE 0
#define REPEAT 1

os_timer_t timer;

void ICACHE_FLASH_ATTR timer_func(void *arg)
{
	/*
	*	Any repetitive code you need run at a constant interval.
	*	For example blink sketch that turns on and off GPIO.
	*	There can be a potiental race condition with the loop.
	*	Be careful.
	*/
	if (GPIO_REG_READ(GPIO_IN_ADDRESS) & 0x2)
		// Clear pin
		GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 0x2);
	else
		// Set pin
		GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, 0x2);
}

#endif /* TIMER_ON */

void init_done()
{
	// configure IO2 as output
	GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, BIT2);
	web_server_init();
}

/* 
*	The SDK calls user_init as the entry point function for the user
*	The SDK runs a watchdog timer on the function and if we don't return
*	in around 3.2 seconds we get reset (not good) so we have to be quick.
*	
*	Technically I lied user_init is not the closest accesible entry point
*	for the user. That would be call_user_start but I can't touch that if
*	I want to keep Wifi working because it does some secret SDK stuff that
*	I can't hope to possibly replicate given the current resources at hand.
*	(Proprietary software/hardware is annoying)
*/
void user_init()
{
	// Initalize uart and set baud rate by setting the uart clock divider
	uart_init(9600);
	serial_write("Initialized UART\n");

	#if TIMER_ON
	os_timer_setfn(&timer, timer_func, TIMER_ARGS);
	os_timer_arm(&timer, TIMER_INTERVAL, REPEAT); // TIMER_INTERVAL in ms, repeating
	#endif /* TIMER_ON */

	/*
	*	Since we only have 3.2 seconds you might wonder how we
	*	are going to run an entire program in the that time frame.
	*	Not even mentioning the fact that we have to have a constantly
	*	running web server. Well luckily user_init is not the only function
	*	of ours the SDK will call. user_init is only to setup the actual
	*	program. The SDK has a function that we can pass a function pointer
	*	to and it will call that function in a loop. Very similar to the arduino
	*	loop. 
	*	Here is the function:
	*	bool ets_task(ETSTask task, uint8 prio, ETSEvent *queue, uint8 qlen);
	*	
	*	Here are the definitions for some the data structures:
	*	// ETSSignal and ETSParam are both unsigned 32 bit integers
	*	typedef struct {
	*		ETSSignal sig;
	*		ETSParam  par;
	*	} ETSEvent;
	*
	*	// ETSTask is just function pointer that pointer to a function
	*	// that takes in a pointer to a ETSEvent (essentially a method with no args)
	*	typedef void (*ETSTask)(ETSEvent *e);
	*
	*	In the ets_task function:
	*	The ETSTask task argument is the function that is going to be
	*	called in a loop by the SDK.
	*	
	*	The uint8 prio is the priority that the function will have
	*	over other loops
	*	
	*	ETSEvent* queue is the queue pointer that is going to be
	*	passed to ETSTask task function in a loop
	*
	*	uint8 qlen is the size/length of the queue
	*/

	ets_task(loop_task, LOOP_TASK_PRIORITY, loop_queue, LOOP_QUEUE_LENGTH);

	system_init_done_cb(&init_done);
}

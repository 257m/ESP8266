/// Proprietary SDK includes
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"

/// Custom libraries for uart_init, printf, gets, etc...
#include "uart_io.h"
#include "serial_io.h"

// No gurantee on my part that this function will run before the loop
static void setup()
{
	printf("Hello from setup\n");
}

// Is used for the ets_loop_task which is explained inside of user_init
#define LOOP_TASK_PRIORITY 1
#define LOOP_QUEUE_LENGTH 1
// Honestly probably never gonna to use this queue but I don't know if
// the sdk even accesses the pointer I pass. Better safe than sorry
static ETSEvent loop_queue[LOOP_QUEUE_LENGTH];

static void loop_task(ETSEvent* events)
{
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
	*	There can be potiental race condition with the loop.
	*	Be careful.
	*/
	printf("Hello from timer\n");
}

#endif /* TIMER_ON */

/* 
*	The SDK calls user_init as the entry point function for the user
*	The SDK runs a watchdog timer on the function and we don't return in
*	around 3.2 seconds we get reset (not good) so we have to be quick
*/
void user_init()
{
	// Initalize uart and set baud rate by setting the uart clock divider
	uart_init(9600);

	#if TIMER_ON
	os_timer_setfn(&timer, timer_func, TIMER_ARGS);
	os_timer_arm(&timer, TIMER_INTERVAL, REPEAT); // TIMER_INTERVAL in ms, repeating
	#endif /* TIMER_ON */

	/*
	*	Since we only have 3.2 seconds you might wonder how we
	*	are going to run an entire program in the that time frame.
	*	Not even mentioning the fact that we have to have a constantly
	*	running web server. Well lucky user_init is not the only function
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

	ets_loop_task(loop_task, LOOP_TASK_PRIORITY, loop_queue, LOOP_QUEUE_LENGTH);

	system_init_done_cb(&setup);
}
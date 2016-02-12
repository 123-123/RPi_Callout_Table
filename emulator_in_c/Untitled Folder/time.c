//
// time library
//

#include "time.h"


static rpi_sys_timer_t* rpiSystemTimer = (rpi_sys_timer_t*)RPI_SYSTIMER_BASE;

rpi_sys_timer_t* RPI_GetSystemTimer(void)
{
    return rpiSystemTimer;
}

void RPI_WaitMicroSeconds( unsigned int us )
{
    volatile unsigned int ts = rpiSystemTimer->counter_lo;

    while( ( rpiSystemTimer->counter_lo - ts ) < us )
    {
 
    }
}


unsigned int
gettime( )
{

 volatile unsigned int a = rpiSystemTimer->counter_lo;
return a;

}


unsigned int
timediff( unsigned int now, unsigned int then )
{
	// your code goes here
	unsigned int diff;
	diff = now - then;
	return diff;
}

void
wait( unsigned int time )
{
//	unsigned int a,b;
//	a=GET32(READTIMER);
//	do{
//		b=GET32(READTIMER);
//	}while((b-a)<=time);

RPI_WaitMicroSeconds(MILLION*time); //secs
	
}



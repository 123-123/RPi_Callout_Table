
#include "os.h"
#include <stdio.h>

void
flash_led ( unsigned int time, unsigned int colors, unsigned int reps )
{

    while(reps--)
    {
		sleep(time);
		if (colors & GRN) printf("\nBlink GREEN");
		if (colors & RED) printf("\nBlink RED");
		sleep(time);
    }

    return;
}

void
flash_lonum( unsigned int num )
{
	if (num == 0) {
		flash_led(1, GRN, 2);
		flash_led(1, RED, 2);
	} else if (num == 1) {
		flash_led(1, GRN, 4);
	} else if (num == 2) {
		flash_led(1, RED, 4);
	} else if (num == 3) {
		flash_led(1, RED | GRN, 4);
	} else {
		flash_led(3, RED | GRN, 4);
	}
}

void
blink_led(unsigned int color)
{
	flash_led( 1, color, 1 );
	return;
}

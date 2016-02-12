//
// time.h
//
// information regarding the time library
//
//#define	ONE_SECOND	0x2000		// you define this however you want/need
#define ONE_SECOND 0x01

#define MILLION 1000000
#define PERIPHERAL_BASE     0x3F000000UL                       // section 7.5 BCM2835 doc.
#define RPI_SYSTIMER_BASE       ( PERIPHERAL_BASE + 0x3000 )
#define RPI_INTERRUPT_CONTROLLER_BASE   ( PERIPHERAL_BASE + 0xB200 )


extern unsigned int timediff( unsigned int now, unsigned int then );
extern void wait( unsigned int time );
extern unsigned int gettime( void );



typedef struct {
    volatile unsigned int control_status;
    volatile unsigned int counter_lo;
    volatile unsigned int counter_hi;
    volatile unsigned int compare0;
    volatile unsigned int compare1;
    volatile unsigned int compare2;
    volatile unsigned int compare3;
    } rpi_sys_timer_t;


extern rpi_sys_timer_t* RPI_GetSystemTimer(void);
extern void RPI_WaitMicroSeconds( unsigned int us );

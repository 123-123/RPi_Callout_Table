//
// timeout queue
//

#include "os.h"
#include "llist.h"
#include "time.h"

struct event {
	LL_PTRS;
	int timeout;
	int repeat_interval;
	pfv_t go;
	unsigned int data;
};

#define MAX_EVENTS	8
struct event queue[ MAX_EVENTS ];

// list anchors -- important, but ignore them; they are never used directly
llobject_t TQ;
llobject_t FL;

struct event *timeoutq;
struct event *freelist;

unsigned int then_usec;

unsigned int now_usec( void )
{
	unsigned int us;
	us=gettime();  //from "time.h"
	return us;
}

// sets up concept of local time
// initializes the timeoutq and freelist
// fills the freelist with entries
// timeoutq is empty
//
void
init_timeoutq()
{
	int i;

	then_usec = now_usec();

	timeoutq = (struct event *)&TQ;
	LL_INIT(timeoutq);
	freelist = (struct event *)&FL;
	LL_INIT(freelist);

	for (i=0; i<MAX_EVENTS; i++) {
		struct event *ep = &queue[i];
		LL_PUSH(freelist, ep);
	}

	return;
}


//
// account for however much time has elapsed since last update
// return timeout or MAX
//
int
bring_timeoutq_current()
{

	unsigned int now;
	struct event *ep;  
	ep = (struct event *)LL_HEAD(timeoutq); //ep now points to the head of TimeoutQ
	if(!LL_IS_EMPTY(ep))
	{
		now = now_usec();
		ep->timeout -= (now-then_usec); 
		return (ep->timeout);
	}
	else
		return 0;
}


//
// get a new event structure from the freelist,
// initialize it, and insert it into the timeoutq
// 
void
create_timeoutq_event(int timeout, int repeat, pfv_t function, unsigned int data)
{
	struct event *x;
	struct event *tempptr;
	
	x = (struct event*)LL_POP(freelist);
	x->timeout         = timeout;	
	x->repeat_interval = repeat;
	x->go		   = function;
	x->data		   = data;	 

	if(LL_IS_EMPTY(timeoutq))
		LL_PUSH(timeoutq, x); //x=timeoutq=head of list
	//insert it correct place and update timout 
	else
	LL_EACH(timeoutq,tempptr,struct event)
	{
		if(x->timeout <= tempptr->timeout)
		{
			tempptr->timeout -= x->timeout;  
			LL_L_INSERT(tempptr,x);
			break;
		}
		else
		{	
			if(LL_NEXT(timeoutq,tempptr) == NULL)
			{
				x->timeout -= tempptr->timeout;			
				LL_R_INSERT(tempptr,x);
				break;
			}
			else
			{
				x->timeout -= tempptr->timeout;			
				continue; //go to the next node 	
			}
		}
	}
	//update then_usec
	then_usec = now_usec() - then_usec; 
}



//
// bring the time current
// check the list to see if anything expired
// if so, do it - and either return it to the freelist 
// or put it back into the timeoutq
//
// return whether or not you handled anything
//
int
handle_timeoutq_event( )
{
	struct event *ep;  
	//struct event *head_timeoutq;
	ep = (struct event *)LL_HEAD(timeoutq); //ep now points to the head of TimeoutQ
	 
	//if diff < 1 us then execute the function
	if(ep->timeout - then_usec < 1 )
	{
		//execute function
		pfv_t tmp = ep->go;
		(* tmp)(ep->data);

		//remove from the timeoutq and reinsert back to freelist
		LL_DETACH(timeoutq, ep);

		if(ep->repeat_interval != 0)
		{
			create_timeoutq_event( ep->repeat_interval, ep->repeat_interval, ep->go, ep->data);	
			//update then_usec
			then_usec -= now_usec();
		}
		else
		{
			//push to freelist
			LL_PUSH(freelist,ep);
		}
		return 1;
	}
	else	
	return 0;
}

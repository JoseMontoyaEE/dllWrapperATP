#include <stdlib.h>
/* #include <lib.h> */

/*---------------------------------------------------------------*/
/* These functions would be declared in a model as follows:      */
/*    FUNCTION seed FOREIGN C_SEED {ixarg: 1}                    */
/*    FUNCTION rand FOREIGN C_RANDOM {ixarg: 1}                  */
/*  and the function names would need to be registered           */
/*  in subroutine fgnfun of ATP's file mod001.for                */
/*---------------------------------------------------------------*/

double c_seed__(void)   /* randomly seeds the C library function rand() */
{
/* { date_time_seed(); */
  srand(time(0));
  return 0;
}

double c_random__(void)   /* returns a random number between 0 and 1 */
{ return (double)rand()/(double)RAND_MAX;
}

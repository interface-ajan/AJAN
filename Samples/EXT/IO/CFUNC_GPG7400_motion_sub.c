/*! CFUNC_GPG7400_motion.AJN の C言語サブルーチン部
*/
#include <stdio.h>

static int gCall = 0;

/*---------------------------------------------------------------------------*/
/*  Function void ControlBoard(int nDevice);                                 */
/*---------------------------------------------------------------------------*/
void c_func(unsigned long event, unsigned long user)
{
  if(event & 0x01) gCall++;
}

void c_set_gCall(int v)
{
	gCall = v;
	printf("set_gCall %d\n", gCall);
}

int c_get_gCall(void)
{
	printf("get_gCall %d\n", gCall);
	return gCall;
}


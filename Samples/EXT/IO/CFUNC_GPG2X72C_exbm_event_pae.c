/*! CFUNC_GPG2X72C_exbm_event_pae.AJN の C言語サブルーチン部
*/
#include <stdio.h>

static int flag = 0;

void c_callback(unsigned long dwEvent, unsigned long dwUser)
{
	flag = 1;
}

void c_set_flag(int v)
{
	flag = v;
	printf("set_flag %d\n", flag);
}

int c_get_flag(void)
{
	printf("get_flag %d\n", flag);
	return flag;
}


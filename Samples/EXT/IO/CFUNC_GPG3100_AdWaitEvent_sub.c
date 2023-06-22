/*! CFUNC_GPG3100_AdWaitEvent.AJN の C言語サブルーチン部
*/
#include <stdio.h>

static int		flag;

void c_event_proc(int dummy)
{
	printf("サンプリング完了!!\n");
	flag = 0;
}

void c_set_flag(int v)
{
	flag = v;
}

int c_get_flag(void)
{
	return flag;
}


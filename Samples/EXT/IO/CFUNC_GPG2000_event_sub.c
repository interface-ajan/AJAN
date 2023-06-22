/*! CFUNC_GPG2000_event.AJN の C言語サブルーチン部
*/
#include <stdio.h>
#include "fbidio.h"

static int		gnflag;

void c_CallBackProc(unsigned long lUserData, unsigned char bEvent, unsigned short nDeviceNum)
{
	unsigned char bData;
	unsigned long dummy;
	dummy = lUserData;

	switch(bEvent){
	case 0x01:
		DioInputByte ( nDeviceNum, FBIDIO_IN1_8, &bData);
		/* Assert ACK1 */
		DioSetAckPulseCommand ( nDeviceNum, 0x80);
		/* Display the data */
		printf("bData: %Xh\n",bData);
		gnflag = 1;
		break;
	default:
		break;
	}
}

void c_set_gnflag(int v)
{
	gnflag = v;
printf("set_flag %d\n", gnflag);
}

int c_get_gnflag(void)
{
printf("get_flag %d\n", gnflag);
	return gnflag;
}


/*! CFUNC_GPG4116_FrameSendRecv.AJN の C言語サブルーチン部
*/
#include <stdio.h>
#include <string.h>
#include "pcihdlc.h"

static char	ReceiveBuffer[MAX_FRAME_LENGTH];
static int	ReceivedCount = 0;

void c_ReceiveHandler(unsigned long EventMask, unsigned long UserData)
{
	unsigned long FrameCount;
	unsigned long FrameLength;
	unsigned long i;
	int nPort;


	printf("callback %#lx %ld\n", EventMask, UserData);
	if(EventMask != 1) return;
	
	nPort = (int)UserData;
	HdlcGetFrameCount(nPort, &FrameCount);
	for(i = 0; i < FrameCount; i++) {
		HdlcReceiveFrame(nPort, ReceiveBuffer, &FrameLength);
		// Output the first 50 bytes of receive frames
		if(FrameLength > 50) {
			ReceiveBuffer[50] = '\0';
		} else {
			ReceiveBuffer[FrameLength] = '\0';
		}
		printf("receive data=%s (size=%ld)\n", ReceiveBuffer, FrameLength);
		ReceivedCount++;
	}
}

void c_set_cnt(int v)
{
	ReceivedCount = v;
}

int c_get_cnt(void)
{
	return ReceivedCount;
}


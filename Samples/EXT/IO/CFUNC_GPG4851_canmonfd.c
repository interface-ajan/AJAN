/*! CFUNC_GPG4851_canmodfd.AJN の C言語サブルーチン部
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "ifcan.h"
//#include "ifcanpf.h"

int							c_exitflag = 1;
static struct sigaction	act;
static int					hPort;

int c_get_exitflag()
{
	return c_exitflag;
}

static void sig_handler(int signal)
{
	c_exitflag = 0;
}

void c_sigaction_SIGQUIT()
{
	act.sa_handler = sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGINT, &act, 0);
}

// ハンドル値を、AJANからセットする用
void c_set_hPort(int i_port)
{
	hPort = i_port;
}

static unsigned char CanDlcToLength(unsigned long DLC)
{
	unsigned char Length;
	switch (DLC) {
	case CAN_MSG_LEN_12: Length = 12; break;
	case CAN_MSG_LEN_16: Length = 16; break;
	case CAN_MSG_LEN_20: Length = 20; break;
	case CAN_MSG_LEN_24: Length = 24; break;
	case CAN_MSG_LEN_32: Length = 32; break;
	case CAN_MSG_LEN_48: Length = 48; break;
	case CAN_MSG_LEN_64: Length = 64; break;
	default: Length = (unsigned char)DLC; break;
	}
	return Length;
}

// コールバック処理
void c_EventCallback(unsigned long Event, unsigned long dummy)
{
	unsigned long i, j;
	CAN_PORT_STATUS_FD Status;
	CAN_MESSAGE CanMsg;
	CAN_MESSAGE_FD CanFdMsg;
	unsigned long length;

	CanGetStatusFD(hPort, &Status);

	/* CAN messages */
	for(i = 0; i < Status.ulRXBCount; i++) {
		length = 1;
		CanReceiveMessage(hPort, &CanMsg, &length);
		printf("[CAN   ] time:%8ld  ID:%4lx  Len:%2lu  Data:",
			   CanMsg.ulTime, CanMsg.ulID, CanMsg.ulLength);
		for(j = 0; j < CanMsg.ulLength; j++) {
			printf("%02X ", CanMsg.bData[j]);
		}

		printf("\n");
	}

	/* CAN FD messages */
	for(i = 0; i < Status.ulRXBCountFD; i++) {
		length = 1;
		CanReceiveMessageFD(hPort, &CanFdMsg, &length);
		printf("[CAN FD] time:%8ld  ID:%4lx  Len:%2u  Data:",
			   CanFdMsg.ulTime, CanFdMsg.ulID, CanDlcToLength(CanFdMsg.ulLength));
		for(j = 0; j < CanDlcToLength(CanFdMsg.ulLength); j++) {
			printf("%02X ", CanFdMsg.bData[j]);
		}

		printf("\n");
	}

	return;
}

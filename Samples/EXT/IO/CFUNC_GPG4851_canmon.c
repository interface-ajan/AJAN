/*! CFUNC_GPG4851_canmod.AJN の C言語サブルーチン部
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "ifcan.h"
#include "ifcanpf.h"

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

// コールバック処理
void c_EventCallback(unsigned long Event, unsigned long dummy)
{
	unsigned long i, j;
	int ret;
	CAN_PORT_STATUS Status;
	CAN_MESSAGE CanMsg;
	unsigned long length;

	CanGetStatus(hPort, &Status);
	for(i = 0; i < Status.ulRXBCount; i++) {
		length = 1;
		CanReceiveMessage(hPort, &CanMsg, &length);
		printf("time:%8ld  ID:%4lx  Data:",
			   CanMsg.ulTime, CanMsg.ulID);
		for(j = 0; j < CanMsg.ulLength; j++) {
			printf("%02X ", CanMsg.bData[j]);
		}
		printf("\n");
		// プロファイル読み取りの処理は、説明の簡略化の為に省いています
	}
	
	return ;
}

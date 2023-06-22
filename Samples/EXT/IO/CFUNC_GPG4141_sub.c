/*! CFUNC_GPG4141_hd_send.AJN の C言語サブルーチン部
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <asm/ioctls.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "cp4141.h"

int							c_exitflag = 1;
static struct sigaction	act;

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
}

int c_open_cp4141port(int DeviceID, int SubsystemID, int BoardID, int Channel)
{
	CP4141_PORTINFO PortInfo;
	int index;
	int tempfd;
	char ttyname[256];

	for(index = 0; index < CP4141_MAX_PORTS; index++) {
		/* ttyname is /dev/ttyS[minornumber - 64] */
		sprintf(ttyname, "/dev/" CP4141_TTY_NAME "%d",
				index + CP4141_MINOR_START - 64);

		tempfd = open(ttyname, O_RDWR | O_NDELAY);
		if(tempfd < 0) continue;

		if(ioctl(tempfd, CP4141_GET_PORTINFO, &PortInfo) < 0) {
			close(tempfd);
			continue;
		}
		if(PortInfo.DeviceID == DeviceID &&
		   PortInfo.SubsystemID == SubsystemID &&
		   PortInfo.BoardID == BoardID &&
		   PortInfo.ChannelNumber == Channel) {

			printf("open %s port.\n", ttyname);
			printf("press CTRL-\\ to terminate program.\n");
			return tempfd;
		}
		close(tempfd);
	}
	return -1;
}
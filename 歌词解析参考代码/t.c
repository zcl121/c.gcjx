#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "date.h"

int main(int argc,char *argv[])
{
	int clock = 0;
	
	while(1){
		usleep(1000*500);
		clock++;
		//printf("%02d:%02d\r",clock/60,clock%60);
		GetDateTime(NULL);
		fflush(NULL);
	}
	return 0;
}










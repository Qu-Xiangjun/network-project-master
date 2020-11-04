/**
 * Name         : sender.c 
 * Author       : Qu Xiangjun
 * Version      : 2.0
 * Date         : 2020-10-8
 * Description  : Ethernet frame receiver test
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
# include "receiver.h"

/* welcome print information */
void welcome()
{
	printf("[INFO] Usage: frame receiver process.\n");
	printf("[INFO] My address  : ");
	output_octal(my_addr,6);
}

int main(){
	welcome();
	receive_frame(frame, sta_addr, des_addr, protocol, payload, my_addr);
}

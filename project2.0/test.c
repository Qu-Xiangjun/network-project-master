#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "datalink_sender.h"
unsigned char data[] = "Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. ";


int main(int argc, char **argv)
{	
	unsigned char ttl;
	unsigned int ttl_temp = 1500;
	ttl = ttl_temp;
	printf("%0x\n",ttl_temp);
	printf("%0x",ttl);
	return 0;
}


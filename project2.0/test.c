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
    /* example1: success */
    unsigned char temp[1500];
    memcpy(temp,&data[0],5);
    printf("%s",temp);
	return 0;
}


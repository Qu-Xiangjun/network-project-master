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
#include "datalink_receiver.h"

/* welcome print information */
void receiver_welcome(){
    printf("[INFO] Usage: receiver process.\n");
	printf("\n");
}

int main(){
	receiver_welcome();
	unsigned char* message = receive_frame();
	printf("receive message: %s",message);
}

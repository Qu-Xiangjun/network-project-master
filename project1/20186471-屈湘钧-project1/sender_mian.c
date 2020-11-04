/**
 * Name         : sender_mian.c 
 * Author       : Qu Xiangjun
 * Version      : 2.0
 * Date         : 2020-10-8
 * Description  : load ethernet frame sender function from sender.h and test
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "sender.h"

unsigned char frame[1546]; /* frame array */
unsigned char data[] = "Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. ";
unsigned char sta_addr[6] = {0x11, 0x33, 0x55, 0x44, 0x22, 0x55}; /* start address */
unsigned char des_addr[6] = {0x42, 0x23, 0x52, 0xb5, 0xa6, 0xc3}; /* destination address */
unsigned short protocol = 1;

/* welcome print information */
void welcome(){
    printf("[INFO] Usage: frame sender process.\n");
}

int main(){
    unsigned int length;

    welcome();

    /* example1: success */
    length = frame_construction(frame, sta_addr, des_addr, protocol, data, sizeof(data));
    send_frame(frame, length);
    
//    /* example2: failed */ 
//    length = frame_construction(frame,sta_addr,des_addr,protocol,data,sizeof(data));
//	frame[length-10] = ~frame[length-10];	/* Change payload information */ 
//    send_frame(frame,length);

    return 0;
}

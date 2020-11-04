/**
 * Name         : sender_mian.c 
 * Author       : Qu Xiangjun
 * Version      : 2.0
 * Date         : 2020-11-1
 * Description  :sender function from datalin_sender.h、 network_sender.h and test
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "network_sender.h"

/* datagram information */
unsigned char payload[] = "Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. Hello! This is a frame payload data. It's a computer project for quxiangjun 20186471. ";  //87Bytes
unsigned short payload_len = sizeof(payload);

/* welcome print information */
void welcome(){
    printf("[INFO] Usage: sender process.\n");
}

int main(){

    welcome();
    /* construct datagram */
    fragmentation_datagram_send(payload,payload_len);
    return 0;
}

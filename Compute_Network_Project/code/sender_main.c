/**
 * Name         : sender_main.c 
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
#include "UDP_sender.h"

/* welcome print information */
void welcome(){
    printf("[INFO] Usage: sender process.\n");
    printf("\n");
}

int main(){
    welcome();
    printf("Please input your message:");
    /* cin payload */
    int strSize = 20; //字符串能存储的最大长度
    int payload_len = 0; //字符串已用的长度
    char input  = NULL; //接收输入的字符
    unsigned char *payload = (unsigned char*)malloc(sizeof(char)*20);
    while((input=getc(stdin))!='\n') {
        payload[payload_len++] = input;
        if(payload_len%20==0) { //如果字符串已用长度等于分配的总长度-1时
            strSize+=20;    //更新数组能存储的最大长度
            payload =(unsigned char*)realloc(payload,strSize);
        }
    }
    payload[payload_len] = '\0';
    /* construct udp and send message*/
    udp_sender(payload,payload_len);
    printf("[INFO] message send successfully!\n");
    return 0;
}

/**
 * Name         : UDP_receiver.h 
 * Author       : Qu Xiangjun
 * Version      : 1.0
 * Date         : 2020-12-21
 * Description  : UDP datagram receive function
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network_receiver.h"

unsigned char sou_port[2]; /* source port 16bit*/
unsigned char des_port[2]; /* source port 16bit*/
unsigned char my_port[2] = {0x00, 0x02}; /* destination port 16bit*/
unsigned char UDP_protocol = '1'; /* udp portocol, 8bit */
unsigned short UDP_total_length = 0; /* udp total length, 16bit */
unsigned short UDP_checksum = 0; /* udp checksum for pseudo_header, 16bit */
unsigned char udp[65515]; /* The max length of udp is 2^16(ip total length) - 20(IP header) = 65515 byte*/
unsigned char data[65515-8]; /* the message's data */

unsigned char* udp_receiver(){
    /* 接收网络层传入临时字符串，包含不完整的pesudo header和udp */
    unsigned char *temp_data_ip_udp = analyse_datagram();

    /* 判断消息是否是空 */
    if(temp_data_ip_udp == NULL){
        return temp_data_ip_udp;
    }

    memcpy(&UDP_total_length, &temp_data_ip_udp[16],2);
    memcpy(&UDP_checksum, &temp_data_ip_udp[18],2);
    
    /* 校验checksum */
    temp_data_ip_udp[8] = 0x00;
    memcpy(&temp_data_ip_udp[9], &UDP_protocol,1);
    memcpy(&temp_data_ip_udp[10], &UDP_total_length,sizeof(UDP_total_length));
    unsigned short UDP_checksum_result = Checksum(temp_data_ip_udp,12+UDP_total_length);
    if(0 != UDP_checksum_result){  // checksum 校验失败
        printf("[Error] UDP checksum error!\n");
        printf("[Error] UDP's checksum : ");
        printf("%02x%02x\n", temp_data_ip_udp[14], temp_data_ip_udp[15]);
        printf("[Error] calculated checksum : ");
        printf("%04x\n",UDP_checksum_result);
        exit(1);
    }

    memcpy(&sou_port, &temp_data_ip_udp[12],2);
    memcpy(&des_port, &temp_data_ip_udp[14],2);
    memcpy(&data[0], &temp_data_ip_udp[20],UDP_total_length-8);

    /* 校验端口号 */
    if( (des_port[0]!=my_port[0]) && (des_port[1]!=my_port[1])){
        printf("[Error] Port check error!\n");
        printf("[Error] Destination port : ");
        output_octal(des_port, 2);
        printf("[Error] My port : ");
        output_octal(my_port, 2);
        exit(1);
    }

    /* print UDP datagram information */ 
    printf("******receive UDP information******\n");
    printf("Source port: ");
    output_octal(sou_port,2);
    printf("Destinamtion port  : ");
    output_octal(des_port,2);
    printf("data            : %s\n",data);
    printf("UDP_checksum    : ");
    printf("%04x\n",UDP_checksum_result);    
    printf("data length     : %d\n", UDP_total_length-8);
    printf("UDP total length: %d(bytes)\n", UDP_total_length);
    printf("**************************************\n");
    printf("[INFO] UDP receive success!\n");
    printf("\n");

    return data;
}

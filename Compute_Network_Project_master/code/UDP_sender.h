/**
 * Name         : UDP_sender.h 
 * Author       : Qu Xiangjun
 * Version      : 1.0
 * Date         : 2020-12-21
 * Description  : UDP datagram sender function
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network_sender.h"

unsigned char sou_port[2] = {0x00, 0x01}; /* source port 16bit*/
unsigned char des_port[2] = {0x00, 0x02}; /* destination port 16bit*/
unsigned char UDP_protocol = '1'; /* udp portocol, 8bit */
unsigned short UDP_total_length = 0; /* udp total length, 16bit */
unsigned short UDP_checksum = 0; /* udp checksum for pseudo_header, 16bit */
unsigned char udp[65515]; /* The max length of udp is 2^16(ip total length) - 20(IP header) = 65515 byte*/
unsigned char pesudo_header_udp[12+65515]; /* pesudo header for checksum, 12byte */

/**
 * udp datagram construct and send 
 * @data 数据
 * @data_len data长度
 */
void udp_sender(unsigned char *data,unsigned short data_len){
    /* data length check */
    if (data_len > 65515-8){
        printf("[Error] Payload length too long!\n");
        exit(1);
    }
    /* Construct data to be a UDP array */
    memcpy(&udp[0], sou_port, 2);
    memcpy(&udp[2], des_port, 6);
    UDP_total_length = data_len+8;
    memcpy(&udp[4], &UDP_total_length, sizeof(UDP_total_length));
    udp[6] = udp[7] = 0x00;
    memcpy(&udp[8], data, data_len);

    /* calculate UDP checksum for pesudo header */
    memcpy(&pesudo_header_udp[0], Source_address,4);
    memcpy(&pesudo_header_udp[4], Destination_address,4);
    pesudo_header_udp[8] = 0x00;
    memcpy(&pesudo_header_udp[9], &UDP_protocol,1);
    memcpy(&pesudo_header_udp[10], &UDP_total_length,sizeof(UDP_total_length));
    memcpy(&pesudo_header_udp[12], &udp[0],UDP_total_length);
    UDP_checksum = Checksum(pesudo_header_udp,12+UDP_total_length);  // calculate for checksum
    memcpy(&udp[6], &UDP_checksum, sizeof(UDP_checksum));

    /* get char array for cksum result, so that program can print it */
    unsigned char UDP_checksum_result[2];
    memcpy(UDP_checksum_result, &UDP_checksum, sizeof(UDP_checksum));

    /* print UDP datagram information */ 
    printf("******Send UDP information******\n");
    printf("Source port: ");
    output_octal(sou_port,2);
    printf("Destinamtion port  : ");
    output_octal(des_port,2);
    printf("data            : %s\n",data);
    printf("UDP_checksum    : ");
    output_octal(UDP_checksum_result, 2);    
    printf("data length     : %d\n", data_len);
    printf("UDP total length: %d(bytes)\n", UDP_total_length);
    printf("**************************************\n");
    printf("[INFO] UDP construct success!\n");
    printf("\n");

    /* construct datagram and send message*/
    fragmentation_datagram_send(udp,UDP_total_length);
}

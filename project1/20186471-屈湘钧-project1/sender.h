/**
 * Name         : sender.h 
 * Author       : Qu Xiangjun
 * Version      : 2.0
 * Date         : 2020-10-8
 * Description  : Ethernet frame sender function
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "crc32.h"

/**
 * print octal number 
 * @x octal array
 * @len octal array's length
 **/
void output_octal(unsigned char *x, unsigned int len){
    unsigned int i;
    for (i = 0; i < len; i++){
        printf("%02x", x[i]);
        if (i != len - 1)
            printf(" ");
    }
    printf("\n");
}

/**
 * frame construction function
 * @frame return frame 
 * @sta_addr start address
 * @des_addr destination address
 * @ protocol
 * @payload frame's data
 * @payload_len data's length
 **/
int frame_construction(unsigned char frame[1546], unsigned char *sta_addr, unsigned char *des_addr,
                       unsigned short protocol, unsigned char *payload, unsigned int payload_len){
    /* data length check */
    if (payload_len < 46)    {
        printf("[Error] Payload length too low!\n");
        exit(1);
    }else if (payload_len > 1500){
        printf("[Error] Payload length too long!\n");
        exit(1);
    }
    /* Construct data to be a frame array */
    memcpy(&frame[0], des_addr, 6);
    memcpy(&frame[6], sta_addr, 6);
    memcpy(&frame[12], &protocol, sizeof(protocol));
    memcpy(&frame[14], payload, payload_len);
    /* crc32: check frame sequence */
    init_crc_table(); /* initailize crc table */
    unsigned int crc32_result = crc32(frame, 14 + payload_len);
    memcpy(&frame[14 + payload_len], &crc32_result, sizeof(crc32_result));
    /* get char array for crc32 result, so that program can print it */
    unsigned char crc32_result_[4];
    memcpy(crc32_result_, &crc32_result, sizeof(crc32_result));

    /* frame length */
    unsigned int length = payload_len + 6 + 6 + sizeof(protocol) + sizeof(crc32_result);

    /* print frame information */ 
    printf("******Received frame information******\n");
    printf("Start address: ");
    output_octal(sta_addr,6);
    printf("Des address  : ");
    output_octal(des_addr,6);
    printf("protocol     : %d\n",protocol);
    printf("payload      : %s\n",payload);
    printf("CRC32        : ");
    output_octal(crc32_result_, 4);
    printf("Frame length : %d\n", length);
    printf("**************************************\n");
    printf("[INFO] Frame received success!\n");

    return length;
}

/**
 * frame send function
 * @frame  all information in frame
 * @frame_len frame's length  46
 **/
unsigned int send_frame(unsigned char *frame, unsigned int frame_len){
    FILE *file = fopen("pipe.bin", "wb+");
    if (file == NULL)    {
        printf("[INFO] File open error!\n");
        return 0;
    }
    fwrite(frame, sizeof(char), frame_len, file);
    fclose(file);
    printf("[INFO] Frame send success!\n");
    return 0;
}

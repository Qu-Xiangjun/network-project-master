/** 
 * Name         : crc16.h
 * Author       : Qu Xiangjun
 * Version      : 1.0
 * Date         : 2020-10-8
 * Description  :  CheckSum
 */
#include <stdio.h>
#include <stdlib.h>
#include "Output_Octal.h"
/**
 * First, set checsum result = 0
 * Second, every 2byte of buffer conbinate to a group. If odd byte in there, it will combinate with a 0x00.
 * Third, calculate every 16bit group into a 32 bit value
 * Fourth, add bigger 16bit and lower 16bit in to 16bit value
 * Last, filps all bit.
 * @buffer Calculated buffer string
 * @size  Calculated buffer string's length (bytes)
 * @return checksum 2Bytes
 **/
unsigned short Checksum(unsigned char* buffer, unsigned short size){
	// output_octal(buffer,size);
    unsigned int sum = 0;
    unsigned short cksum;
    unsigned char lower,higher;
    unsigned short temp;
    if(size%2 == 1){  // odd length combinate with a 0x00
        higher= buffer[size-1];
        lower = 0x00;
        temp = higher;
        temp = (temp << 8) + lower;
        sum += temp;
        size--;
    }
    while(size > 1){
        lower = buffer[size-1];
        higher = buffer[size - 2];
        temp = higher;
        temp = (temp << 8) + lower;
        // printf("%04x\n",temp);
        sum += temp;
        size -= 2;
    }
    
    cksum = (sum >> 16) + (sum & 0xffff);
    cksum = ~cksum;
    cksum = (cksum>>8) + (cksum << 8);
    // printf("%04x\n",cksum);
    return cksum;
}

//  int main(){
//      unsigned char data[] = {0x45,0x00,0x00,0x30,0x80,0x4c,0x40,0x00,0x80,0x06,0xb5,0x2e,0xd3,0x43,0x11,0x7b,0xcb,0x51,0x15,0x3d};
//      unsigned short ck = Checksum(data,sizeof(data));
//      printf("%04x",ck);
//  }

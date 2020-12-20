/** 
 * Name         : crc16.h
 * Author       : Qu Xiangjun
 * Version      : 1.0
 * Date         : 2020-10-8
 * Description  :  CheckSum
 */
#include <stdio.h>
#include <stdlib.h>
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
    unsigned int sum = 0;
    unsigned short cksum;
    unsigned char lower,higher;
    unsigned short temp;
    while(size > 1){
        lower = buffer[size-1];
        higher = buffer[size - 2];
        temp = higher;
        temp = (temp << 8) + lower;
        printf("%04x\n",temp);
        sum += temp;
        size -= 2;
    }
    if(size == 1){  // odd length combinate with a 0x00
        lower = buffer[size-1];
        higher = 0x00;
        temp = higher;
        temp = (temp << 8) + lower;
        sum += temp;
    }
    cksum = (sum >> 16) + (sum & 0xffff);
    cksum = ~cksum;
    printf("%04x\n",cksum);
    return cksum;
}

  int main(){
      unsigned char data[] = {0x47,0x00,0x87,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x4d,0x5f,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x02,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x00};
      unsigned short ck = Checksum(data,sizeof(data));
      printf("%04x",ck);
  }

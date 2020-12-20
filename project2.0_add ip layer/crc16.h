/** 
 * Name         : crc16.h
 * Author       : Qu Xiangjun
 * Version      : 1.0
 * Date         : 2020-10-8
 * Description  : CRC16 Checking
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define CRC_16_POLYNOMIALS            0x8005
/**
 * find crc32 value in crc32 table
 * @buffer Calculated buffer string
 * @size  Calculated buffer string's length
 * @return crc32 2Bytes
 **/
unsigned short CRC16(unsigned char* buffer, unsigned short size)
{
    unsigned char i, chChar;
    unsigned short wCRC = 0;
    while (size--)
    {
        chChar = *buffer++;        
        wCRC ^= (((unsigned short) chChar) << 8);
        for (i = 0; i < 8; i++)
        {
            if (wCRC & 0x8000)
                wCRC = (wCRC << 1) ^ CRC_16_POLYNOMIALS;
            else
                wCRC <<= 1;
        }
    }
    
    return wCRC;
}

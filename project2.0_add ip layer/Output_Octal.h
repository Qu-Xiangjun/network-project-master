/**
 * Name         : Output_Octal.h 
 * Author       : Qu Xiangjun
 * Version      : 1.0
 * Date         : 2020-11-1
 */
#ifndef _COMMON
#define _COMMON
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#endif

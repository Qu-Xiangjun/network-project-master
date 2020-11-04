/**
 * Name         : network_sender.h 
 * Author       : Qu Xiangjun
 * Version      : 2.0
 * Date         : 2020-11-1
 * Description  : network datagram sender function
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc16.h"
#include "output_octal.h"

int fragmention(){
    
}

/**
 * datagram construction function
 * @datagram 数据报数组，限制64kB长度，作为返回的数据报
 * @Version IPV版本号 4bit, 取char中的低四位
 * @Differentialted_Services 服务类型，1Byte
 * @Identification 识别号 2Bytes
 * @DF 不分片标识 1bit，取char中低一位
 * @MF 非末尾片标识 1bit，取char中低一位
 * @Fragment_offset 位偏移 13bit，取short中低13位
 * @TTL  生存期 1Byte
 * @Protocol 协议 1Byte
 * @Source_address 源地址 4bytes
 * @Destination_address 目的地址 4Bytes
 * @Option 可选项
 * @Option_len 可选项长度 （单位 行，一行4Bytes）
 * @payload 数据
 * @payload_len payload长度
 * @return length : datagram's length
 * 
 */
unsigned int datagram_construction(unsigned char datagram[65535],unsigned char Version,
                            unsigned char Differentialted_Services,unsigned short Identification,
                            unsigned short DF, unsigned short MF,
                            unsigned short Fragment_offset, unsigned char TTL, 
                            unsigned char Protocol,
                            unsigned char *Source_address, unsigned char *Destination_address,
                            unsigned char *Option,unsigned int Option_len,
                            unsigned *payload,unsigned short payload_len){
    /* data length check ,0--44KB*/
    if (payload_len > 65515){
        printf("[Error] Payload length too long!\n");
        exit(1);
    }
    /* Construct data to be a datagram array */
    /* 计算datagram header 长度 ,单位（行）*/
    unsigned char IHL = 5 + Option_len; // 取char中低四位,4bit
    printf("IHL%d\n",IHL);

    /* 组装Version IHL为一个Byets */
    unsigned char temp = 0x00;
    Version = Version << 4;  // 左移四位低位补0
    temp = temp | IHL; // 高四位全为0，低四位为IHL
    temp = temp | Version; // 高四位为Version，低四位为IHL
    memcpy(&datagram[0],&temp,1);

    memcpy(&datagram[1],&Differentialted_Services,1);

    /* datagram length */
    unsigned short Total_length = payload_len + IHL*4; // 总长= 首部行数*4bytes + payload长度
    memcpy(&datagram[2],&Total_length,2);
    memcpy(&datagram[4],&Identification,2);

    /* 组装DF MF Fragment为一个Byets */
    unsigned short temp2;
    short df = DF;
    df = df << 14;
    df = df & 0x4000;  // 保证除第15位必须为0
    short mf = MF;
    mf = mf << 13;
    mf = mf & 0x2000; // 保证除第14位必须为0
    Fragment_offset = Fragment_offset & 0x1FFF; // 取出低13位，其他全为0
    temp2 = df | mf | Fragment_offset;
    memcpy(&datagram[6],&temp2,2);

    memcpy(&datagram[8],&TTL,1);
    memcpy(&datagram[9],&Protocol,1);
    memcpy(&datagram[12],Source_address,4);
    memcpy(&datagram[16],Destination_address,4);
    memcpy(&datagram[20],Option,Option_len*4);
    
    /* 对Header进行checksum */
    unsigned char Header[IHL * 4 - 2]; // 去除checksum后的头大小
    memcpy(&Header[0],&datagram[0],10);
	memcpy(&Header[10],&datagram[12],8 + Option_len * 4);
    unsigned short Header_checksum = CRC16(Header,IHL * 4 - 2);
    memcpy(&datagram[10],&Header_checksum,2); 
    /* get char array for crc16 result, so that program can print it */
    unsigned char crc16_result[2];
    memcpy(crc16_result, &Header_checksum, sizeof(Header_checksum));

    memcpy(&datagram[20 + Option_len*4],payload,payload_len);

    /* print datagram information */ 
    printf("******Send datagrame information******\n");
    printf("Source address: ");
    output_octal(Source_address,4);
    printf("Destinamtion address  : ");
    output_octal(Destination_address,4);
    printf("payload      : %s\n",payload);
    printf("CRC16        : ");
    output_octal(crc16_result, 2);    
    printf("payload length  : %d\n", payload_len);
    printf("Header length   : %d(row*4bytes)\n", IHL);
    printf("datagram length : %d\n", Total_length);
    printf("**************************************\n");
    printf("[INFO] datagram send success!\n");
    return Total_length;
}

int send_datagram(){

}


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
#include "Checksum.h"
#include "datalink_sender.h"

unsigned char Source_address[4] = {0x00,0x00,0x00,0x01}; //源地址 4bytes
unsigned char Destination_address[4] = {0x00,0x00,0x00,0x02}; //目的地址 4Bytes


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
 */
unsigned int datagram_construction(unsigned char datagram[65535],unsigned char Version,
                            unsigned char Differentialted_Services,unsigned short Identification,
                            unsigned short DF, unsigned short MF,
                            unsigned short Fragment_offset, unsigned char TTL, 
                            unsigned char Protocol,
                            unsigned char *Source_address, unsigned char *Destination_address,
                            unsigned char *Option,unsigned int Option_len,
                            unsigned char *payload,unsigned short payload_len){
    /* data length check ,0--44KB*/
    if (payload_len > 65515){
        printf("[Error] Payload length too long!\n");
        exit(1);
    }
    /* Construct data to be a datagram array */
    /* 计算datagram header 长度 ,单位（行）*/
    unsigned char IHL = 5 + Option_len; // 取char中低四位,4bit

    /* 组装Version IHL为一个Bytes */
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

    /* 组装DF MF Fragment为一个Bytes */
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
    unsigned char Header[IHL * 4]; // 去除checksum后的头大小
    Header[10] = Header[11] = 0x00;  //  将checksum部分16bit全置0
    memcpy(&Header[0],&datagram[0],10);
	memcpy(&Header[12],&datagram[12],8 + Option_len * 4);
    unsigned short Header_checksum = Checksum(Header,IHL * 4);
    memcpy(&datagram[10],&Header_checksum,2); 
    
    /* get char array for cksum result, so that program can print it */
    unsigned char checksum_result[2];
    memcpy(checksum_result, &Header_checksum, sizeof(Header_checksum));
    memcpy(&datagram[20 + Option_len*4],payload,payload_len);

    /* print datagram information */ 
    printf("******Send datagram information******\n");
    printf("Source address: ");
    output_octal(Source_address,4);
    printf("Destinamtion address  : ");
    output_octal(Destination_address,4);
    printf("payload      : %s\n",payload);
    printf("checksum     : ");
    output_octal(checksum_result, 2);    
    printf("payload length  : %d\n", payload_len);
    printf("Header length   : %d(row*4bytes)\n", IHL);
    printf("datagram length : %d\n", Total_length);
    printf("**************************************\n");
    printf("[INFO] datagram construct success!\n");
    printf("\n");
    return Total_length;
}

/**
 * fragmentation and send datagram
 * @payload 数据
 * @payload_len payload长度
 */
void fragmentation_datagram_send(unsigned char *payload,unsigned short payload_len){
    printf("[INFO] datagram fragment begin!\n");
    /* 本地信息 */
    unsigned int fragmentation_size = 1500; // 限制分片大小 1500B
    unsigned char Version = 4; // IPV版本号 4bit, 取char中的低四位
    unsigned char Differentialted_Services = 0; //服务类型，1Byte
    unsigned short Identification = 0; // 识别号 2Bytes
    unsigned short DF = 0x00;   // 可分片  0为可分，1未不分片，1bit，取short中低一位
    unsigned short MF = 0x00; // 非末尾片标识 1bit，取short中低一位
    unsigned short Fragment_offset = 0; // 位偏移 13bit，取short中低13位
    unsigned char TTL = 20; // 生存期 1Byte
    unsigned char Protocol = 0; // 协议 1Byte
    unsigned char Option[] = "1234567"; // 可选项
    unsigned int Option_len = sizeof(Option)/4; // 可选项长度 （单位 行，一行4Bytes）

    /* payload length check ,0--44KB*/
    if (payload_len > 65515){
        printf("[Error] Payload length too long!\n");
        exit(1);
    }
    /* 分片 */
    int max_len = fragmentation_size - Option_len*4-20; //分片最大可容量
    int i = 0;
    /* 循环切片生成datagram并发送*/
    for(i=0;i<payload_len/max_len; i++){
        unsigned char frag_datagram[65535]; // 分片后的数据报
        /* 判断是否允许分片 */
        unsigned short df = DF & 0x0001;
        if(df){ // DF == 1, 不允许分片
            printf("[Error] DF = 1. Don't fragmented!\n");
            exit(1);
        }
        unsigned short mf = 1;
        /* MF=0表示这一串整体，即可以对已分片的再分片
         * 判断是否为末尾最后一个分片 
         */ 
        if(MF == 0 && i == payload_len/max_len -1 && payload_len%max_len == 0){ 
            mf = 0;
        }
        
        /* offset 偏移量处理 */
        unsigned short offset = Fragment_offset + max_len * i;

        /* 构建payload */
        unsigned char *buffer = (unsigned char*)malloc(max_len + 1);
        memcpy(buffer,&payload[max_len*i],max_len);
        buffer[max_len] = '\0';

        /* 构建datagram */
        int length = datagram_construction(frag_datagram,Version,Differentialted_Services,Identification,
                            df,mf,offset,TTL,Protocol,Source_address,
                            Destination_address,Option,Option_len,
                            buffer,max_len);
        
        /* 调用datalink frame构建即发送 */
        int frame_length = frame_construction_send(frag_datagram,length);
    }

    /* 处理分片尾部剩余 */
    if(payload_len%max_len != 0){
        unsigned char frag_datagram[65535]; // 分片后的数据报
        int last_len = payload_len%max_len;
        unsigned short df = DF & 0x0001;
        unsigned short mf = 1;
        if(MF == 0){ // 已经到末尾More Fragmentation = 0
            mf = 0;
        }
        /* 构建payload */
        unsigned char *buffer = (unsigned char*)malloc(payload_len%max_len + 1);
        memcpy(buffer,&payload[max_len*i],payload_len%max_len);
        buffer[payload_len%max_len] = '\0';
        /* 构建datagram */
        unsigned short offset = Fragment_offset + max_len * i;
        int length = datagram_construction(frag_datagram,Version,Differentialted_Services,Identification,
                            df,mf,offset,TTL,Protocol,Source_address,
                            Destination_address,Option,Option_len,
                            buffer,payload_len%max_len);

        /* 调用datalink frame 构建即发送 */
        int frame_length = frame_construction_send(frag_datagram,length);
        printf("[INFO] datagram fragment successfully!\n");
    }
}



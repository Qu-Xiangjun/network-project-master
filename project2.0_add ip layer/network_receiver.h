
/**
 * Name         : newtwork_receiver.h 
 * Author       : Qu Xiangjun
 * Version      : 2.0
 * Date         : 2020-11-1
 * Description  : network datagram receiver function, and analysis datagram.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Output_Octal.h"
#include "crc16.h"

/**
 * check address (char array, length = 6 Bytes)
 * @des_addr received frame's destination address
 * @return flag: the resul of check (boolean) 
 **/
int addr_check(unsigned char des_addr[4], unsigned char my_addr[4]){
    int flag = 1;
    int i = 0;
    for (i = 0; i < 4; i++){
        if (des_addr[i] != my_addr[i])
        flag = 0;
    }
    if (flag){
        printf("[INFO] Address check success!\n");
    } else{
        printf("[Error] Address check error!\n");
        printf("[Error] Destination address : ");
        output_octal(des_addr, 4);
    }
    return flag;
}

void analyse_datagram(){
    /* 定义局部变量 */
    unsigned char my_addr[4] =  {0x00,0x00,0x00,0x02};
    unsigned char datagram[65535];
    unsigned char Version;
    unsigned char IHL;
    unsigned char Differentialted_Services; 
    unsigned short Total_length;
    unsigned short Identification; 
    unsigned char DF; 
    unsigned char MF; 
    unsigned short Fragment_offset; 
    unsigned char TTL; 
    unsigned char Protocol; 
    unsigned short Header_checksum;
    unsigned short crc16_result;
    unsigned char Source_address[4];
    unsigned char Destination_address[4];
    unsigned char *Option; 
    unsigned int Option_len;
    unsigned char payload[65535]; 
    unsigned short payload_len;


    printf("[INFO] datagram receiver process begin.\n");
	printf("[INFO] My IP address  : ");
	output_octal(my_addr,4);
    printf("\n");
    printf("\n");
    FILE *file = fopen("datalink_to_network.bin", "r");

    int count = 0; //接收到的片数
    int MF_flag = 0; // 确认已经收到了MF = 0最后一片
    int max_len = 0; // 片的最大长度
    int max_offset = 0; // 片最后一位的开头
    int payload_total_len = 0;
    unsigned short total_identification;

    /* read frame length */
    while (fread(&Total_length,sizeof(unsigned int),1,file)){     
    	printf("Total_length %d\n",Total_length); 
        fread(&datagram, sizeof(char), Total_length, file);
        /* 取Version 和 IHL */
        unsigned char temp;
        memcpy(&temp, &datagram[0],1);
        Version = temp & 0xF0;
        Version = Version >> 4;
        IHL = temp & 0x0F;

        memcpy(&Differentialted_Services,&datagram[1],1);
        memcpy(&Total_length,&datagram[2],2);
        memcpy(&Identification,&datagram[4],2);

        /* 取DF、MF、Fragment Offset */
        memcpy(&Fragment_offset,&datagram[6],2);
        DF = (Fragment_offset & 0x4000) >> 14;
        MF = (Fragment_offset & 0x2000) >> 13;
        Fragment_offset  = Fragment_offset & 0x1fff;

        memcpy(&TTL,&datagram[8],1);
        memcpy(&Protocol,&datagram[9],1);
        memcpy(&Header_checksum,&datagram[10],2);
        memcpy(&Source_address,&datagram[12],4);
        memcpy(&Destination_address,&datagram[16],4);

        /* 取Option */
        Option_len = IHL;
        Option_len -= 5; // 比header少五行
        memcpy(&Option,&datagram[20],Option_len * 4);

        /* 取payload,有偏移位 */
        payload_len = Total_length - (Option_len + 5) * 4;
        memcpy(&payload[Fragment_offset],&datagram[20+Option_len*4],payload_len);

        payload_total_len += payload_len; // 计算总payload长度
        if(payload_len > max_len){
            max_len = payload_len;
        }
        
        /* 地址验证 */
        if (!addr_check(Destination_address, my_addr))
            exit(1);

        /* 校验crc16 */
        unsigned char Header[IHL * 4 - 2];
        memcpy(&Header[0],&datagram[0],10);
        memcpy(&Header[10],&datagram[12],8 + Option_len * 4);
        crc16_result = CRC16(Header,IHL * 4 - 2);
        if(Header_checksum != crc16_result){  // crc16 校验失败
            printf("[Error] CRC16 check error!\n");
            printf("[Error] datagram's CRC16 : ");
            printf("%02x%02x\n", datagram[10], datagram[11]);
            printf("[Error] calculated CRC16 : ");
            printf("%04x\n",crc16_result);
            exit(1);
        }

        /* 检查是否不用分片 */
        if(DF == 1){
        	printf("[Error] DF = 1, Don't fragmentation.' : ");
            break;
        }

        /* 获得最后一片的偏移开始 */
        if(max_offset < Fragment_offset){
            max_offset = Fragment_offset;
        }

        /* 检查是否获得到最后一片 */
        if(MF == 0){
            MF_flag = 1;
        }

        /* 检查调数是否为0 */
        if(TTL == 0){  // 为0则丢弃
            continue;
        }
        
        /* 收到的第一片，确定头信息
         * 确定identification,用于之后甄别正确的
         */ 
        if(count == 0){
            total_identification = Identification;
        }else{
            if(total_identification != Identification) // 不是同一分片,跳过
                continue;
        }   
		
		count ++;     
    }
    fclose(file);
    /* 清除文件内容 */
    FILE *clean = fopen("datalink_to_network.bin", "w");
    fclose(clean);
    
    // /* 计算Total length */
    // Total_length = payload_total_len + IHL*4;

    /* 判断是否组装完全 */
    if(count-1 != max_offset/max_len){ // 前面来的片都是最大片长度，如若其数量不为count-1，则为组装完成
        printf("[Error] Don't received all fragment!\n");
        exit(1);
    }  

    /* print frame information */ 
    printf("******receive datagrame information******\n");
    printf("Source address: ");
    output_octal(Source_address,4);
    printf("Destination address  : ");
    output_octal(Destination_address,4);
    printf("payload      : %s\n",payload);
    printf("CRC16        : ");
    printf("%04x\n",crc16_result); 
    printf("payload length  : %d\n", payload_total_len);
    printf("Header length   : %d(row*4bytes)\n", IHL);
    printf("datagram length : %d\n", Total_length);
    printf("MF              : %d\n", MF_flag);
    printf("**************************************\n");
    printf("[INFO] datagram received success!\n");

    printf("[INFO] datagram receiver process end.\n");

}

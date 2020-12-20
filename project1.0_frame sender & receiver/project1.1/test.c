#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
unsigned char frame[2000];  //frame array
int file_size(FILE * file_handle)
{
	//获取当前读取文件的位置 进行保存
	unsigned int current_read_position=ftell( file_handle );
	int file_size;
	fseek( file_handle,0,SEEK_END );
	//获取文件的大小
	file_size=ftell( file_handle );
	//恢复文件原来读取的位置
	fseek( file_handle,current_read_position,SEEK_SET );
	
	return file_size;
}
int main(int argc, char **argv)
{
    FILE *file = fopen("pipe.bin", "r");

//	fgets(frame,2000-1,file);
//    int frame_len = strlen(frame);
	int size = file_size(file);
	printf("%d",size); 

	return 0;
}


/*****************************************************
** Name         : crc32.h
** Author       : Qu Xiangjun
** Version      : 1.0
** Date         : 2020-10-8
** Description  : CRC32 Checking
******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFSIZE     1024*4

static unsigned int crc_table[256];
const static char * program_name = "crc32";

static void usage(void);
static void init_crc_table(void);
static unsigned int crc32(unsigned char * buffer, unsigned int size);
// static int calc_img_crc(const char * in_file, unsigned int * img_crc);

static void usage(void)
{
	fprintf(stderr, "Usage: %s input_file\n", program_name);
}


static void init_crc_table(void)
{
	unsigned int c;
	unsigned int i, j;
	
	for (i = 0; i < 256; i++) {
		c = (unsigned int)i;
		for (j = 0; j < 8; j++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[i] = c;
	}
}

static unsigned int crc32(unsigned char *buffer, unsigned int size)
{
	unsigned int crc = 0xFFFFFFFF;
	unsigned int i;
	for (i = 0; i < size; i++) {
		crc = crc_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
	}
	return crc ;
}

static int calc_img_crc(const char *in_file, unsigned int *img_crc)
{
	int fd;
	int nread;
	int ret;
	unsigned char buf[BUFSIZE];

	unsigned int crc;

	fd = open(in_file, O_RDONLY);  // O_RDONLY 只锟斤拷
	if (fd < 0) {
		printf("%d:open %s.\n", __LINE__, strerror(errno));
		return -1;
	}
		
	while ((nread = read(fd, buf, BUFSIZE)) > 0) {
		crc = crc32(buf, nread);
	}
	*img_crc = crc;

	close(fd);
	
	if (nread < 0) {
		printf("%d:read %s.\n", __LINE__, strerror(errno));
		return -1;
	}
	
	return 0;
}


// int main(int argc, char **argv)
// {
// 	// int ret;
// 	// unsigned int img_crc;
// 	// const char *in_file = argv[1];

// 	// if (argc < 2) {
// 	// 	usage();
// 	// 	exit(1);
// 	// }

// 	// init_crc_table();
	
// 	// ret = calc_img_crc(in_file, &img_crc);
// 	// if (ret < 0) {
// 	// 	exit(1);
// 	// }

// 	// printf("The crc of %s is:%u\n", in_file, img_crc);

//     unsigned char data[] = {0x60,0x0f,0x02,0xf4,0x00,0x3b,0x11,0x40,0x20,0x01,0x0d,0x58,0xc8,0x00,0xad,0x08,0xe3,0xa9,0x18,0xfa,0x43,0x74,0x94,0x53,0x20,0x01,0x0d,0xa8,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0xc8,0x60,0x00,0x35,0x00,0x3b,0xbc,0xd5,0x9a,0x9e,0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x6e,0x65,0x74,0x77,0x6f,0x72,0x6b,0x2d,0x70,0x72,0x6f,0x6a,0x65,0x63,0x74,0x2d,0x69,0x6e,0x66,0x6f,0x07,0x63,0x79,0x79,0x73,0x65,0x6c,0x66,0x04,0x6e,0x61,0x6d,0x65,0x00,0x00,0x10,0x00,0x01};
// 	unsigned int crc32_result_cyy = crc32cyy(data,87);
// 	init_crc_table();
// 	unsigned int crc32_result = crc32cyy(data,87);
// 	unsigned int crc32_result_lql = crc32lql(data,87);
//     printf("crc32_result      %d\n",crc32_result);
//     printf("crc32_result_cyy  %d\n",crc32_result_cyy);    
//     printf("crc32_result_lql  %d\n",crc32_result_lql);

// 	return 0;
// }

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc != 2 || 0 != strcmp("-r", argv[1]))
	{
		printf("Usage:%s -r\n", argv[0]);
		return -1;
	};

	int fd;

	fd = open("/dev/ap3216Chrdev", O_RDONLY);

	if(fd < 0)
	{
		printf("Read chrdev error.\n");
		return -1;
	};

	char data[6];
	int temp = 0;
	for(; ;)
	{
		
		read(fd,data,6);
		printf("Data_read as fellow:\n");
		
		//IR 10bit data
		temp = (data[0]&0x3)|((data[1]&0xff)<<2);
		printf("IR: %4d,",temp);
		
		//ALS 16bit data
		temp = (data[2]&0xff)|((data[3]&0xff)<<8);
		printf("ALS: %5d,",temp);
		//PS 10bit data
		temp = (data[4]&0xf)|((data[5]&0x3f)<<4);
		printf("PS: %4d.\n",temp);
	}
	

	
	return 0;
};


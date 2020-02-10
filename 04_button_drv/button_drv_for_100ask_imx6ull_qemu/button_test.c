#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
* ./button_test /dev/kxb_button0 按下，点亮led0，或者led1
*/

int main(int argc,char *argv[])
{
	int fd;
	int fd_led;
	int val;
	char path_led[64];
	unsigned char status;

	if(2 != argc)
	{
		printf("Usage: %s <dev>\n",argv[0]);
		return -1;
	}

	fd = open(argv[1],O_RDWR);
	if(-1 == fd)
	{
		printf("cannot open file %s\n",argv[1]);
		return -1;
	}

	read(fd,&val,sizeof(val));
	printf("get button : %d\n",val);

	memset(path_led,0,sizeof(path_led));
	if(0 == strcmp(argv[1],"/dev/kxb_button0"))
	{
		memcpy(path_led,"/dev/kxb_led0",strlen("/dev/kxb_led0"));
	}
	else if(0 == strcmp(argv[1],"/dev/kxb_button1"))
	{
		memcpy(path_led,"/dev/kxb_led1",strlen("/dev/kxb_led1"));
	}

	//打开led
	fd_led = open(path_led,O_RDWR);
	if(-1 == fd_led)
	{
		printf("cannot open file %s\n",path_led);
		return -1;
	}

	if(val)
	{
		status = 1;
		
	}
	else 
	{
		status = 0;
	}

	write(fd_led,&status,1);
	
	close(fd_led);
	close(fd);
	return 0;
}

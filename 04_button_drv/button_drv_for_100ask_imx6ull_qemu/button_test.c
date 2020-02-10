#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
* ./button_test /dev/kxb_button0/1
*/

int main(int argc,char *argv[])
{
	int fd;
	int val;

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
	
	close(fd);
	return 0;
}

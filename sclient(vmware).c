#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define BAUDRATE        B9600
#define MODEMDEVICE     "/dev/ttyACM0"
#define _POSIX_SOURCE   1       // POSIX compliant source

#define FALSE   0
#define TRUE    1

void error_handling(char *message);
int main(int argc, char *argv[])
{
        int fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
        if (fd <0) { perror(MODEMDEVICE); exit(-1); }

        struct termios tio;
        tcgetattr(fd,&tio);
        tio.c_cflag |= BAUDRATE;
        tio.c_cflag |= CS8;
        tio.c_lflag = ICANON;
        tcsetattr(fd,TCSANOW,&tio);

        int count=1;
	int sock;
	char buf[BUF_SIZE];
	int str_len, recv_len, recv_cnt;
	struct sockaddr_in serv_adr;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");

        while(1)
        {
                int res = read(fd,buf,255);
		buf[res] = '\0';
		if(res > 1)
		{
                	printf("%d :%s\n",count++, buf);
			write(sock, buf, strlen(buf));
		}
                if(res<=0) break;

        }

        close(fd);
        return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}



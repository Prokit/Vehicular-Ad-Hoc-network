#include     <stdio.h>      /*��׼�����������*/
#include     <stdlib.h>     /*��׼�����ⶨ��*/
#include     <unistd.h>     /*Unix��׼��������*/
#include     <sys/types.h>  /**/
#include     <sys/stat.h>   /**/
#include     <fcntl.h>      /*�ļ����ƶ���*/
#include     <sys/socket.h>
#include     <arpa/inet.h>
#include     <netinet/in.h>
#include     <termios.h>    /*PPSIX�ն˿��ƶ���*/
#include     <errno.h>      /*����Ŷ���*/
struct termios opt;
int fd;
int SerialInit();
int main(void)
{
    int sockfd;
    struct sockaddr_in address;
    int result;
	int  n, len,  fd_max,i;
    fd_set  readfds;/* ������һ�� fd_set ��������������Ҫ���� socket��� */
  	char buffer[1];
    char send;
	static struct termios oldtio,newtio;
	int fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY | O_NDELAY);//270
	if(fd<0)
	{
		perror("Cannot Open Serial Port !\n");
	}
	tcgetattr(fd,&oldtio);//����ԭ�ȴ�������ʹ��tcgetattr(fd,&oldtio)����
	tcgetattr(fd,&newtio);
	//���ò����ʣ�ʹ�ú���cfsetispeed�� cfsetospeed
	cfsetispeed(&newtio,B9600);
	cfsetospeed(&newtio,B9600);
	newtio.c_lflag&= ~(ICANON | ECHO | ECHOE | ISIG);
	newtio.c_cflag &= ~PARENB;
	newtio.c_cflag &= ~CSTOPB;	
    //��������λ����ʹ���������á�	
	newtio.c_cflag &= ~CSIZE;
	newtio.c_cflag |= CS8;	
	//��������
	newtio.c_oflag &= ~OPOST;	
	//����ѡ����CLOCAL��CREAD,���ڱ������Ӻͽ���ʹ��
	newtio.c_cflag |= (CLOCAL | CREAD);	
	newtio.c_iflag = 0;
	newtio.c_cc[VTIME] = 1;
	newtio.c_cc[VMIN] = 10;
	if(tcsetattr(fd,TCSAFLUSH,&newtio)<0)
	{
		printf("tcsetattr fail !\n");
		exit(1);
	}
    FD_ZERO(&readfds);//���������������set��ȫ��λ
    FD_SET(fd, &readfds);//����������������set�����fd��λ
    fd_max = fd + 1;
	/*socket*/
    while(1) 
    {
	   SerialInit();
	   n = select(fd_max,&readfds,NULL,NULL,NULL);
	   memset(buffer, 0, sizeof(buffer));
	   if (n<=0)/* ��˵��select�������� */
     	      printf("select failed");
     else
        {  	
        len=read(fd,buffer, 1);
        buffer[len] = '\0';
        send='5';
        fprintf(stderr, "Readed %d data:%c\n",len,send);  
        sockfd = socket(AF_INET,SOCK_STREAM,0);
        address.sin_family = AF_INET;
        address.sin_addr.s_addr =inet_addr("192.168.1.102");
        address.sin_port = 9734;
        len = sizeof(address);
        result = connect(sockfd, (struct sockaddr *)&address,len);
        if(result == -1)
     		{
                perror("oops: client");
                exit(1);
            }
		 write(sockfd, &send ,1);//����Ƭ������������д��socket�У�Ȼ�󷢸�A��
		 close(sockfd);
	       }
     }
}	
int SerialInit()
{
	fd=open("/dev/ttyUSB0",O_RDWR);	
	tcgetattr(fd,&opt);
	cfsetispeed(&opt,B9600);
	cfsetospeed(&opt,B9600);
	opt.c_cflag |= (CLOCAL|CREAD);
	opt.c_cflag &= ~CSIZE;
	opt.c_cflag |= CS8;
	opt.c_cflag &= ~PARENB;
	opt.c_cflag &= ~CSTOPB;
	opt.c_iflag &= ~INPCK;
	opt.c_iflag |= IGNPAR|ICRNL;
	opt.c_iflag &= ~(IXON|IXOFF|IXANY); 
	opt.c_oflag |= OPOST;   
 	opt.c_cc[VMIN]=0;
	opt.c_cc[VTIME]=0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &opt);
}


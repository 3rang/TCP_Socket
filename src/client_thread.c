#include <stdio.h>
#include <string.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8899
#define BUFFSIZE 500000

int main(void)
{
	int client_fd;
	struct sockaddr_in client_addr;
	client_fd = socket(AF_INET,SOCK_STREAM,0);

	client_addr.sin_family = AF_INET;
    	client_addr.sin_port = htons(PORT);
    	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(client_fd,(struct sockaddr *)&client_addr,sizeof(client_addr));

	char buff[BUFFSIZE];
	
	while(1)
	{
	
		send(client_fd,&buff,sizeof(buff),0);
		usleep(200000);
	
	}
	close(client_fd);

}

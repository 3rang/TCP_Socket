#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/time.h>
#include<stdlib.h>

#define PORT 8899
#define MAX_CONNECTION 50
#define BUFFSIZE 120000
struct arg_struct
{
	int arg1;
	long arg2;
} *args;

long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

void *thread_handler(void *arg)
{
	struct arg_struct *ar = arg;
	printf("here\n");
	char buff[BUFFSIZE];
	int newsock = ar->arg1;
	long ct= ar->arg2;
	int j=0;
	long time_up=getMicrotime();
	while(1)
	{
		j=read(newsock,(char *)&buff,sizeof(buff));
		if(j > 0 ){
//			if(j==0)
//			{
				time_up=getMicrotime();
			//	j=1;
//			}
		//	printf("In thread at accept_time = %ld & sock id =%d \n",time_up,newsock);
			printf("Read_size / Cli_Id / Diff_time / size / = %d / %d / %ld / %ld \n",j,newsock,(time_up-ct),sizeof(buff));
			ct=time_up;
		//	printf("Data_size=%ld\n",sizeof(buff));
			bzero(&buff,BUFFSIZE);
//			j=1;
		}
	}	
	return NULL;
}



int main(void)
{
	int server_fd,client_fd;
	struct sockaddr_in server_addr, client_addr;
	pthread_t pid[MAX_CONNECTION];
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==server_fd)
	{
		printf("Error in socket creation\n");
		return -1;
	}
	else
	{
		printf("Socket created successfully\n");

	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(server_fd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
	{
		printf("bind error \n");
		return -1;
	}
	else
	{
		printf("bind successful..\n");
	}

	listen(server_fd,50);
	int client_sock,i=0;
	socklen_t client_size; 

	args = malloc(sizeof(struct arg_struct) * 1);


	while(1)
	{	

		client_size = sizeof(client_addr);
		client_sock = accept(server_fd, (struct sockaddr *)&client_addr,&client_size);

		args->arg1 = client_sock;
		args->arg2 = getMicrotime();

		printf("at accept_time = %ld & sock id =%d \n",args->arg2,args->arg1);
		pthread_create(&pid[i++],NULL,thread_handler,args);

	}

	close(server_fd);
}

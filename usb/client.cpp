#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <port of server> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    int port = atoi(argv[1]);
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 
    
    char msg[1024] = {0};
    for (int i = 0; i < 15; i++) {
    	sprintf(msg, "zexing - %d", i);
    	printf("write ...\n");
    	n = write(sockfd, msg, strlen(msg));
    	printf("write complete %d bytes transfered.\n", n);
    	n = read(sockfd, recvBuff, sizeof(recvBuff) - 1);
    	if (n > 0) {
    		printf("GOT %d bytes : %s\n", n, recvBuff);
    	} else {
    		fprintf(stderr, "%d read fail\n", i);
    	}
    	sleep(1);
    }

/*
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    } 
*/
    if(n < 0)
    {
        printf("\n Read error \n");
    } 

    return 0;
}


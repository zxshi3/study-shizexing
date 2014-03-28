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

int main(int argc, char ** argv)
{
	int sockfd = 0, n = 0;
	char buf[1024];
	struct sockaddr_in addr;

	memset(&buf[0], 0, sizeof(buf));
	sprintf(&buf[0], "hello, ios");
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("create socket fail\n");
		return 1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	//addr.sin_port = htons(12345);
	addr.sin_port = htons(8008);
	
	char * ip = "127.0.0.1";
	if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
	{
		printf("inet_pton error\n");
		close(sockfd);
		return 1;
	}

	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		printf("connect error\n");
		close(sockfd);
		return 1;
	}

	n = write(sockfd, buf, strlen(buf));
	printf("write %d bytes. %s\n", n, buf);

	n = read(sockfd, buf, sizeof(buf) - 1);
	if (n > 0)
	{
		buf[n] = '\0';
		printf("read %d bytes. %s\n", n, buf);
	}
	else {
		printf("read error. %d\n", n);
	}

	return 0;
}


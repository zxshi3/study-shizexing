#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

bool quit = false;
void sig_handler(int signum) {
	printf("handle signal %s\n", strsignal(signum));
	quit = 1;
}

int main(int argc, char** argv) {
	if (SIG_ERR == signal(SIGINT, sig_handler)) {
		printf("can't catch SIGINT\n");
	}
	int cnt = 60;
	while(!quit) {
		if (cnt > 0) {
			sleep(1);
			cnt--;
		} else {
			break;
		}
	}
}


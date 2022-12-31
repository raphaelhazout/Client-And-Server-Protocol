#include <stdlib.h>
#include <iostream>
#include <sys/fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/random.h>
#include <string.h>
// Client side


void handler(int sig) {
	char buffer[50] = "to_client_";
	pid_t clientPID = getpid();
	char clientStringPID[15];
	sprintf(clientStringPID, "%d", clientPID);
	strcat(buffer, clientStringPID);
	int fd1 = open(buffer, O_RDONLY);
	int j = 0;
	char c;
	char calc[50]="";
	int rd = read(fd1, &c, 1);
	while (rd > 0) {
		if (c == '\n') {
			break;
		}
		calc[j++] = c;
		rd = read(fd1, &c, 1);
	}
	printf("The calculation result is: %s", calc);
	remove(buffer);
}



int main(int argc,char* argv[])
{
	if (argc != 5) {
		printf("ERROR_FROM_EX2\n");
		return -1;
	}
	pid_t clientPID = getpid();
	char clientStringPID[20];
	sprintf(clientStringPID, "%d", clientPID);
	int serPid = atoi(argv[1]);
	char num1[50];
	strcpy(num1, argv[2]);
	char operation = argv[3][0];
	char num2[50];
	strcpy(num2, argv[4]);

	int count = 1;
	int rand_num;
	while (1) {
		int fd;
		if ((fd = open("to_serv.txt", O_RDONLY)) == -1) {
			fd = open("to_serv.txt", O_CREAT | O_WRONLY | O_RDONLY, 0777);
			write(fd, clientStringPID, 7);
			write(fd," ", 1);
			write(fd, num1, strlen(num1)+1);
			write(fd, " ", 1);
			write(fd, &operation, sizeof(operation));
			write(fd, " ", 1);
			write(fd, num2, strlen(num2)+1);
			close(fd);
			kill(serPid,1);
			break;
		} else {
			count++;
			if (count == 10) {
				printf("ERROR_FROM_EX2\n");
				return -1;
			}
			getrandom(&rand_num, sizeof(int), 0);   // Use getrandom to get a random number between 0 and 5
			rand_num %= 6;
			sleep(rand_num);
		}
	}
	signal(2, handler);
	int slp_time = sleep(30);
	if (slp_time == 0) {
		printf("Client closed because no response was received from the server for 30 second");
		remove("to_serv.txt");
		return -1;
	}
	return 0;
}
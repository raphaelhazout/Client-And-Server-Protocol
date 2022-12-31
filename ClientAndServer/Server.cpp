#include <stdlib.h>
#include <iostream>
#include <sys/fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
// Server side

void handler2(int sig) {
	pid_t pid = fork();
	if (pid == 0) { // child process
		int fd = open("to_serv.txt", O_RDONLY);
		char c;
		int i = 0;
		int counter = 0;
		char clientPID[50];
		char operation;
		char firstNum[50];
		char secondNum[50];
		int rd = read(fd, &c, 1);
		while (rd > 0) {
			if (c == ' ') {
				++counter;
				i = 0;
				rd = read(fd, &c, 1);
			}
			if (counter == 0) {
				clientPID[i++] = c;
			}
			if (counter == 1) {
				firstNum[i++] = c;
			}
			if (counter == 2) {
				operation = c;
			}
			if (counter == 3) {
				secondNum[i++] = c;
			}
			if (counter == 4) {
				break;
			}
			rd = read(fd, &c, 1);
		}
		remove("to_serv.txt");
		int num1 = atoi(firstNum);
		int num2 = atoi(secondNum);
		char buff[100] = "to_client_";
		strcat(buff, clientPID);
		int cliepid = atoi(clientPID);

		int fd2 = open(buff, O_CREAT | O_WRONLY | O_RDONLY, 0777);
		int calculated;

		switch (operation)
		{
			case '1':
					calculated = num1 + num2;
					break;
			case '2':
					calculated = num1 - num2;
					break;
			case '3':
					calculated = num1 * num2;
					break;
			case '4':
				if (num2 == 0) {
					write(fd2, "Cant Divide in Zero!", sizeof("Cant Divide in Zero!"));
					close(fd2);
					kill(cliepid, 2);
					exit(0);

				}
				else {
					calculated = num1 / num2;
				}
				break;
			default:
				break;
		}
		char calculatedChar[50];
		sprintf(calculatedChar, "%d", calculated);
		write(fd2, calculatedChar, strlen(calculatedChar));
		kill(cliepid, 2);
		exit(0);
	} else {
		wait(NULL);
		return;
	}
}

int main(int argc, char* argv[])
{
	signal(1, handler2);
	while (1) {
		int sleep_time = sleep(60);
		if (sleep_time == 0) {
			printf("The server was closed because no service request was received for the last 60 Seconds\n");
			exit(0);
		}
	}
}
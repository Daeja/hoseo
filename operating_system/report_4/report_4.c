#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 100

void printPID(char * header, char * msg);

int main() {
	int pid;
	int p_len, c_len;
	int c_to_p_fd[2];
	int p_to_c_fd[2];
	char c_to_p_msg[MAXLINE], c_to_p_line[MAXLINE];
	char p_to_c_msg[MAXLINE], p_to_c_line[MAXLINE];

	pipe(c_to_p_fd);
	pipe(p_to_c_fd);

	////////////////////////////////////////////////////////
	//
	//	* 이동 방향 c_to_p_fd[1] -> c_to_p_fd[0]
	//	* 이동 방향 p_to_c_fd[1] -> p_to_c_fd[0]
	//
	////////////////////////////////////////////////////////
	if((pid = fork()) == 0) {
		close(c_to_p_fd[0]);
		close(p_to_c_fd[1]);

		sprintf(c_to_p_msg, "child PID = %d\n\n", getpid());

		c_len = strlen(c_to_p_msg) + 1;

		write(c_to_p_fd[1], c_to_p_msg, c_len);
		read(p_to_c_fd[0], p_to_c_line, MAXLINE);

		printPID("---- child process ----", p_to_c_line);
	}
	else {
		close(c_to_p_fd[1]);
		close(p_to_c_fd[0]);

		sprintf(p_to_c_msg, "parent PID = %d\n\n", getpid());

		p_len = strlen(p_to_c_msg) + 1;

		read(c_to_p_fd[0], c_to_p_line, MAXLINE);
		
		// 0.1초 대기함 - 부모프로세스가 먼저 종료되는 경우 쉘 프롬프트가 
		//				  자식프로세스보다 먼저 튀어나와서 보기에 좋지 않음.
		usleep(1000 * 100);
		write(p_to_c_fd[1], p_to_c_msg, p_len);
		
		printPID("---- parent processs ----", c_to_p_line);
	}

	exit(0);
}

////////////////////////////////////////////////////////
//
//	* 출력 부분
//
////////////////////////////////////////////////////////
void printPID(char * header, char * msg) {
	printf("%s\n", header);
	printf("%s", msg);
}

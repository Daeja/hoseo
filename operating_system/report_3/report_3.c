#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

#define Bool int
#define TRUE 1
#define FALSE 0

#define BUFSIZE 100

void printMainMenu();
char * initSize(char * buf);
void printCommandList();

void main() {
		Bool OnOff;
		int pid;				//process ID
		int child;				//child
		int status;				//wait child process when end it.
		char * command = initSize(command);								//선언과 동시에 동적 할당
		char path[20] = "/bin/";										//해당 명령어 file의 경로 앞부분
		char temp[6] = "/bin/";											//strcat후 다시 덮어씌우는 변수
	
		OnOff = TRUE;													//TRUE(1)값을 주고 exit를 입력하기 전까지 반복함

		while(OnOff) {
				printMainMenu();

				scanf("%s", command);									//매뉴얼을 볼 명령어 입력
	
				if(strcmp(command, "exit") == 0) {						//exit라는 문자열이 들어오면 프로세스 종료
						exit(0);
				}

				strcpy(path, temp);										//path의 내용을 원 내용으로 덮어쓰기
				strcat(path, command);									//"/bin/" + 해당 명령어

				pid = fork();											//create child process

				if(pid == 0) {											//child process일 때 아래 내용 수행
						if(strcmp(command, "list") == 0) {				//list라는 문자열이 들어오면 /bin 내의 리스트 출력
								execl("/bin/ls", "ls", "/bin", NULL);
								exit(1);								//child process 종료
						}
						else {											//list가 아닐 경우 입력한 command의 매뉴얼 출력(--help)
								execl(path, command, "--help", NULL);
						}
				}
		
		sleep(5);														//manual or list를 약 5초간 보여주기 위해

		child = wait(&status);											//parent process가 child process가 끝날때까지 기다림
		}

		free(command);													//동적 할당된 메모리를 반환
}

///////////////////////////////////////////////////////
//
//      * 시작시 메인 메뉴 출력
//
///////////////////////////////////////////////////////
void printMainMenu() {
		system("clear");

		printf("□ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □\n");
		printf("□                                             □\n");
		printf("□            Command         Helper           □\n");
		printf("□                                             □\n");
		printf("□ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □\n\n\n");

		printf("1. Input \"list\" to read the command list.\n");
		printf("2. Please input the command to read the manual.\n");
		printf("3. The manual in not outputted in case of the command which doesn't exist.\n\n");
		printf("            input : ");
}

///////////////////////////////////////////////////////
//
//      * 동적 할당 부분
//
///////////////////////////////////////////////////////
char * initSize(char * buf) {
		buf = (char *)malloc(sizeof(char) * BUFSIZE);
		return buf;
}

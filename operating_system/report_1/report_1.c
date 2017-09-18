#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>				//시간

#define NAMESIZE 100			//파일이름의 최대 크기 100(byte)
#define BUFSIZE 4096			//입력 내용의 최대 크기 4k

#define READBYTE 10000			//read() 함수를 사용할때 읽어오는 바이트 크기

#define Bool int				//Bool 타입 생성
#define TRUE 1					//참
#define FALSE 0					//거짓

char * initBuffer(char * buf);											//버퍼 초기화
char * increaseBuffer(char * buf);										//버퍼 증가
char * initFileName(char * fileName);									//파일명 초기화
int createFile(char * fileName);										//파일 생성
int openFile(char * fileName);											//파일 불러오기
void writeFile(int fd, char * buf);										//파일 쓰기
void readFile(int fd);													//파일 읽기
void printMainMenu();													//메인메뉴 출력
void printOpenMenu();													//불러오기 메뉴 출력

int main(int argc, char * argv[]) {

		Bool OnOff;
		int fd;
		int menu;
		int hour, min, sec;
		char * buf;
		char * fileName;

		OnOff = TRUE;

		while(OnOff == TRUE) {

				printMainMenu();

				scanf("%d", &menu);

				switch(menu) {
						case 1 :
								///////////////////////////////////////////////////////
								//
								//		* 파일 생성
								//
								///////////////////////////////////////////////////////
								fileName = initFileName(fileName);
								buf = initBuffer(buf);

								fflush(stdin);		//버퍼 비워주기

								printf("생성할 메모 이름을 입력하세요 : ");

								scanf("%s", fileName);
								//fgets(buf, BUFSIZE, stdin);

								printf("File Name : %s\n\n", fileName);

								if((fd = createFile(fileName) )!= -1) {					//같은 파일이 없을때 실행
										writeFile(fd, buf);
								}
								break;

						case 2 :
								///////////////////////////////////////////////////////
								//
								//      * 파일 불러오기
								//
								///////////////////////////////////////////////////////
								printOpenMenu();

								fileName = initFileName(fileName);
								buf = initBuffer(buf);

								fflush(stdin); 										    //버퍼 비워주기

								scanf("%s", fileName);

								if((fd = openFile(fileName) )!= -1) {
										readFile(fd);
										writeFile(fd, buf);
								}
								break;

						case 3 :
								///////////////////////////////////////////////////////
								//
								//      * 종료
								//
								///////////////////////////////////////////////////////
								OnOff = FALSE;

								if(menu == 1 || menu == 2) {
										free(buf);
										free(fileName);

										close(fd);
								}

								system("clear");
								printf("종료합니다.\n\n");
								break;
				}
		}

		return 0;

}

///////////////////////////////////////////////////////
//
//      * 버퍼 초기화 -> realloc를 이용하여 버퍼크기 증가
//
///////////////////////////////////////////////////////
char * initBuffer(char * buf) {
		buf = (char *)malloc(sizeof(char));
		buf = increaseBuffer(buf);

		return buf;
}

char * increaseBuffer(char * buf) {
		buf = (char *)realloc(buf, sizeof(int) * BUFSIZE);

		return buf;
}

///////////////////////////////////////////////////////
//
//      * File Name에 쓰일 Buffer 초기화
//
///////////////////////////////////////////////////////
char * initFileName(char * fileName) {
		fileName = (char *)malloc(sizeof(char) * NAMESIZE);

		return fileName;
}

///////////////////////////////////////////////////////
//
//      * 파일 생성
//
///////////////////////////////////////////////////////
int createFile(char * fileName) {
		int new_fd;

		if((new_fd = open(fileName, O_CREAT | O_RDWR | O_EXCL)) == -1) {
				printf("해당 파일이 이미 존재합니다.\n\n");
				sleep(1);
		}

		return new_fd;
}

///////////////////////////////////////////////////////
//
//      * 파일 불러오기
//
///////////////////////////////////////////////////////
int openFile(char * fileName) {
		int old_fd;

		if((old_fd = open(fileName, O_RDWR | O_APPEND)) == -1) {
				printf("해당 파일이 존재하지 않습니다.\n\n");
				sleep(1);
		}

		return old_fd;
}

///////////////////////////////////////////////////////
//
//      * Buf내용을 File Descriptor를 통해 write사용시
//
///////////////////////////////////////////////////////
void writeFile(int fd, char * buf) {
		struct tm *t = NULL;
		time_t timer;
		char * tempBuf = initBuffer(tempBuf);
		char * delimeter = "\n\n";
		int timeBuf[6];

		printf("Write File...\n\n");

		scanf("%s", buf);

		timer = time(NULL);
		t = localtime(&timer);

		timeBuf[0] = t->tm_year + 1900;
		timeBuf[1] = t->tm_mon + 1;
		timeBuf[2] = t->tm_mday;
		timeBuf[3] = t->tm_hour;
		timeBuf[4] = t->tm_min;
		timeBuf[5] = t->tm_sec;

		sprintf(tempBuf, "%d / %d / %d	%d : %d : %d	", timeBuf[0], timeBuf[1], timeBuf[2], timeBuf[3], timeBuf[4], timeBuf[5]);

		strcat(buf, delimeter);
		strcat(tempBuf, buf);

		if(write(fd, tempBuf, strlen(tempBuf))) {
				printf("저장하였습니다.\n\n");
				sleep(1);
		}

		free(tempBuf);
}

///////////////////////////////////////////////////////
//
//      * File Descriptor로 부터 read사용시
//
///////////////////////////////////////////////////////
void readFile(int fd) {
		char readData[READBYTE];

		read(fd, readData, READBYTE);

		printf("%s", readData);
}	


///////////////////////////////////////////////////////
//
//      * 시작시 메인 메뉴 출력
//
///////////////////////////////////////////////////////
void printMainMenu() {
		system("clear");

		printf("□ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □\n");
		printf("□       M e m o    N o t e      □\n");
		printf("□           1. N e w            □\n");
		printf("□           2. O p e n          □\n");
		printf("□           3. E x i t          □\n");
		printf("□ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □\n\n\n");

		printf("                 번호 : ");
}


///////////////////////////////////////////////////////
//
//      * 2번 메뉴(Open) 실행시 메뉴 출력
//
///////////////////////////////////////////////////////
void printOpenMenu() {
		system("clear");

		printf("□ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □\n");
		printf("□         Open Text File        □\n");
		printf("□ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □\n\n\n");

		printf("               파일명 : ");
}

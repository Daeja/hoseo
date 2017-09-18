#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>								//ctime() 사용
#include <string.h>								//strcmp() 사용
#include <unistd.h>								//sleep() 사용

#define BOOL int
#define TRUE 1
#define FALSE 0

#define NAMESIZE 1024							//경로명의 길이는 1k로 제한.

char type(mode_t);
char * perm(mode_t);
void printStat(char *, char *, struct stat *);
void printMainMenu();
char * initPathName(char * pathName);

///////////////////////////////////////////////////////
//
//      * 메인
//
///////////////////////////////////////////////////////
int main(int argc, char ** argv) {
		DIR * dp;												//디렉토리 포인터
		char * dir;												//경로 명이 들어갈 변수						
		struct stat st;											//file에 대한 상태 정보를 갖는 변수
		struct dirent * d;										//dentry 구조체
		char path[BUFSIZ + 1];									//(경로명 / dentry의 디렉토리 이름)이 들어감
		BOOL onoff;												//종료 조건

		dir = initPathName(dir);								//변수 초기화
		onoff = TRUE;											//기본값 = TRUE

		///////////////////////////////////////////////////////
		//
		//      * 기본 내용 출력
		//
		///////////////////////////////////////////////////////

		system("clear");

		printMainMenu();

		while(onoff) {
				fflush(stdin);

				scanf("%s", dir);														//경로 설정

				system("clear");

				if(dir == NULL)															//경로가 설정이 안되어있는 경우
						dir = ".";
				else if (strcmp(dir, "exit") == 0) {									//경로명에 exit를 입력 할 경우 종료하기
						onoff = FALSE;
						break;
				}

				if((dp = opendir(dir)) == NULL) {										//디렉터리 열기
						printf("존재하지 않는 디렉토리입니다...\n\n");					//perror은 프로그램 걍 꺼버려서 내용만 출력
						sleep(1);														//내용 보여주고 1초 쉬기
						printf("경로를 다시 입력해 주시기 바랍니다...\n\n");
						printf("\n\n경로 입력(종료는 exit 입력) : ");

						continue;
				}

				///////////////////////////////////////////////////////
				//
				//      * 디렉토리의 각 파일에 대해 파일 경로명 만들기
				//
				///////////////////////////////////////////////////////
				while((d = readdir(dp)) != NULL) {
						sprintf(path, "%s/%s", dir, d->d_name);

						if(lstat(path, &st) < 0)										//존재하지 않는 file이라면??
								perror(path);											//↑↑에러 출력
						else 
								printStat(path, d->d_name, &st);						//상태 정보 출력
				}

				if(dp != NULL)
						closedir(dp);

				printf("\n\n경로 입력(종료는 exit 입력) : ");							//리스트를 출력하고 다시 경로 입력 메뉴 출력
		}
		
		system("clear");

		printf("프로그램을 종료합니다...\n\n");

		exit(0);
}

///////////////////////////////////////////////////////
//
//      * 파일 구조채 내용 출력
//
///////////////////////////////////////////////////////
void printStat(char * pathname, char * file, struct stat * st) {
		printf("%5d ", (int)st->st_blocks);												//블록의 크기
		printf("%c%s ", type(st->st_mode), perm(st->st_mode));							//파일 타입과 퍼미션
		printf("%3d ", (int)st->st_nlink);												//링크 수
		printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);	//소유자 아이디, 그룹 아이디
		printf("%9d ", (int)st->st_size);												//파일 사이즈
		printf("%.12s ", ctime(&st->st_mtime) + 4);										//파일 최종 수정 시간
		printf("%s\n", file);															//파일 이름
}

///////////////////////////////////////////////////////
//
//      * 파일 타입(블록, 캐릭터, 디렉토리 및 여러가지 (총 7가지) 반환하는 함수
//
///////////////////////////////////////////////////////
char type(mode_t mode) {
		if(S_ISREG(mode))
				return ('-');

		if(S_ISDIR(mode))
				return ('d');

		if(S_ISCHR(mode))
				return ('c');

		if(S_ISBLK(mode))
				return ('b');

		if(S_ISLNK(mode))
				return ('l');

		if(S_ISFIFO(mode))
				return ('p');

		if(S_ISSOCK(mode))
				return ('s');
}

///////////////////////////////////////////////////////
//
//      * 해당 파일에 대한 퍼미션 반환하는 함수
//
//////////////////////////////////////////////////////
char * perm(mode_t mode) {
		int i;
		static char perms[10] = "---------";											//파일 퍼미션이 들어갈 변수

		for(i = 0; i < 3; i++) {
				if(mode & (S_IRUSR >> i * 3))											//해당 파일이 읽기가 가능한가??
						perms[i * 3] = 'r';

				if(mode & (S_IWUSR >> i * 3))											//해당 파일이 쓰기가 가능한가??
						perms[i * 3 + 1] = 'w';

				if(mode * (S_IXUSR >> i * 3))											//해당 파일이 실행이 가능한가??
						perms[i * 3 + 2] = 'x';
		}																				//↑↑↑ 전부 체크 후 퍼미션 배열에 체크
		//e.g. -rwxrwx---
		return (perms);
}

///////////////////////////////////////////////////////
//
//      * Path Name에 쓰일 변수 초기화
//
///////////////////////////////////////////////////////
char * initPathName(char * pathName) {
		pathName = (char *)malloc(sizeof(char) * NAMESIZE);

		return pathName;
}

///////////////////////////////////////////////////////
//
//      * 해당 파일에 대한 퍼미션 반환하는 함수
//
//////////////////////////////////////////////////////
void printMainMenu() {
		system("clear");

		printf("□ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □\n");
		printf("□                                                                                       □\n");
		printf("□                             F i l e          E x p l o r e                            □\n");
		printf("□                                                                                       □\n");
		printf("□ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □\n\n\n");

		printf("파일 리스트를 볼 경로를 입력하세요...(경로 명의 길이 1024byte로 제한)\n");
		printf("현재 디렉토리 기준으로 상대 경로를 결정하며, 절대 경로로도 입력이 가능합니다.\n");
		printf("경로 입력(종료는 exit 입력) : ");
}

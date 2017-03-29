/*	남은 스택 출력
	수동 + 자동 모드 고르기 
	-> 테스트 해보기 위함
	주변에 출구가 있다면 그곳으로 바로 탈출
*/

#include "Location2D.h"
#include <Windows.h>
#include <deque>
using namespace std;

extern int** alloc2DInt( int rows, int cols);
extern void free2DInt (int** mat, int rows, int cols=0);

int**	map=NULL;
int		rows=1;
int		cols=0;

int		energy=0;
double	mana=0;

void loadMazeFile(char* filename);
void printMaze();
bool isValidLoc(int r, int c);
bool isExit(int r, int c);

void main() {
	loadMazeFile("maze1.txt");
	printf("%d %d",rows,cols);

	printMaze();

	deque<Location2D> locDeque;
	Location2D entry(0,1);// -> 시작지점 0, 1
	locDeque.push_front(entry);

	getchar();

	while( locDeque.empty() == false ) {
		Location2D here = locDeque.front();
		locDeque.pop_front();
		energy--;			// pop이 한번 일어날때마다(=한번 이동할 때마다) energy가 소모
		mana = mana + 0.1;	// pop이 한번 일어날때마다(=한번 이동할 때마다) mana가 0.1 증가

		int r = here.row;
		int c = here.col;

		if( isExit(r,c) ) {
				printf(" 미로 탐색 성공\n");
				getchar();
				getchar();
				free2DInt (map, rows, cols);
				return;
		}
		else {
			if(map[r][c] == 0) map[r][c] = 9;
			else if(map[r][c] == 9) map[r][c] = 8;
			else if(map[r][c] == 8) map[r][c] = 7;
			else map[r][c] = 6;
			//map[r][c] = 9;	// 한번 지나간길
			

			// 수정필요
			if( isExit(r,c-1) ) {	// 왼쪽
				locDeque.push_front(Location2D(r,c-1));
				continue;
			}
			else if ( isExit(r, c+1) ) {	// 오른쪽
				locDeque.push_front(Location2D(r,c+1));
				continue;
			}
			else if ( isExit(r-1, c) ) {	// 위
				locDeque.push_front(Location2D(r-1,c));
				continue;
			}
			else if ( isExit(r+1, c) ) {	// 아래
				locDeque.push_front(Location2D(r+1,c));
				continue;
			}


			if( isValidLoc(r, c-1) ) {	// 왼쪽
				if( map[r][c] == 0|| map[r][c] == 9) {
					locDeque.push_front(Location2D(r,c));
					locDeque.push_front(Location2D(r,c-1));
				}
			}
			if( isValidLoc(r-1, c) ) {	// 위
				if( map[r][c] == 0|| map[r][c] == 9 ) {
					locDeque.push_front(Location2D(r,c));
					locDeque.push_front(Location2D(r-1,c));
				}
			}
			if( isValidLoc(r+1, c) ) {	// 아래
				if( map[r][c] == 0|| map[r][c] == 9) {
					locDeque.push_front(Location2D(r,c));
					locDeque.push_front(Location2D(r+1,c));
				}
			}
			if( isValidLoc(r, c+1) ) {	// 오른쪽
				if( map[r][c] == 0|| map[r][c] == 9) {
					locDeque.push_front(Location2D(r,c));
					locDeque.push_front(Location2D(r,c+1));
				}
			}


			/*
			if( isValidLoc(r-1, c) ) locDeque.push_front(Location2D(r-1,c));
			if( isValidLoc(r+1, c) ) locDeque.push_front(Location2D(r+1,c));
			if( isValidLoc(r, c-1) ) locDeque.push_front(Location2D(r,c-1));
			if( isValidLoc(r, c+1) ) locDeque.push_front(Location2D(r,c+1));
			*/
		}
		printMaze();
		//Sleep(100);
	}

	printf("미로 탐색 실패\n");
	free2DInt (map, rows, cols);
				
}

bool isValidLoc(int r, int c) {
	if ( r < 0 || c < 0 || r>=rows || c>=cols ) return false;
	else return map[r][c] == 0 ;
}
void printMaze() {
	system("cls");
	printf( "=============================================\n");
	printf( " 전체 미로의 크기 = ");
	printf( "%d %d\n", cols, rows);
	printf( " 에너지 = ");
	printf( "%d\n", energy);
	printf( " 마나 = ");
	printf( "%.1lf\n", mana);
	printf( "=============================================\n");
	for( int i=0 ; i<rows ; i++ ) {
		for( int j=0 ; j<cols ; j++ ) {
			switch (map[i][j]) {
			case 0 : printf( "  " ); break;
			case 1 : printf( "■" ); break;
			case 9 : printf( "①" ); break;
			case 8 : printf( "②" ); break;
			case 7 : printf( "③" ); break;
			case 6 : printf( "④" ); break;		
			}
		} printf("\n");
	}
}

void loadMazeFile(char* filename) {	// 미로를 읽는 함수
	char ch;	// 열은 파일을 읽기 위해 만든 임시 변수.
	FILE *fp = fopen(filename, "r");	// 파일을 연다.

	if ( fp == NULL ) {
		printf(" 파일 %s을 열 수 없습니다.!!!\n", filename);
		return;
	}

	while(fscanf(fp, "%c", &ch) != EOF) {	// 미로 txt파일을 읽는다, 파일이 끝나면 반복문 종료
		if(ch == '\n') rows++;				// txt파일에서 개행문자(enter)를 만나면 열개수를 센다.
		if(ch == '0' || ch == '1') cols++;	// txt파일에서 숫자의 개수를 센다.
	}
	cols = cols/rows;	// 총 숫자의 개수 / 열의 개수 = 행의 개수 
	
	energy = 2 * cols * rows;

	map = alloc2DInt(rows,cols);	// map을 동적할당 받는다.
	fclose(fp);

	/* 
		fopen을 다시 하는 이유 
		위의 while반복문을 통해 한번 열은 fp를 끝까지 다 읽었다.
		내가 아직 다 읽은 파일을 다시 처음부터 읽게 하는법을 몰라서
		일단 새롭게 파일을 열어 다시 읽도록 하였다.
	*/

	FILE *map_read = fopen(filename, "r");
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			fscanf(map_read, "%d", &(map[i][j]));
		}
	}
	fclose(map_read);
}

bool isExit(int r, int c) {
	return ( (r != 0 && c != 1) &&			// 입구가 아니면서
		((r == 0 && map[r][c] == 0) ||		// 윗 벽
		(r == rows-1 && map[r][c] == 0) ||	// 아랫 벽
		(c == 0 && map[r][c] == 0) ||		// 왼쪽 벽
		(c == cols-1 && map[r][c] == 0)) );	// 오른쪽 벽이 0 이라면 출구이므로 true를 반환
}
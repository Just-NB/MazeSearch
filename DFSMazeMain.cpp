#include "Yhs_Location2D.h"
#include <Windows.h>
#include <deque>
using namespace std;
//const		int MAZE_SIZE = 8;
/*
char map[MAZE_SIZE][MAZE_SIZE] = {
{'1', '1', '1', '1', '1', '1', '1', '1', },
{'e', '0', '1', '0', '0', '0', '1', '1', },
{'1', '0', '1', '0', '1', '0', '0', '1', },
{'1', '0', '1', '0', '1', '1', '1', '1', },
{'1', '0', '0', '0', '0', '1', '1', '1', },
{'1', '0', '1', '1', '0', '0', '0', '1', },
{'1', '0', '1', '1', '1', '1', '0', 'x', },	
{'1', '1', '1', '1', '1', '1', '1', '1', }
};
*/
extern int** alloc2DInt( int rows, int cols);
extern void free2DInt (int** mat, int rows, int cols=0);

int**	map=NULL;
int		rows=1;
int		cols=0;

void loadMazeFile(char* filename) {	// 미로를 읽는 함수
	char ch;	// 열은 파일을 읽기 위해 만든 임시 변수.
	FILE *fp = fopen(filename, "r");	// 파일을 연다.

	if ( fp == NULL ) {
		printf(" 파일 %s을 열 수 없습니다.!!!\n", filename);
		return;
	}

	//fscanf(fp, "%d%d", &cols, &rows);
	
	while(fscanf(fp, "%c", &ch) != EOF) {	// 미로 txt파일을 읽는다, 파일이 끝나면 반복문 종료
		if(ch == '\n') rows++;				// txt파일에서 개행문자(enter)를 만나면 열개수를 센다.
		//if(ch == '0' || ch == '1') cols++;	// txt파일에서 숫자의 개수를 센다.
		if(ch == '0' || ch == '1' || ch == '5' || ch == '9') cols++;
	}
	cols = cols/rows;	// 총 숫자의 개수 / 열의 개수 = 행의 개수
	
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

void printMaze() {
	system("cls");
	printf( "=============================================\n");
	printf( " 전체 미로의 크기 = ");
	printf( "%d %d\n", cols, rows);
	printf( "=============================================\n");
	for( int i=0 ; i<rows ; i++ ) {
		for( int j=0 ; j<cols ; j++ ) {
			switch (map[i][j]) {
			case 1 : printf( "■" ); break;
			case 5 : printf( "○" ); break;
			case 9 : printf( "◎" ); break;
			case 0 : printf( "  " ); break;
			case 2 : printf( "□" ); break;

		//	case 0 : printf( "■" ); break;
		//	case 5 : printf( "○" ); break;
		//	case 9 : printf( "◎" ); break;
		//	case 1 : printf( "  " ); break;
		//	case 2 : printf( "□" ); break;
			}
		} printf("\n");
	}
}

CYhs_Location2D findStartLocation() {
	for(int i=0; i<rows ; i++) {
		for(int j=0; j<cols; j++) { 
			if( map[i][j] == 5 ) {
				return CYhs_Location2D(i,j);
			}
		}
	}
	printf(" 미로 파일에서 입구 위치를 찾을 수 없습니다.!!\n");
	return CYhs_Location2D(0,0);
}

bool isValidLoc(int r, int c) {
	//if ( r < 0 || c < 0 || r>=rows || c>=cols ) return false;
	//else return map[r][c] == 1 || map[r][c] == 9 ;
	if ( r < 1 || c < 1 || r>=rows || c>=cols ) return false;
	else return map[r][c] == 0 || map[r][c] == 9 ;

}

void main() {
	loadMazeFile("maze1.txt");
	printf("%d %d",rows,cols);

	printMaze();

	deque<CYhs_Location2D> locDeque;
	// Location2D entry(2,0); -> 시작지점 2,0
	locDeque.push_front(findStartLocation());

	getchar();

	while( locDeque.empty() == false ) {
		CYhs_Location2D here = locDeque.front();
		locDeque.pop_front();

		int r = here.row;
		int c = here.col;

		if(map[r][c] == 9) {
			printf(" 미로 탐색 성공\n");
			getchar();
			getchar();
			return;
		}

		else {
			map[r][c] = 5;
			if( isValidLoc(r-1, c) ) locDeque.push_front(CYhs_Location2D(r-1,c));
			if( isValidLoc(r+1, c) ) locDeque.push_front(CYhs_Location2D(r+1,c));
			if( isValidLoc(r, c-1) ) locDeque.push_front(CYhs_Location2D(r,c-1));
			if( isValidLoc(r, c+1) ) locDeque.push_front(CYhs_Location2D(r,c+1));
		}
		printMaze();
		Sleep(100);
	}

	printf("미로 탐색 실패\n");
}

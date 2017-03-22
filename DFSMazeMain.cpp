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

void loadMazeFile(char* filename) {	// �̷θ� �д� �Լ�
	char ch;	// ���� ������ �б� ���� ���� �ӽ� ����.
	FILE *fp = fopen(filename, "r");	// ������ ����.

	if ( fp == NULL ) {
		printf(" ���� %s�� �� �� �����ϴ�.!!!\n", filename);
		return;
	}

	//fscanf(fp, "%d%d", &cols, &rows);
	
	while(fscanf(fp, "%c", &ch) != EOF) {	// �̷� txt������ �д´�, ������ ������ �ݺ��� ����
		if(ch == '\n') rows++;				// txt���Ͽ��� ���๮��(enter)�� ������ �������� ����.
		//if(ch == '0' || ch == '1') cols++;	// txt���Ͽ��� ������ ������ ����.
		if(ch == '0' || ch == '1' || ch == '5' || ch == '9') cols++;
	}
	cols = cols/rows;	// �� ������ ���� / ���� ���� = ���� ����
	
	map = alloc2DInt(rows,cols);	// map�� �����Ҵ� �޴´�.
	fclose(fp);

	/* 
		fopen�� �ٽ� �ϴ� ���� 
		���� while�ݺ����� ���� �ѹ� ���� fp�� ������ �� �о���.
		���� ���� �� ���� ������ �ٽ� ó������ �а� �ϴ¹��� ����
		�ϴ� ���Ӱ� ������ ���� �ٽ� �е��� �Ͽ���.
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
	printf( " ��ü �̷��� ũ�� = ");
	printf( "%d %d\n", cols, rows);
	printf( "=============================================\n");
	for( int i=0 ; i<rows ; i++ ) {
		for( int j=0 ; j<cols ; j++ ) {
			switch (map[i][j]) {
			case 1 : printf( "��" ); break;
			case 5 : printf( "��" ); break;
			case 9 : printf( "��" ); break;
			case 0 : printf( "  " ); break;
			case 2 : printf( "��" ); break;

		//	case 0 : printf( "��" ); break;
		//	case 5 : printf( "��" ); break;
		//	case 9 : printf( "��" ); break;
		//	case 1 : printf( "  " ); break;
		//	case 2 : printf( "��" ); break;
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
	printf(" �̷� ���Ͽ��� �Ա� ��ġ�� ã�� �� �����ϴ�.!!\n");
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
	// Location2D entry(2,0); -> �������� 2,0
	locDeque.push_front(findStartLocation());

	getchar();

	while( locDeque.empty() == false ) {
		CYhs_Location2D here = locDeque.front();
		locDeque.pop_front();

		int r = here.row;
		int c = here.col;

		if(map[r][c] == 9) {
			printf(" �̷� Ž�� ����\n");
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

	printf("�̷� Ž�� ����\n");
}

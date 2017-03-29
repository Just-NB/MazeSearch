/*	���� ���� ���
	���� + �ڵ� ��� ���� 
	-> �׽�Ʈ �غ��� ����
	�ֺ��� �ⱸ�� �ִٸ� �װ����� �ٷ� Ż��
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
	Location2D entry(0,1);// -> �������� 0, 1
	locDeque.push_front(entry);

	getchar();

	while( locDeque.empty() == false ) {
		Location2D here = locDeque.front();
		locDeque.pop_front();
		energy--;			// pop�� �ѹ� �Ͼ������(=�ѹ� �̵��� ������) energy�� �Ҹ�
		mana = mana + 0.1;	// pop�� �ѹ� �Ͼ������(=�ѹ� �̵��� ������) mana�� 0.1 ����

		int r = here.row;
		int c = here.col;

		if( isExit(r,c) ) {
				printf(" �̷� Ž�� ����\n");
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
			//map[r][c] = 9;	// �ѹ� ��������
			

			// �����ʿ�
			if( isExit(r,c-1) ) {	// ����
				locDeque.push_front(Location2D(r,c-1));
				continue;
			}
			else if ( isExit(r, c+1) ) {	// ������
				locDeque.push_front(Location2D(r,c+1));
				continue;
			}
			else if ( isExit(r-1, c) ) {	// ��
				locDeque.push_front(Location2D(r-1,c));
				continue;
			}
			else if ( isExit(r+1, c) ) {	// �Ʒ�
				locDeque.push_front(Location2D(r+1,c));
				continue;
			}


			if( isValidLoc(r, c-1) ) {	// ����
				if( map[r][c] == 0|| map[r][c] == 9) {
					locDeque.push_front(Location2D(r,c));
					locDeque.push_front(Location2D(r,c-1));
				}
			}
			if( isValidLoc(r-1, c) ) {	// ��
				if( map[r][c] == 0|| map[r][c] == 9 ) {
					locDeque.push_front(Location2D(r,c));
					locDeque.push_front(Location2D(r-1,c));
				}
			}
			if( isValidLoc(r+1, c) ) {	// �Ʒ�
				if( map[r][c] == 0|| map[r][c] == 9) {
					locDeque.push_front(Location2D(r,c));
					locDeque.push_front(Location2D(r+1,c));
				}
			}
			if( isValidLoc(r, c+1) ) {	// ������
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

	printf("�̷� Ž�� ����\n");
	free2DInt (map, rows, cols);
				
}

bool isValidLoc(int r, int c) {
	if ( r < 0 || c < 0 || r>=rows || c>=cols ) return false;
	else return map[r][c] == 0 ;
}
void printMaze() {
	system("cls");
	printf( "=============================================\n");
	printf( " ��ü �̷��� ũ�� = ");
	printf( "%d %d\n", cols, rows);
	printf( " ������ = ");
	printf( "%d\n", energy);
	printf( " ���� = ");
	printf( "%.1lf\n", mana);
	printf( "=============================================\n");
	for( int i=0 ; i<rows ; i++ ) {
		for( int j=0 ; j<cols ; j++ ) {
			switch (map[i][j]) {
			case 0 : printf( "  " ); break;
			case 1 : printf( "��" ); break;
			case 9 : printf( "��" ); break;
			case 8 : printf( "��" ); break;
			case 7 : printf( "��" ); break;
			case 6 : printf( "��" ); break;		
			}
		} printf("\n");
	}
}

void loadMazeFile(char* filename) {	// �̷θ� �д� �Լ�
	char ch;	// ���� ������ �б� ���� ���� �ӽ� ����.
	FILE *fp = fopen(filename, "r");	// ������ ����.

	if ( fp == NULL ) {
		printf(" ���� %s�� �� �� �����ϴ�.!!!\n", filename);
		return;
	}

	while(fscanf(fp, "%c", &ch) != EOF) {	// �̷� txt������ �д´�, ������ ������ �ݺ��� ����
		if(ch == '\n') rows++;				// txt���Ͽ��� ���๮��(enter)�� ������ �������� ����.
		if(ch == '0' || ch == '1') cols++;	// txt���Ͽ��� ������ ������ ����.
	}
	cols = cols/rows;	// �� ������ ���� / ���� ���� = ���� ���� 
	
	energy = 2 * cols * rows;

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

bool isExit(int r, int c) {
	return ( (r != 0 && c != 1) &&			// �Ա��� �ƴϸ鼭
		((r == 0 && map[r][c] == 0) ||		// �� ��
		(r == rows-1 && map[r][c] == 0) ||	// �Ʒ� ��
		(c == 0 && map[r][c] == 0) ||		// ���� ��
		(c == cols-1 && map[r][c] == 0)) );	// ������ ���� 0 �̶�� �ⱸ�̹Ƿ� true�� ��ȯ
}
#pragma once
#include <cstdio>

class CYhs_Location2D
{
public:
	int row;
	int col;
	CYhs_Location2D();
	CYhs_Location2D(int r = 0 , int c = 0) { 
		row = r;
		col = c;
	}
	~CYhs_Location2D(void) 	{ }

	bool isNeighbor( CYhs_Location2D &p ) {
		return( (row==p.row && (col==p.col-1 || col==p.col+1)) ||
					(col==p.col && (row==p.row-1 || row==p.row+1)) );
	}

	bool operator== (CYhs_Location2D &p) { return row==p.row && col==p.col; }

};
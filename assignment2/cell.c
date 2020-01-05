#include "cell.h"
#include <stdlib.h>
#include <assert.h>

struct cell{
	int row;
	int column;
	bool rightWall;
	bool bottomWall;
	bool buildVisit;
	int pathVal;
	CELL *predecessor;
};

CELL *newCELL(void){
	CELL *temp = malloc(sizeof(CELL));
	assert(temp != 0);
	temp->predecessor = NULL;
	temp->row = temp->column = 0;
	temp->rightWall = temp->bottomWall = true;
	temp->buildVisit = false;
	temp->pathVal = -1;
	return temp;
}

void setCELLlocation(CELL *box,int r,int c){
	box->row = r;
	box->column =c;
}

void setCELLbuildVisit(CELL *box,bool state){
	box->buildVisit = state;
}

bool getCELLbuildVisit(CELL *box){
	return box->buildVisit;
}

int getCELLrow(CELL *box){
	return box->row;
}

int getCELLcolumn(CELL *box){
	return box->column;
}

//L = left, R = right, T = top, B = bottom
void setCELLwall(CELL* cell,bool newVal,char side){
	switch(side){
		case 'R':
			cell->rightWall = newVal;
			return;
		case 'B':
			cell->bottomWall = newVal;
			return;
	}
	assert("Invalid wall choice if got here!" == 0);
}

bool getCELLwall(CELL* cell,char side){
	switch(side){
		case 'R':
			return cell->rightWall;
		case 'B':
			return cell->bottomWall;
	}
	assert("Invalid wall choice if got here!" == 0);
}

void setCELLPathVal(CELL* cell,int value){
	cell->pathVal = value;
}

int getCELLPathVal(CELL* cell){
	return cell->pathVal;
}

void setCELLpredecessor(CELL *cell,CELL *from){
	cell->predecessor = from;
}

CELL *getCELLpredecessor(CELL *cell){
	return cell->predecessor;
}

void freeCELL(CELL* cell){
	free(cell);
}
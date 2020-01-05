#include "maze.h"
#include "cell.h"
#include "stack.h"
#include "queue.h"

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

struct maze{
	int rows;
	int columns;
	CELL ***matrix;
};

MAZE *newMAZE(void){
	MAZE *temp = malloc(sizeof(MAZE));
	assert(temp != NULL);
	temp->rows = 0;
	temp->columns = 0;
	temp->matrix = NULL;
	return temp;
}

void setMAZErow(MAZE *puzzle, int r){
	puzzle->rows = r;
}

int getMAZErow(MAZE *puzzle){
	return puzzle->rows;
}

void setMAZEcolumn(MAZE *puzzle, int c){
	puzzle->columns = c;
}

int getMAZEcolumn(MAZE *puzzle){
	return puzzle->columns;
}

void createMAZE(MAZE *puzzle){
	CELL ***temp = malloc(sizeof(CELL **) * puzzle->rows);
	assert(temp != 0);
	for(int i=0;i<puzzle->rows;i++){
		temp[i] = malloc(sizeof(CELL*) * puzzle->columns);
		assert(temp != 0);
		for(int j=0;j<puzzle->columns;j++){
			temp[i][j] = newCELL();
			setCELLlocation(temp[i][j],i,j);
		}
	}
	puzzle->matrix = temp;
}

void buildMAZE(MAZE *puzzle){
	STACK *cells = newSTACK();
	push(cells,puzzle->matrix[0][0]);
	int choice = 0;
	int walls = 0;
	int row = 0;
	int column = 0;
	CELL *temp[4];
	while(sizeSTACK(cells)){
		setCELLbuildVisit(peekSTACK(cells),true);
		row = getCELLrow(peekSTACK(cells));
		column = getCELLcolumn(peekSTACK(cells));
		if(row-1 >= 0){
			if(!getCELLbuildVisit(puzzle->matrix[row-1][column])){
				temp[walls] = puzzle->matrix[row-1][column];
				walls++;
			}
		}
		if(column-1 >= 0){
			if(!getCELLbuildVisit(puzzle->matrix[row][column-1])){
				temp[walls] = puzzle->matrix[row][column-1];
				walls++;
			}
		}
		if(column+1 < puzzle->columns){
			if(!getCELLbuildVisit(puzzle->matrix[row][column+1])){
				temp[walls] = puzzle->matrix[row][column+1];
				walls++;
			}
		}
		if(row+1 < puzzle->rows){
			if(!getCELLbuildVisit(puzzle->matrix[row+1][column])){
				temp[walls] = puzzle->matrix[row+1][column];
				walls++;
			}
		}
		if(walls == 0){
			pop(cells);
		}
		else{ 
			choice = rand()%walls;
			if(row > getCELLrow(temp[choice])){
				setCELLwall(temp[choice],false,'B');
			}
			if(row < getCELLrow(temp[choice])){
				setCELLwall(peekSTACK(cells),false,'B');
			}
			if(column > getCELLcolumn(temp[choice])){
				setCELLwall(temp[choice],false,'R');
			}
			if(column < getCELLcolumn(temp[choice])){
				setCELLwall(peekSTACK(cells),false,'R');	
			}
			walls = 0;
			push(cells,temp[choice]);
		}	
	}
	setCELLwall(puzzle->matrix[puzzle->rows-1][puzzle->columns-1],false,'R');
	freeSTACK(cells);
}

void displayMAZE(MAZE *puzzle,FILE *fp){
	int col = puzzle->columns;
	int row = puzzle->rows;
	for(int i=0;i<(col*4+1);i++){
		fprintf(fp, "-");
	}
	fprintf(fp,"\n");
	for(int i=0;i<row;i++){
		if(i==0){
				fprintf(fp, " ");
		}
		else{
			fprintf(fp,"|");
		}
		for(int j=0;j<col;j++){
			if(getCELLPathVal(puzzle->matrix[i][j]) >= 0){
				fprintf(fp," %d ",getCELLPathVal(puzzle->matrix[i][j]));
			}
			else{
				fprintf(fp, "   ");
			}
			if(getCELLwall(puzzle->matrix[i][j],'R')){
				fprintf(fp, "|");
			}
			else{
				fprintf(fp, " ");
			}
		}
		fprintf(fp, "\n-");
		for(int j=0;j<col;j++){
			if(getCELLwall(puzzle->matrix[i][j],'B')){
				fprintf(fp, "----");
			}
			else{
				fprintf(fp, "   -");
			}
		}
		fprintf(fp, "\n");
	}
}

MAZE *readMAZE(FILE *fp){
	int cols = 0;
	char holder = fgetc(fp);
	while(holder == '-'){
		++cols;
		holder = fgetc(fp);
	}
	if(cols == 0){return NULL;}
	cols = (cols-1)/4;
	MAZE *temp = newMAZE();
	setMAZEcolumn(temp,cols);
	int rows = 0;
	QUEUE *maze = newQUEUE();
	holder = fgetc(fp);
	while(holder != EOF){
		rows++;
		CELL **cells = malloc(sizeof(CELL*)*cols);
		for(int i=0;i<cols;i++){
			cells[i] = newCELL();
			holder = fgetc(fp);
			holder = fgetc(fp);
			if(!isspace(holder)){
				setCELLPathVal(cells[i],holder-'0');
			}
			holder = fgetc(fp);
			holder = fgetc(fp);
			if(holder == '|'){
				setCELLwall(cells[i],true,'R');
			}
			else{
				setCELLwall(cells[i],false,'R');
			}
			setCELLlocation(cells[i],rows-1,i);
		}
		holder = fgetc(fp);
		holder = fgetc(fp);
		for(int i=0;i<cols;i++){
			for(int j=0;j<4;j++){
				holder = fgetc(fp);
				if(j==0){
					if(holder == '-'){
						setCELLwall(cells[i],true,'B');
					}
					else{
						setCELLwall(cells[i],false,'B');
					}
				}
			}
		}
		enqueue(maze,cells);
		holder = fgetc(fp);
		holder = fgetc(fp);
	}
	setMAZErow(temp,rows);
	temp->matrix = malloc(sizeof(CELL**)*rows);
	for(int i=0;i<rows;i++){
		temp->matrix[i] = dequeue(maze);
	}
	freeQUEUE(maze);
	return temp;
}

void solveMAZE(MAZE *puzzle){
	QUEUE *cells = newQUEUE();
	enqueue(cells,puzzle->matrix[0][0]);
	CELL *currentCell;
	CELL *endCell = puzzle->matrix[getMAZErow(puzzle)-1][getMAZEcolumn(puzzle)-1];
	setCELLPathVal(peekQUEUE(cells),0);
	while(sizeQUEUE(cells)){
		currentCell = dequeue(cells);
		if(currentCell == endCell){
			while(sizeQUEUE(cells)){
				setCELLPathVal(dequeue(cells),-1);
			}
			break;
		}
		int row = getCELLrow(currentCell);
		int col = getCELLcolumn(currentCell);
		if(row-1>=0 && 
			!(getCELLwall(puzzle->matrix[row-1][col],'B')) &&
			getCELLPathVal(puzzle->matrix[row-1][col])<0){

			enqueue(cells,puzzle->matrix[row-1][col]);
			setCELLPathVal(puzzle->matrix[row-1][col],
				(getCELLPathVal(currentCell)+1)%10);
			//printf("%d\n", count %10 );
			setCELLpredecessor(puzzle->matrix[row-1][col],currentCell);
			//currentCell = puzzle->matrix[row-1][col];
		}
		if(col-1>=0 &&
			!(getCELLwall(puzzle->matrix[row][col-1],'R')) &&
			getCELLPathVal(puzzle->matrix[row][col-1])<0){

			enqueue(cells,puzzle->matrix[row][col-1]);
			setCELLPathVal(puzzle->matrix[row][col-1],
				(getCELLPathVal(currentCell)+1)%10);
			setCELLpredecessor(puzzle->matrix[row][col-1],currentCell);
			//currentCell = puzzle->matrix[row][col-1];
			//printf("%d\n", count %10 );
		}
		if(col+1 < getMAZEcolumn(puzzle) &&
			!(getCELLwall(currentCell,'R')) &&
			getCELLPathVal(puzzle->matrix[row][col+1]) < 0){

			enqueue(cells,puzzle->matrix[row][col+1]);
			setCELLPathVal(puzzle->matrix[row][col+1],
				(getCELLPathVal(currentCell)+1)%10);
			setCELLpredecessor(puzzle->matrix[row][col+1],currentCell);
			//currentCell = puzzle->matrix[row][col+1];
			//printf("%d\n", count %10 );
		}
		if(row+1 < getMAZErow(puzzle) &&
			!(getCELLwall(currentCell,'B')) &&
			getCELLPathVal(puzzle->matrix[row+1][col]) < 0){

			enqueue(cells,puzzle->matrix[row+1][col]);
			setCELLPathVal(puzzle->matrix[row+1][col],
				(getCELLPathVal(currentCell)+1)%10);
			setCELLpredecessor(puzzle->matrix[row+1][col],currentCell);
			//currentCell = puzzle->matrix[row+1][col];
			//printf("%d\n", count %10 );
		}
	}
	freeQUEUE(cells);
}

void freeMAZE(MAZE *puzzle){
	if(puzzle->matrix){
		for(int i=0;i<puzzle->rows;i++){
			for(int j=0;j<puzzle->columns;j++){
				freeCELL(puzzle->matrix[i][j]);
			}
			free(puzzle->matrix[i]);
		}
		free(puzzle->matrix);
	}
	free(puzzle);
}
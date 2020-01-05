#include "processOptions.h"
#include <assert.h>
#include <stdlib.h>

struct process{
	bool author;
	bool solve;
	bool create;
	bool draw;
	unsigned int seed;
	int rows;
	int columns;
	int displayFile;
	int readFile;
	int writeFile;
	int mazeFile;
};

PROCESS *newPROCESS(void){
	PROCESS *temp = malloc(sizeof(PROCESS));
	assert(temp != 0);
	temp->author=temp->solve=temp->create=temp->draw=false;
	temp->seed = 1;
	temp->rows=temp->columns=0;
	temp->readFile=temp->writeFile=temp->mazeFile=temp->displayFile=0;
	return temp;
}

bool AuthorOption(PROCESS *option){
	return option->author;
}

bool SolveOption(PROCESS *option){
	return option->solve;
}

bool CreateOption(PROCESS *option){
	return option->create;
}

bool DrawOption(PROCESS *option){
	return option->draw;	
}

int rowsOption(PROCESS *option){
	return option->rows;
}

int columnsOption(PROCESS *option){
	return option->columns;
}

int readFileOption(PROCESS *option){
	return option->readFile;
}

int writeFileOption(PROCESS *option){
	return option->writeFile;
}

int mazeFileOption(PROCESS *option){
	return option->mazeFile;
}

int displayFileOption(PROCESS *option){
	return option->displayFile;
}

int ProcessOptions(int argc, char **argv, PROCESS *option){
    int argIndex = 1;

    while((argIndex < argc)&&(argv[argIndex][0] == '-')){
    	//printf(" Arg - %d\n",argIndex);
        switch(argv[argIndex][1]){
        	case 'v':
        		option->author = true;
				return 0;
			case 's':
				option->solve = true;
				argIndex++;
				if(argc > argIndex){
					option->readFile = argIndex++;
					option->writeFile = argIndex;
				}
				break;
			case 'c':
				option->create = true;
				argIndex++;
				if(argc-1 > argIndex){
					option->rows = atoi(argv[argIndex++]);
					option->columns = atoi(argv[argIndex++]);
					option->mazeFile = argIndex;
				}
				//printf("%d %d %d\n",option->rows,option->columns,option->mazeFile );
				break;
			case 'r':
				argIndex++;
				if(argc >= argIndex){
					option->seed = atoi(argv[argIndex]);
				}
				break;
			case 'd':
				option->draw = true;
				if(argc > argIndex){
					argIndex++;
					option->displayFile = argIndex;
				}
				//printf(option->draw ? "true":"false");
				//printf(" %d\n",option->readFile);
				break;
			default:
				fprintf(stdout, "Invalid argument!\n");
				return 0;			    
        }
        argIndex ++;
    }
    //printf("%d %d %d %d\n",option->displayFile,option->readFile,option->writeFile,option->mazeFile );
    return 1;
}

void freePROCESS(PROCESS *option){
	free(option);
}

unsigned int getSeed(PROCESS *option){
	return option->seed;
}
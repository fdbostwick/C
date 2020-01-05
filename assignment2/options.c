#include "options.h"
#include <assert.h>
#include <stdlib.h>

struct process{
	bool author;
	char tree;
};

PROCESS *newPROCESS(void){
	PROCESS *temp = malloc(sizeof(PROCESS));
	assert(temp != 0);
	temp->author = false;
	temp->tree = 'r';
	return temp;
}

bool AuthorOption(PROCESS *option){
	return option->author;
}

char TreeType(PROCESS *option){
	return option->tree;
}

int ProcessOptions(int argc, char **argv, PROCESS *option){
    int argIndex = 1;
    //fprintf(stdout, "%s\n",argv[argIndex] );
    while((argIndex < argc)&&(argv[argIndex][0] == '-')){
    	//printf(" Arg - %d\n",argIndex);
        switch(argv[argIndex][1]){
        	case 'v':
        		option->author = true;
        		return argIndex;
			case 'g':
			{
				option->tree = 'g';
				argIndex++;
			}
				break;
			case 'r':
				argIndex++;
				break;
			default:
				argIndex++;
				break;
        }
    }
    //printf("%d %d %d %d\n",option->displayFile,option->readFile,option->writeFile,option->mazeFile );
    return argIndex;
}

void freePROCESS(PROCESS *option){
	free(option);
}
#include "maze.h"
#include "processOptions.h"

int main(int argv,char **argc){
	PROCESS *options = newPROCESS();
	if(ProcessOptions(argv,argc,options)){
		srand(getSeed(options));
		FILE *readFile;
		FILE *writeFile;
		MAZE *puzzle;

		if(CreateOption(options)){
			puzzle = newMAZE();
			setMAZErow(puzzle,rowsOption(options));
			setMAZEcolumn(puzzle,columnsOption(options));
			createMAZE(puzzle);
			buildMAZE(puzzle);
			if(argv > 4){
				writeFile = fopen(argc[mazeFileOption(options)],"w");
			}
			else{
				writeFile = NULL;
			}
			if(writeFile != NULL){
				displayMAZE(puzzle,writeFile);
			}
			else{
				printf("Error opening file!\n");
				freeMAZE(puzzle);
				freePROCESS(options);
				return 0;
			}
			fclose(writeFile);
			freeMAZE(puzzle);
		}
		if(SolveOption(options)){
			readFile = fopen(argc[readFileOption(options)],"r");
			writeFile = fopen(argc[writeFileOption(options)],"w");
			puzzle = readMAZE(readFile);
			solveMAZE(puzzle);
			displayMAZE(puzzle,writeFile);
			fclose(readFile);
			fclose(writeFile);
			freeMAZE(puzzle);
		}
		if(DrawOption(options)){
			readFile = fopen(argc[displayFileOption(options)],"r");
			puzzle = readMAZE(readFile);
			displayMAZE(puzzle,stdout);
			fclose(readFile);
			freeMAZE(puzzle);
		}
	}
	else{
		if(AuthorOption(options)){
			printf("Written by Frederick Bostwick\n");
		}
		else{
			printf("Invalid arguments!");
		}
	}
	freePROCESS(options);
	return 0;
}







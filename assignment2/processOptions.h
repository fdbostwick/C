#ifndef __PROCESSOPTIONS_INLCUDED__
#define __PROCESSOPTIONS_INLCUDED__

#include <stdio.h>
#include <stdbool.h>

typedef struct process PROCESS;

extern PROCESS  *newPROCESS(void);
extern bool 	AuthorOption(PROCESS*);
extern bool		SolveOption(PROCESS*);
extern bool 	CreateOption(PROCESS*);
extern bool 	DrawOption(PROCESS*);
extern int 		rowsOption(PROCESS *option);
extern int 		columnsOption(PROCESS *options);
extern int 		readFileOption(PROCESS *option);
extern int		writeFileOption(PROCESS *option);
extern int		mazeFileOption(PROCESS*);
extern int		displayFileOption(PROCESS*);
extern int	 	ProcessOptions(int,char**,PROCESS*);
extern void 	freePROCESS(PROCESS*);
extern unsigned int	getSeed(PROCESS*);

#endif
#ifndef __OPTIONS_INCLUDED__
#define __OPTIONS_INCLUDED__

#include <stdio.h>
#include <stdbool.h>

typedef struct process PROCESS;

extern PROCESS  *newPROCESS(void);
extern bool 	AuthorOption(PROCESS*);
extern char     TreeType(PROCESS*);
extern int	 	ProcessOptions(int,char**,PROCESS*);
extern void 	freePROCESS(PROCESS*);

#endif
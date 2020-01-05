#ifndef __CELL_INCLUDED__
#define __CELL_INCLUDED__

#include <stdio.h>
#include <stdbool.h>

typedef struct cell CELL;

extern CELL *newCELL(void);
extern void setCELLlocation(CELL*,int r,int c);
extern void setCELLbuildVisit(CELL*,bool);
extern bool getCELLbuildVisit(CELL*);
extern int getCELLrow(CELL*);
extern int getCELLcolumn(CELL*);
extern void setCELLwall(CELL*,bool,char);
extern bool getCELLwall(CELL*,char);
extern void setCELLPathVal(CELL*,int);
extern int getCELLPathVal(CELL*);
extern void setCELLpredecessor(CELL *cell,CELL *from);
extern CELL *getCELLpredecessor(CELL *cell);
extern void freeCELL(CELL*);

#endif
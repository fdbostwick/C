#ifndef __MAZE_INCLUDED__
#define __MAZE_INCLUDED__

#include <stdio.h>
#include <stdlib.h>

typedef struct maze MAZE;

extern MAZE *newMAZE(void);
extern void setMAZErow(MAZE *, int);
extern int getMAZErow(MAZE *);
extern void setMAZEcolumn(MAZE *, int);
extern int getMAZEcolumn(MAZE *);
extern void createMAZE(MAZE *);
extern void buildMAZE(MAZE*);
extern void displayMAZE(MAZE*,FILE*);
extern MAZE *readMAZE(FILE*);
extern void solveMAZE(MAZE*);
extern void freeMAZE(MAZE*);



#endif
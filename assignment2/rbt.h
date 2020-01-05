//Red Black Binary Search Tree

#ifndef __RBT_INCLUDED__
#define __RBT_INCLUDED__

#include <stdio.h>
#include "tnode.h"

typedef struct rbt RBT;

extern RBT 		*newRBT(int(*c)(void*,void*));//must pass in a comparator function
extern void 	setRBTdisplay(RBT*,void(*d)(void*,FILE*));//pass in a function to display items
extern void 	setRBTswapper(RBT*,void(*s)(TNODE*,TNODE*));//pass in a function that swaps values between nodes
extern void 	setRBTfree(RBT*,void(*f)(void*));//pass in a function that frees value in node
extern TNODE 	*getRBTroot(RBT*);
extern void		setRBTroot(RBT*,TNODE*);
extern void		setRBTsize(RBT*,int);
extern TNODE 	*insertRBT(RBT*,void* val);
extern void 	*findRBT(RBT*,void *key);
extern TNODE 	*locateRBT(RBT*,void *key);
extern int 		deleteRBT(RBT*,void *key);
extern TNODE 	*swapToLeafRBT(RBT*,TNODE*);
extern void 	pruneLeafRBT(RBT*,TNODE*);
extern int 		sizeRBT(RBT*);
extern void 	statisticsRBT(RBT*,FILE*);
extern void 	displayRBT(RBT*,FILE*);
extern int 		debugRBT(RBT*,int level);
extern void 	freeRBT(RBT*);
extern void 	*unwrapRBT(TNODE*);
extern int 		freqRBT(RBT*,void *key);
extern int 		duplicatesRBT(RBT*);

#endif

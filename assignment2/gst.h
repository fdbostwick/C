//Green Binary Search Tree
//efficient storage of duplicates by using a frequency value

#ifndef __GST_INCLUDE__
#define __GST_INCLUDE__

#include <stdio.h>
#include "tnode.h"

typedef struct gst GST;

extern GST 		*newGST(int(*c)(void*,void*));
extern void 	setGSTdisplay(GST*,void(*d)(void*,FILE*));
extern void 	setGSTswapper(GST*,void(*s)(TNODE*,TNODE*));
extern void 	setGSTfree(GST*,void(*f)(void*));
extern TNODE 	*getGSTroot(GST*);
extern void 	setGSTroot(GST*,TNODE*);
extern void 	setGSTsize(GST*,int);
extern TNODE 	*insertGST(GST*,void*);
extern void 	*findGST(GST*,void*);
extern TNODE  	*locateGST(GST*,void*);
extern int 		deleteGST(GST*,void*);
extern TNODE 	*swapToLeafGST(GST*,TNODE*);
extern void 	pruneLeafGST(GST*,TNODE*);
extern int 		sizeGST(GST*);
extern void 	statisticsGST(GST*,FILE*);
extern void 	displayGST(GST*,FILE*);
extern int 		debugGST(GST*,int);
extern void 	freeGST(GST*);

//Extensions of BST
extern void 	*unwrapGST(TNODE*);
extern int 		freqGST(GST*,void*);
extern int 		duplicatesGST(GST*);

#endif
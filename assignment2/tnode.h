#ifndef __TNODE_INCLUDED__
#define __TNODE_INCLUDED__

#include <stdio.h>

typedef struct tnode TNODE;

extern TNODE *newTNODE(void*,TNODE *lft,TNODE *prnt,TNODE *rght);
extern void	 setTNODEdisplay(TNODE*,void(*d)(void*,FILE*));
extern void  setTNODEfree(TNODE*,void(*f)(void*));
extern void  *getTNODEvalue(TNODE*);
extern void  setTNODEvalue(TNODE*,void*);
extern TNODE *getTNODEleft(TNODE*);
extern void  setTNODEleft(TNODE*,TNODE *lft);
extern TNODE *getTNODEright(TNODE*);
extern void  setTNODEright(TNODE*,TNODE *rght);
extern TNODE *getTNODEparent(TNODE*);
extern void  setTNODEparent(TNODE*,TNODE *prnt);
extern void  displayTNODE(TNODE*,FILE*);
extern int	 debugTNODE(TNODE*,int);
extern void  freeTNODE(TNODE*);

#endif 	

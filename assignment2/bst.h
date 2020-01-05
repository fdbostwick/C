#ifndef __BST_INCLUDED__
#define __BST_INCLUDED__

#include <stdio.h>
#include "tnode.h"

typedef struct bst BST;

//Must pass a comparator function when creating BST
//so it knows how to handle the values
//If a display function is not added it displays the address 
//of the pointer to the node value
//If a free function is set then it will free the underlying
//values
//If no swapper function is set the program sets one that
//swaps the value pointer in the node
extern BST 	 *newBST(int(*c)(void*,void*));
extern void	 setBSTdisplay(BST*,void(*d)(void*,FILE*));
extern void  setBSTswapper(BST*,void(*s)(TNODE*,TNODE*));
extern void  setBSTfree(BST*,void(*f)(void*));
extern TNODE *getBSTroot(BST*);
extern void  setBSTroot(BST*,TNODE*);
extern void  setBSTsize(BST*,int);
extern TNODE *insertBST(BST*,void*);
extern void  *findBST(BST*,void*);
extern TNODE *locateBST(BST*,void*);
extern int   deleteBST(BST*,void*);
extern TNODE *swapToLeafBST(BST*,TNODE*);
extern void  pruneLeafBST(BST*,TNODE*);
extern int   sizeBST(BST*);
extern void  statisticsBST(BST*,FILE*);
extern void  displayBST(BST*,FILE*);
extern int   debugBST(BST*,int);
extern void  freeBST(BST*);

#endif
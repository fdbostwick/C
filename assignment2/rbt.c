#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tnode.h"
#include "rbt.h"
#include "gst.h"

typedef struct rbtnode RBTNODE;

struct rbt{
	GST *root;
	void(*free)(void*);
	void(*display)(void*,FILE*);
	int(*compare)(void*,void*);
	void(*swap)(TNODE*,TNODE*);
};

struct rbtnode{
	RBT *home;
	void *value;
	int color;//0 = black 1 = red	
};

static int compareRBTNODE(void *a,void *b){
	RBTNODE *c = a;
	RBTNODE *d = b;
	return c->home->compare(c->value,d->value);
}

static void displayRBTNODE(void *node,FILE *fp){
	RBTNODE *val = node;
	val->home->display(val->value,fp);
	if(val->color > 0){
		fprintf(fp, "*");
	}
}

static void swapRBT(TNODE* a,TNODE *b){
	void *aa = getTNODEvalue(a);
	void *bb = getTNODEvalue(b);
	setTNODEvalue(a,bb);
	setTNODEvalue(b,aa);
	RBTNODE *s = unwrapGST(a);
	RBTNODE *t = unwrapGST(b);
	int col = s->color;
	s->color = t->color;
	t->color = col; 
}

RBTNODE *newRBTNODE(void *val,RBT *tree){
	RBTNODE *temp = malloc(sizeof(RBTNODE));
	assert(temp != 0);
	temp->value = val;
	temp->home = tree;
	temp->color = 1;//always create as a red node;
	return temp;
}

RBT	*newRBT(int(*c)(void*,void*)){
	RBT *temp = malloc(sizeof(RBT));
	assert(temp != 0);
	temp->compare = c;
	temp->free = 0;
	temp->display = 0;
	temp->swap = swapRBT;
	temp->root = newGST(compareRBTNODE);
	setGSTdisplay(temp->root,displayRBTNODE);
	setGSTswapper(temp->root,swapRBT);
	return temp;
}

void setRBTdisplay(RBT *tree,void(*d)(void*,FILE*)){
	tree->display = d;
}

void setRBTswapper(RBT *tree,void(*s)(TNODE*,TNODE*)){
	tree->swap = s;
	setGSTswapper(tree->root,s);
}

void setRBTfree(RBT *tree,void(*f)(void*)){
	tree->free = f;
	setGSTfree(tree->root,f);
}

TNODE *getRBTroot(RBT *tree){
	return getGSTroot(tree->root);
}

void setRBTroot(RBT *tree,TNODE *node){
	setGSTroot(tree->root,node);
}

void setRBTsize(RBT *tree,int s){
	setGSTsize(tree->root,s);
}

static RBTNODE *uncle(TNODE *node){
	TNODE *parent = getTNODEparent(node);
	if(parent == getTNODEparent(parent)){
		return 0;
	}
	TNODE *grand = getTNODEparent(parent);
	if(parent == getTNODEleft(grand)){
		if(getTNODEright(grand) != 0){
			return unwrapGST(getTNODEright(grand));
		}		
	}
	else if(parent == getTNODEright(grand)){
		if(getTNODEleft(grand) != 0){
			return unwrapGST(getTNODEleft(grand));
		}
	}
	return 0;
}

static int color(RBTNODE *node){
	if(node == 0){
		return 0;
	}
	else{
		return node->color;
	}
}

static int checkLinear(TNODE *node){
	TNODE *parent = getTNODEparent(node);
	TNODE *grandparent = getTNODEparent(parent);
	if(parent == getTNODEleft(grandparent)){
		if(node != getTNODEleft(parent)){
			return 0;//is not linear
		}
		else{
			return 1;//is linear
		}
	}
	if(parent == getTNODEright(grandparent)){
		if(node != getTNODEright(parent)){
			return 0;
		}
		else{
			return 1;
		}
	}
	return 1;
}

static void rotateRight(RBT *tree,TNODE *node){
	//fprintf(stdout, "before right...\n");
	//displayRBT(tree,stdout);
	TNODE *child = getTNODEparent(node);
	TNODE *upperRoot = getTNODEparent(child);
	setTNODEleft(child,getTNODEright(node));
	if(getTNODEleft(child) != 0){
		setTNODEparent(getTNODEleft(child),child);
	}
	setTNODEparent(child,node);
	if(child == getRBTroot(tree)){
		setRBTroot(tree,node);
		setTNODEparent(node,node);
	}
	else{
		if(child == getTNODEleft(upperRoot)){
			setTNODEleft(upperRoot,node);
		}
		else{
			setTNODEright(upperRoot,node);
		}
		setTNODEparent(node,upperRoot);
	}
	setTNODEright(node,child);
	//fprintf(stdout, "after right...\n");
	//displayRBT(tree,stdout);	
}

static void rotateLeft(RBT *tree,TNODE *node){
	//fprintf(stdout, "before left...\n");
	//displayRBT(tree,stdout);
	TNODE *child = getTNODEparent(node);
	TNODE *upperRoot = getTNODEparent(child);
	setTNODEright(child,getTNODEleft(node));
	if(getTNODEright(child) != 0){
		setTNODEparent(getTNODEright(child),child);
	}
	setTNODEparent(child,node);
	if(child == getRBTroot(tree)){
		setRBTroot(tree,node);
		setTNODEparent(node,node);
	}
	else{
		if(child == getTNODEleft(upperRoot)){
			setTNODEleft(upperRoot,node);
		}
		else{
			setTNODEright(upperRoot,node);
		}
		setTNODEparent(node,upperRoot);
	}
	setTNODEleft(node,child);
	//fprintf(stdout, "after left...\n");
	//displayRBT(tree,stdout);
}

static void insertRBTfix(TNODE *node,RBT *tree){
	TNODE *temp;
	RBTNODE *rbtNode;
	//fprintf(stdout, "before fix - \n");
	//displayRBT(tree,stdout);
	while((color(unwrapGST(getTNODEparent(node))) != 0) && (node != getRBTroot(tree))){
		//fprintf(stdout, "node value - ");
		//tree->display(unwrapRBT(node),stdout);
		//fprintf(stdout, "\n");
		if(color(uncle(node)) == 1){
			//fprintf(stdout, "uncle red...\n");
			temp = getTNODEparent(node);
			if(temp != 0){
				rbtNode = unwrapGST(temp);
				rbtNode->color = 0;
			}
			rbtNode = uncle(node);
			rbtNode->color = 0;
			temp = getTNODEparent(temp);
			if(temp != 0){
				rbtNode = unwrapGST(temp);
				rbtNode->color = 1;
			}
			node = temp;
		}
		else{
			TNODE *parent = getTNODEparent(node);
			//fprintf(stdout, "uncle black...\n");
			if(checkLinear(node) == 0){
				//fprintf(stdout, "Not Linear\n");
				if(node == getTNODEleft(parent)){
					rotateRight(tree,node);
				}
				else{
					rotateLeft(tree,node);
				}
				temp = parent;
				parent = node;
				node = temp;
				//displayRBT(tree,stdout);
			}
			if(parent != 0){
				rbtNode = unwrapGST(parent);
				rbtNode->color = 0;
			}
			temp = getTNODEparent(parent);
			if(temp != 0){
				rbtNode = unwrapGST(temp);
				rbtNode->color = 1;
			}		
			//displayRBT(tree,stdout);
			if(parent == getTNODEleft(getTNODEparent(parent))){
				//fprintf(stdout, "rotate black uncle\n");
				rotateRight(tree,parent);
			}
			else if(parent == getTNODEright(getTNODEparent(parent))){
				//fprintf(stdout, "rotating with black uncle...\n");
				rotateLeft(tree,parent);
			}
			temp = getRBTroot(tree);
			if(temp != 0){
				rbtNode = unwrapGST(temp);
				rbtNode->color = 0;
			}
			return;
		}
	}
	temp = getRBTroot(tree);
	if(temp != 0){
		rbtNode = unwrapGST(temp);
		rbtNode->color = 0;
	}
	//fprintf(stdout, "after fix - \n" );
	//displayRBT(tree,stdout);
}

TNODE *insertRBT(RBT *tree,void* val){
	//fprintf(stdout, "freq before insert - %d\n", freqRBT(tree,val));
	RBTNODE *temp = newRBTNODE(val,tree);
	setGSTfree(tree->root,0);
	TNODE *node = insertGST(tree->root,temp);
	setGSTfree(tree->root,tree->free);
	if(freqRBT(tree,val) == 1){
		insertRBTfix(node,tree);
	}
	else{
		//fprintf(stdout, "freq after insert - %d\n",freqRBT(tree,val));
		if(tree->free != 0){
			tree->free(val);
		}
		free(temp);
		return 0;
	}
	return node;
}

void *findRBT(RBT *tree,void *key){
	TNODE *node = locateRBT(tree,key);
	if(node != 0){
		return unwrapRBT(node);
	}
	return 0;
}

TNODE *locateRBT(RBT *tree,void *key){
	RBTNODE *temp = newRBTNODE(key,tree);
	TNODE *locate = locateGST(tree->root,temp);
	free(temp);
	return locate;	
}

static TNODE *sibling(TNODE *node){
	TNODE *parent = getTNODEparent(node);
	//RBTNODE *temp = unwrapGST(parent);
	//fprintf(stdout, "parent value - ");
	//temp->home->display(temp->value,stdout);
	//fprintf(stdout, "\n");
	if(getTNODEleft(parent) == node){
		node = getTNODEright(parent);
	}
	else{
		node = getTNODEleft(parent);
	}
	/*if(node != 0){
		temp = unwrapGST(node);
		fprintf(stdout, "sibling value - ");
		temp->home->display(temp->value,stdout);
		fprintf(stdout, "\n");
	}*/
	return node;
}

static TNODE *nephew(TNODE *node){
	TNODE *parent = getTNODEparent(node);
	if(node == getTNODEright(parent)){
		if(getTNODEleft(parent) != 0){
			node = getTNODEleft(getTNODEleft(parent));
		}
		else{
			node = 0;
		}
	}
	else if(node == getTNODEleft(parent)){
		if(getTNODEright(parent) != 0){
			node = getTNODEright(getTNODEright(parent));
		}
		else{
			node = 0;
		}
	}
	else{
		node = 0;
	}
	/*if(node != 0){
		RBTNODE *temp = unwrapGST(node);
		fprintf(stdout, "nephew value - ");
		temp->home->display(temp->value,stdout);
		fprintf(stdout, "\n");
	}*/
	return node;
}

static TNODE *niece(TNODE *node){
	TNODE *parent = getTNODEparent(node);
	if(node == getTNODEright(parent)){
		if(getTNODEleft(parent) != 0){
			node = getTNODEright(getTNODEleft(parent));
		}
		else{
			node = 0;
		}
	}
	else if(node == getTNODEleft(parent)){
		if(getTNODEright(parent) != 0){
			node = getTNODEleft(getTNODEright(parent));
		}
		else{
			node = 0;
		}
	}
	else{
		node = 0;
	}
	/*if(node != 0){
		RBTNODE *temp = unwrapGST(node);
		fprintf(stdout, "niece value - ");
		temp->home->display(temp->value,stdout);
		fprintf(stdout, "\n");
	}*/
	return node;
}

static void deleteRBTfix(TNODE *node,RBT *tree){
	RBTNODE *rbtNode;
	TNODE *temp = node;
	while(node != 0 && sizeRBT(tree) > 0){
		if(node == getRBTroot(tree)){
			break;
		}
		if(node != 0 && color(unwrapGST(node)) == 1){
			break;
		}
		//check sibling's color
		//displayRBT(tree,stdout);
		//fprintf(stdout, "node value - ");
		//tree->display(unwrapRBT(node),stdout);
		//fprintf(stdout, "\n");
		if(node != 0 && color(unwrapGST(sibling(node))) == 1){
			//fprintf(stdout, "sibling red!\n");
			temp = getTNODEparent(node);
			if(temp != 0){
				rbtNode = unwrapGST(temp);
			    rbtNode->color = 1;
			}
			temp = sibling(node);
			if(temp != 0){
				rbtNode = unwrapGST(temp);
				rbtNode->color = 0;
			}
			TNODE *parent = getTNODEparent(node);
			if(sibling(node) == getTNODEleft(parent)){
				//fprintf(stdout, "am i here?\n");
				rotateRight(tree,sibling(node));
			}
			else{
				rotateLeft(tree,sibling(node));
			}
		}
		else if(nephew(node) != 0 && color(unwrapGST(nephew(node))) == 1){
			//fprintf(stdout, "nephew red!!\n");
			temp = sibling(node);
			if(temp != 0){
				rbtNode = unwrapGST(temp);
				TNODE *parent = getTNODEparent(node);
				rbtNode->color = color(unwrapGST(parent));
				rbtNode = unwrapGST(parent);
				rbtNode->color = 0;
			}
			temp = nephew(node);
			if(temp != 0){
				rbtNode = unwrapGST(temp);
				rbtNode->color = 0;
			}
			if(sibling(node) == getTNODEleft(getTNODEparent(sibling(node)))){
				rotateRight(tree,sibling(node));
			}
			else{
				rotateLeft(tree,sibling(node));
			}
			break;
		}
		else if(niece(node) != 0 && color(unwrapGST(niece(node))) == 1){
			//fprintf(stdout, "niece red!!\n");
			rbtNode = unwrapGST(niece(node));
			rbtNode->color = 0;
			rbtNode = unwrapGST(sibling(node));
			rbtNode->color = 1;
			if(niece(node) == getTNODEleft(sibling(node))){
				rotateRight(tree,niece(node));
			}
			else{
				rotateLeft(tree,niece(node));
			}
		}
		else{
			temp = sibling(node);
			if(temp != 0){
				rbtNode = unwrapGST(temp);
				rbtNode->color = 1;
			}
			node = getTNODEparent(node);
		}
	}
	if(node != 0){
		rbtNode = unwrapGST(node);
		rbtNode->color = 0;
	}
}

int	deleteRBT(RBT *tree,void *key){
	TNODE *node = locateRBT(tree,key);
	if(node == 0)return -1;
	RBTNODE *temp = unwrapGST(node);
	int freq = freqRBT(tree,key);
	if(freq == 1){
		node = swapToLeafRBT(tree,node);
		deleteRBTfix(node,tree);
		pruneLeafRBT(tree,node);
		freeTNODE(node);
		setGSTsize(tree->root,sizeGST(tree->root)-1);
	}
	if(freq > 1){
		freq = deleteGST(tree->root,temp);
	}
	return freq;
}

TNODE *swapToLeafRBT(RBT *tree,TNODE *node){
	return swapToLeafGST(tree->root,node);
}

void pruneLeafRBT(RBT *tree,TNODE *node){
	free(unwrapGST(node));
	setGSTfree(tree->root,0);
	pruneLeafGST(tree->root,node);
	setGSTfree(tree->root,tree->free);
}

int	sizeRBT(RBT *tree){
	return sizeGST(tree->root);
}

void statisticsRBT(RBT *tree,FILE *fp){
	statisticsGST(tree->root,fp);
}

void displayRBT(RBT *tree,FILE *fp){
	displayGST(tree->root,fp);
}

int debugRBT(RBT *tree,int level){
	return debugGST(tree->root,level);
}

static void freeHelper(TNODE *node){
	if(node == 0){
		return;
	}
	freeHelper(getTNODEleft(node));
	freeHelper(getTNODEright(node));
	RBTNODE *temp = unwrapGST(node);
	if(temp->home->free != 0){
		temp->home->free(temp->value);
	}
	free(temp);
}

void freeRBT(RBT *tree){
	freeHelper(getRBTroot(tree));
	setGSTfree(tree->root,0);
	freeGST(tree->root);
	free(tree);
}

void *unwrapRBT(TNODE *node){
	RBTNODE *temp = unwrapGST(node);
	void *val = 0;
	if(temp != 0){
		val = temp->value;
	}
	return val;
}

int	freqRBT(RBT *tree,void *key){
	RBTNODE *temp = newRBTNODE(key,tree);
	int freq = freqGST(tree->root,temp);
	//fprintf(stdout, "freq - %d\n", freq );
	free(temp);
	return freq;
}

int	duplicatesRBT(RBT *tree){
	return duplicatesGST(tree->root);
}
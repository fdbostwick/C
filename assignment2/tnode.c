#include "tnode.h"
#include <assert.h>
#include <stdlib.h>

struct tnode{
	int debug;
	void *value;
	TNODE *left;
	TNODE *right;
	TNODE *parent;
	void (*display)(void*,FILE*);
	void (*free)(void*);
};

TNODE *newTNODE(void *val,TNODE *lft,TNODE *prnt,TNODE *rght){
	TNODE *temp = malloc(sizeof(TNODE));
	assert(temp != NULL);
	temp->debug = 0;
	temp->value = val;
	temp->left = lft;
	temp->right = rght;
	temp->parent = prnt;
	temp->display = 0;
	temp->free = 0;
	return temp;
}

void setTNODEdisplay(TNODE *node,void(*d)(void*,FILE*)){
	node->display=d;
}

void setTNODEfree(TNODE *node,void(*f)(void*)){
	node->free=f;
}

void *getTNODEvalue(TNODE *node){
	return node->value;
}

void setTNODEvalue(TNODE *node,void *val){
	node->value=val;
}

TNODE *getTNODEleft(TNODE *node){
	return node->left;
}

void setTNODEleft(TNODE *node,TNODE *lft){
	node->left=lft;
}

TNODE *getTNODEright(TNODE *node){
	return node->right;
}

void setTNODEright(TNODE *node,TNODE *rght){
	node->right=rght;
}

TNODE *getTNODEparent(TNODE *node){
	return node->parent;
}

void setTNODEparent(TNODE *node,TNODE *prnt){
	node->parent=prnt;
}

void displayTNODE(TNODE *node,FILE *fp){
	if(node->display == NULL || node->debug > 0){
		fprintf(fp, "@%p",node);
	}
	else{
		node->display(node->value,fp);
	}
}

int debugTNODE(TNODE *node,int level){
	int temp = node->debug;
	node->debug = level;
	return temp;
}

void freeTNODE(TNODE *node){
	if(node->free){
		free(node->value);
	}
	free(node);
}
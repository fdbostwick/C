#include "bst.h"
#include "tnode.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct bst{
	int(*compare)(void*,void*);
	void(*display)(void*,FILE*);
	void(*free)(void*);
	void(*swap)(TNODE*,TNODE*);
	int size;
	int minHeight;
	int maxHeight;
	int debug;
	TNODE *root;
};

static void swapper(TNODE *original,TNODE *next){
	assert(original != 0 && next != 0);
	void *val = getTNODEvalue(original);
	setTNODEvalue(original,getTNODEvalue(next));
	setTNODEvalue(next,val);
}

BST *newBST(int(*c)(void*,void*)){
	BST *temp = malloc(sizeof(BST));
	assert(temp != 0);
	//Make a generic swapper function
	temp->compare = c;
	temp->display = 0;
	temp->free = 0;
	temp->root = 0;
	temp->minHeight = 400000;
	temp->maxHeight = -1;
	temp->debug = 0;
	temp->swap = swapper;
	temp->size = 0;
	return temp;
}

void setBSTdisplay(BST *tree,void(*d)(void*,FILE*)){
	tree->display=d;
}

void setBSTswapper(BST *tree,void(*s)(TNODE*,TNODE*)){
	tree->swap=s;
}

void setBSTfree(BST *tree,void(*f)(void*)){
	tree->free=f;
}

TNODE *getBSTroot(BST *tree){
	return tree->root;
}

void setBSTroot(BST *tree,TNODE *node){
	tree->root = node;
	setTNODEparent(node,node);
}

void setBSTsize(BST *tree,int s){
	tree->size = s;
}

TNODE *insertBST(BST *tree,void *val){
	TNODE *temp = newTNODE(val,0,0,0);
	if(tree->root == 0){
		setBSTroot(tree,temp);
	}
	else{
		TNODE *check = tree->root;
		TNODE *prnt = 0;
		while(check != 0){
			prnt = check;
			if(tree->compare(val,getTNODEvalue(check))<0){
				check = getTNODEleft(check);
			}
			else{
				check = getTNODEright(check);
			}
		}
		if(tree->compare(val,getTNODEvalue(prnt))<0){
			setTNODEleft(prnt,temp);
		}
		else{
			setTNODEright(prnt,temp);
		}
		setTNODEparent(temp,prnt);
	}
	tree->size++;
	return temp;
}

void *findBST(BST *tree,void *value){
	TNODE *check = locateBST(tree,value);
	if(check != 0){
		return getTNODEvalue(check);
	}
	return 0;
}

TNODE *locateBST(BST *tree,void *value){
	TNODE *check = tree->root;
	while(check != 0){
		if(tree->compare(value,getTNODEvalue(check))==0){
			return check;
		}
		if(tree->compare(value,getTNODEvalue(check))<0){
			check = getTNODEleft(check);
		}
		else{
			check = getTNODEright(check);
		}
	}
	return 0;
}

int deleteBST(BST *tree,void *val){
	TNODE *node = locateBST(tree,val);
	if(node != 0){
		node = swapToLeafBST(tree,node);
		pruneLeafBST(tree,node);
		freeTNODE(node);
		tree->size--;
		//debugBST(tree,0);
		//displayBST(tree,stdout);
		return 0;
	}
	return -1;
}

static TNODE *goLeft(TNODE *node){
	if(getTNODEleft(node) != 0){
		node = goLeft(getTNODEleft(node));
	}
	return node;
}

static TNODE *goRight(TNODE *node){
	if(getTNODEright(node) != 0){
		node = goRight(getTNODEright(node));
	}
	return node;
}

TNODE *swapToLeafBST(BST *tree,TNODE *node){
	TNODE *original = node;
	if(original != 0){
		while(getTNODEleft(node) != 0 || getTNODEright(node) != 0){
			if(getTNODEleft(node) != 0){
				node = goRight(getTNODEleft(node));
			}
			else{
				node = goLeft(getTNODEright(node));
			}
			tree->swap(original,node);
			original = node;
		}
	}
	return node;
}

void pruneLeafBST(BST *tree,TNODE *node){	
	if(node == tree->root){
		tree->root = NULL;
	}
	else if(getTNODEleft(getTNODEparent(node)) == node){
		setTNODEleft(getTNODEparent(node),NULL);
	}
	else{
		setTNODEright(getTNODEparent(node),NULL);
	}
}

int sizeBST(BST *tree){
	return tree->size;
}

static int minBST(TNODE *node){
	if(node == 0){
		return -1;
	}
	else{
		int left = minBST(getTNODEleft(node));
		int right = minBST(getTNODEright(node));
		if(left < right){
			return left + 1;
		}
		else{
			return right + 1;
		}
	}
}

static int maxBST(TNODE *node){
	if(node == 0){
		return -1;
	}
	int left = maxBST(getTNODEleft(node));
	int right = maxBST(getTNODEright(node));
	if(left > right){
		return left + 1;
	}
	else{
		return right + 1;
	}
}

static void height(BST *tree){
	tree->minHeight = minBST(tree->root);
	tree->maxHeight = maxBST(tree->root);
}

void statisticsBST(BST *tree,FILE *fp){
	height(tree);
	fprintf(fp, "Nodes: %d\n", tree->size);
	fprintf(fp, "Minimum depth: %d\n", tree->minHeight);
	fprintf(fp, "Maximum depth: %d\n", tree->maxHeight);
}

static void inOrderBST(TNODE *node,FILE *fp,BST *tree){
	if(node == NULL){
		return;
	}
	fprintf(fp, "[");
	inOrderBST(getTNODEleft(node),fp,tree);
	if(getTNODEleft(node) != 0){
		fprintf(fp, " ");
	}
	setTNODEdisplay(node,tree->display);
	displayTNODE(node,fp);
	if(getTNODEright(node) != 0){
		fprintf(fp, " ");
	}
	inOrderBST(getTNODEright(node),fp,tree);
	fprintf(fp, "]");
}

static void preOrderBST(TNODE *node,FILE *fp,BST *tree){
	if(node == NULL){
		return;
	}
	if(node != tree->root){
		fprintf(fp, " ");
	}
	fprintf(fp, "[");
	setTNODEdisplay(node,tree->display);
	displayTNODE(node,fp);
	preOrderBST(getTNODEleft(node),fp,tree);
	preOrderBST(getTNODEright(node),fp,tree);
	fprintf(fp, "]");
}

static void postOrderBST(TNODE *node,FILE *fp,BST *tree){
	if(node == NULL){
		return;
	}
	fprintf(fp, "[");
	postOrderBST(getTNODEleft(node),fp,tree);
	postOrderBST(getTNODEright(node),fp,tree);
	setTNODEdisplay(node,tree->display);
	displayTNODE(node,fp);
	fprintf(fp, "]");
	if(node != tree->root){
		fprintf(fp, " ");
	}
}

void displayBST(BST *tree,FILE *fp){
	if(tree->size == 0){
		if(tree->debug != 0){
			fprintf(fp, "[]");
		}
		else{
			fprintf(fp, "0:\n");
		}
		return;
	}
	if(tree->debug == 1){
		//in-order traversal
		inOrderBST(tree->root,fp,tree);	
	}
	else if(tree->debug == 2){
		//pre-order traversal
		preOrderBST(tree->root,fp,tree);
	}
	else if(tree->debug == 3){
		//post-order traversal
		postOrderBST(tree->root,fp,tree);
	}
	else{
		//level-order traversal (use a queue)
		QUEUE *nodes = newQUEUE();
		enqueue(nodes,tree->root);
		TNODE *node = 0;
		TNODE *l = 0;
		TNODE *r = 0;
		TNODE *p = 0;
		height(tree);
		//fprintf(fp, "min: %d, max: %d\n",tree->minHeight,tree->maxHeight);
		int max = tree->maxHeight+1;
		for(int i=0;i<max;i++){
			enqueue(nodes,tree);
			fprintf(fp, "%d: ",i);
			while(peekQUEUE(nodes) != tree){
				node = dequeue(nodes);
				l = getTNODEleft(node);
				if(l != 0){
					enqueue(nodes,l);
				}
				r = getTNODEright(node);
				if(r != 0){
					enqueue(nodes,r);
				}
				p = getTNODEparent(node);
				if(r == 0 && l == 0){
					fprintf(fp, "=");
				}
				setTNODEdisplay(node,tree->display);
				displayTNODE(node,fp);
				fprintf(fp, "(");
				setTNODEdisplay(p,tree->display);
				displayTNODE(p,fp);
				fprintf(fp, ")");
				if(tree->root == node){
					fprintf(fp, "X");
				}
				else if(getTNODEleft(p) == node){
					fprintf(fp, "L");
				}
				else{
					fprintf(fp, "R");
				}
				if(peekQUEUE(nodes) != tree){
					fprintf(fp, " ");
				}
			}
			dequeue(nodes);
			fprintf(fp, "\n");
		}
		freeQUEUE(nodes);
	}
}

int debugBST(BST *tree,int level){
	int temp = tree->debug;
	tree->debug = level;
	return temp;
}

static void freeHelp(TNODE *node,BST *tree){
	if(node == 0){
		return;
	}
	freeHelp(getTNODEleft(node),tree);
	freeHelp(getTNODEright(node),tree);
	if(tree->free != 0){
		tree->free(getTNODEvalue(node));
	}
	freeTNODE(node);
}

void freeBST(BST *tree){
	freeHelp(tree->root,tree);
	free(tree);
}
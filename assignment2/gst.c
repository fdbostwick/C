#include "gst.h"
#include "bst.h"
#include "tnode.h"
#include <stdlib.h>
#include <assert.h>

//Actual value and frequency of node pointed to by TNODE
typedef struct gstnode GSTNODE;

struct gstnode{
	GST *home;
	int frequency;
	void *value;
};

struct gst{
	void(*free)(void*);
	void(*display)(void*,FILE*);
	void(*swap)(TNODE*,TNODE*);
	int(*compare)(void*,void*);
	int insertions;
	BST *tree;
};

static void swapGST(TNODE *original,TNODE *replace){
	assert(original != 0 && replace != 0);
	GSTNODE *origin = getTNODEvalue(original);
	setTNODEvalue(original,getTNODEvalue(replace));
	setTNODEvalue(replace,origin);
}

static int compareGSTNODE(void *a,void *b){
	GSTNODE *c = a;
	GSTNODE *d = b;
	return c->home->compare(c->value,d->value);
}

static void displayGSTNODE(void *node,FILE *fp){
	GSTNODE *val = node;
	val->home->display(val->value,fp);
	if(val->frequency > 1){
		fprintf(fp, "<%d>",val->frequency);
	}
}

GSTNODE *newGSTNODE(GST *g,void *v){
	GSTNODE *temp = malloc(sizeof(GSTNODE));
	assert(temp != 0);
	temp->frequency = 1;
	temp->value = v;
	temp->home = g;
	return temp;
}

GST	*newGST(int(*c)(void*,void*)){
	GST *temp = malloc(sizeof(GST));
	assert(temp != 0);
	temp->free = 0;
	temp->display = 0;
	temp->swap = swapGST;
	temp->compare = c;
	temp->tree = newBST(compareGSTNODE);
	temp->insertions = 0;
	setBSTswapper(temp->tree,swapGST);
	setBSTdisplay(temp->tree,displayGSTNODE);
	return temp;
}

void setGSTdisplay(GST *green,void(*d)(void*,FILE*)){
	green->display = d;
}

void setGSTswapper(GST *green,void(*s)(TNODE*,TNODE*)){
	green->swap = s;
	setBSTswapper(green->tree,s);
}

void setGSTfree(GST *green,void(*f)(void*)){
	green->free = f;
	setBSTfree(green->tree,f);
}

TNODE *getGSTroot(GST *green){
	return getBSTroot(green->tree);
}

void setGSTroot(GST *green,TNODE *node){
	setBSTroot(green->tree,node);
}

void setGSTsize(GST *green,int s){
	setBSTsize(green->tree,s);
}

TNODE *insertGST(GST *green,void *val){
	TNODE *temp = locateGST(green,val);
	GSTNODE *node = 0;
	if(temp != 0){
		node = getTNODEvalue(temp);
		node->frequency++;
		if(green->free != 0){
			green->free(val);
		}
	}
	else{
		node = newGSTNODE(green,val);
		temp = newTNODE(node,0,0,0);
		if(sizeGST(green) == 0){
			setBSTroot(green->tree,temp);
		}
		else{
			TNODE *check = getBSTroot(green->tree);
			TNODE *prnt = 0;
			while(check != 0){
				prnt = check;
				if(green->compare(val,unwrapGST(check))<0){
					check = getTNODEleft(check);
				}
				else{
					check = getTNODEright(check);
				}
			}
			if(green->compare(val,unwrapGST(prnt))<0){
				setTNODEleft(prnt,temp);
			}
			else{
				setTNODEright(prnt,temp);
			}
			setTNODEparent(temp,prnt);
		}
		setBSTsize(green->tree,sizeBST(green->tree)+1);
	}
	green->insertions++;
	return temp;
}

void *findGST(GST *green,void *key){
	TNODE *temp = locateGST(green,key);
	if(temp != 0){
		return unwrapGST(temp);
	}
	return 0;
}

TNODE *locateGST(GST *green,void *key){
	TNODE *check = getGSTroot(green);
	while(check != 0){
		if(green->compare(key,unwrapGST(check)) == 0){
			return check;
		}
		if(green->compare(key,unwrapGST(check)) < 0){
			check = getTNODEleft(check);
		}
		else{
			check = getTNODEright(check);
		}
	}
	return 0;
}

int	deleteGST(GST *green,void *key){
	TNODE *temp = locateGST(green,key);
	if(temp == 0){return -1;}
	GSTNODE *check = getTNODEvalue(temp);
	if(check->frequency > 1){
		check->frequency--;
		green->insertions--;
		return check->frequency;
	}
	temp = swapToLeafGST(green,temp);
	pruneLeafGST(green,temp);
	freeTNODE(temp);
	setBSTsize(green->tree,sizeBST(green->tree)-1);
	return 0;
}

TNODE *swapToLeafGST(GST *green,TNODE *node){
	return swapToLeafBST(green->tree,node);
}

void pruneLeafGST(GST *green,TNODE *node){
	pruneLeafBST(green->tree,node);
	free(getTNODEvalue(node));
	green->insertions--;
}

int	sizeGST(GST *green){
	return sizeBST(green->tree);
}

void statisticsGST(GST *green,FILE *fp){
	fprintf(fp, "Duplicates: %d\n", duplicatesGST(green));
	statisticsBST(green->tree,fp);
}

void displayGST(GST *green,FILE *fp){
	displayBST(green->tree,fp);
}

int	debugGST(GST *green,int level){
	int temp = debugBST(green->tree,level);
	return temp;
}

static void freeHelp(TNODE *node,GST *green){
	if(node == 0){
		return;
	}
	freeHelp(getTNODEleft(node),green);
	freeHelp(getTNODEright(node),green);
	GSTNODE *temp = getTNODEvalue(node);
	if(green->free != 0){
		green->free(temp->value);
	}
	free(temp);	
}

void freeGST(GST *green){
	freeHelp(getGSTroot(green),green);
	setBSTfree(green->tree,0);
	freeBST(green->tree);
	free(green);
}

//Extensions of BST
void *unwrapGST(TNODE *node){
	void *val = 0;
	if(node != 0){
		GSTNODE *temp = getTNODEvalue(node);
		val = temp->value;
	}
	return val;
}

int	freqGST(GST *green,void *key){
	TNODE *find = locateGST(green,key);
	int temp = 0;
	if(find != 0){
		GSTNODE *val = getTNODEvalue(find);
		temp = val->frequency;
	}
	return temp;
}

int	duplicatesGST(GST *green){
	return (green->insertions - sizeBST(green->tree));
}
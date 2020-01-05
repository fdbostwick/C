#include "stack.h"
#include "da.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

struct stack{
	DA *store;
	void (*display)(void *,FILE*);
	int debug;
};

STACK *newSTACK(void){
	STACK *temp = malloc(sizeof(STACK));
	assert(temp != 0);
	temp->debug = 0;
	temp->display = NULL;
	temp->store = newDA();
	return temp;
}

void setSTACKdisplay(STACK *items,void(*d)(void*,FILE*)){
	items->display = d;
	setDAdisplay(items->store,d);
}

void setSTACKfree(STACK *items,void(*f)(void*)){
	setDAfree(items->store,f);
}

void push(STACK *items,void *value){
	insertDAback(items->store,value);
}

void *pop(STACK *items){
	assert(sizeDA(items->store) > 0);
	return removeDAback(items->store);
}

void *peekSTACK(STACK *items){
	assert(sizeDA(items->store) > 0);
	return getDA(items->store,sizeDA(items->store)-1);
}

int sizeSTACK(STACK *items){
	return sizeDA(items->store);
}

void displaySTACK(STACK *items,FILE *fp){
	if(items->debug == 1){
		displayDA(items->store,fp);	
	}
	else if(items->debug == 2){
		int old = debugDA(items->store,1);
		displayDA(items->store,fp);
		debugDA(items->store,old);
	}
	else{
		fprintf(fp,"|");
		int size = sizeDA(items->store)-1;
		for(int i=size;i>=0;i--){
			items->display(getDA(items->store,i),fp);
	  		if(i > 0){
	    		fprintf(fp, ",");
	   		}
		}
		fprintf(fp, "|");
	}
}

int debugSTACK(STACK *items,int level){
	int temp = items->debug;
	items->debug = level;
	return temp;
}

void freeSTACK(STACK *items){
	freeDA(items->store);
	free(items);
}
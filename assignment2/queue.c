#include "queue.h"
#include "cda.h"
#include <assert.h>
#include <stdlib.h>

struct queue{
	CDA *store;
	void (*display)(void *,FILE*);
	int debug;
};

QUEUE *newQUEUE(void){
	QUEUE *temp = malloc(sizeof(QUEUE));
	assert(temp != 0);
	temp->debug = 0;
	temp->display = NULL;
	temp->store = newCDA();
	return temp;
}

void  setQUEUEdisplay(QUEUE* items,void(*d)(void*,FILE*)){
	items->display = d;
	setCDAdisplay(items->store,d);
}

void  setQUEUEfree(QUEUE* items,void(*f)(void*)){
	setCDAfree(items->store,f);
}

void enqueue(QUEUE *items,void *value){
	insertCDAback(items->store,value);
}

void *dequeue(QUEUE *items){
	assert(sizeCDA(items->store) > 0);
	return removeCDAfront(items->store);
}

void *peekQUEUE(QUEUE *items){
	assert(sizeCDA(items->store) > 0);
	return getCDA(items->store,0);
}

int sizeQUEUE(QUEUE *items){
	return sizeCDA(items->store);
}

void displayQUEUE(QUEUE *items,FILE *fp){
	if(items->debug == 1){
		displayCDA(items->store,fp);	
	}
	else if(items->debug == 2){
		int old = debugCDA(items->store,1);
		displayCDA(items->store,fp);
		debugCDA(items->store,old);
	}
	else{
		fprintf(fp,"<");
		int size = sizeCDA(items->store);
		for(int i=0;i<size;i++){
			items->display(getCDA(items->store,i),fp);
	  		if(i < (size-1)){
	    		fprintf(fp, ",");
	   		}
		}
		fprintf(fp, ">");
	}
}

int debugQUEUE(QUEUE *items,int level){
	int temp = items->debug;
	items->debug = level;
	return temp;
}

void freeQUEUE(QUEUE *items){
	freeCDA(items->store);
	free(items);
}

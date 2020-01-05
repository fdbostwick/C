#include "da.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

struct da{
	int size;
	int capacity;
	int debugFlag;
	void **dynArray;
	void (*display)(void *,FILE*);
	void (*free)(void *);
};

DA *newDA(void){
	void **temp = malloc(sizeof(void*));
	assert(temp != NULL);
	DA *newAry = malloc(sizeof(DA));
	assert(newAry != 0);
	newAry->size = 0;
	newAry->capacity = 1;
	newAry->debugFlag = 0;
	newAry->dynArray = temp;
	newAry->display = NULL;
	newAry->free = NULL;
	return newAry;
}

void setDAdisplay(DA *temp,void(*show)(void *x,FILE *fp)){
	temp->display = show;
}

void setDAfree(DA *temp,void(*release)(void *x)){
	temp->free = release;
}

static void growArray(DA *items){
	int cap = items->capacity * 2;
	void **newArray = realloc(items->dynArray,sizeof(void*) * cap);
	assert(newArray != 0);
	items->dynArray = newArray;
	items->capacity = cap;
	return;
}

void insertDA(DA *items,int index,void *value){
	assert(index >= 0);
	assert(index <= items->size);
	//checkSize at start
	if(items->size == items->capacity){
		growArray(items);
	}
	if(index == items->size){
		items->dynArray[index] = value;
	}
	else{
		for(int i=items->size;i>index;i--){
			items->dynArray[i] = items->dynArray[i-1];
		}
		items->dynArray[index] = value;
	}
	items->size++;
}

static void shrinkArray(DA *items){
	int cap = 0;
	if(items->size == 0){
		cap = 1;
	}
	else{
		cap = items->capacity / 2;
	}
	void **newArray = realloc(items->dynArray,sizeof(void*) * cap);
	assert(newArray != 0);
	items->dynArray = newArray;
	if(items->size == 0){
		items->capacity = 1;
	}
	else{
		items->capacity = cap;
	}
	return;
}

void *removeDA(DA *items,int index){
	assert(items->size > 0);
	assert(index >= 0);
	assert(index < items->size);
	void *temp = items->dynArray[index];
	for(int i=index;i<(items->size-1);i++){
		items->dynArray[i] = items->dynArray[i+1];
	}
	//checkSize at end 
	items->size--;
	double check = items->size/(double)items->capacity;
	//fprintf(stdout, " %f", check);
	if((items->size == 0)||(check < 0.25)){
		shrinkArray(items);
	}
	return temp;
}

void unionDA(DA *recipient,DA *donor){
	int ctr = 0;//used in union for loop
	int i = recipient->size;//used for union for loop
	recipient->size += donor->size;
	while(recipient->size >= recipient->capacity){
		growArray(recipient);
	}
	for(;i<recipient->size;i++){
		recipient->dynArray[i] = donor->dynArray[ctr++];
	}
	donor->size = 0;
	donor->capacity = 1;
	donor->dynArray = realloc(donor->dynArray,sizeof(void*));
}

void *getDA(DA *items,int index){
	assert(index >= 0 && index < items->size);
	return items->dynArray[index];
}

void *setDA(DA *items,int index,void *value){
	assert(index >= 0 && index <= items->size);
	if(index == items->size){
		insertDA(items,index,value);
		return NULL;
	}
	void *temp = items->dynArray[index];
	items->dynArray[index] = value;
	return temp;
}

int sizeDA(DA *items){
	return items->size;
}

void displayDA(DA *items,FILE *fp){
	fprintf(fp,"[");
	for(int i=0;i<items->size;i++){
		if(items->display){
			items->display(items->dynArray[i],fp);
	    }
	    else{
	    	fprintf(fp, "@%p", items->dynArray[i]);	    	
	    }
	    if(i < (items->size-1)){
	    	fprintf(fp, ",");
	    }
	}
	if(items->debugFlag > 0){
		int emptySlots = items->capacity - items->size;
		if(items->size > 0){
			fprintf(fp, ",");
		}
		fprintf(fp,"[%d]", emptySlots);
	}
	fprintf(fp, "]");
	//fprintf(fp," %d",items->size);
}

int debugDA(DA *items,int level){
	int temp = items->debugFlag;
	items->debugFlag = level;
	return temp;
}

/*static void freeItems(DA *items,int index){
	if(index < items->size){
		items->free(items->dynArray[index]);
		freeItems(items,index+1);
	}
}*/

void freeDA(DA *items){
	if(items->free != NULL){
		//freeItems(items,0);
		for(int i=0;i<items->size;i++){
			items->free(items->dynArray[i]);
		}
	}
	free(items->dynArray);
	free(items);
}
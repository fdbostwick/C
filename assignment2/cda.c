#include "cda.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

struct cda {
	int zeroIndex;
	int size;
	int capacity;
	int debugFlag;
	void **dynArray;
	void (*display)(void *,FILE*);
	void (*free)(void *);
};

CDA *newCDA(void){
	void **tempArray = malloc(sizeof(void*));
	assert(tempArray != 0);
	CDA *tempCDA = malloc(sizeof(CDA));
	assert(tempArray != 0);
	tempCDA->zeroIndex = 0;
	tempCDA->size = 0;
	tempCDA->capacity = 1;
	tempCDA->debugFlag = 0;
	tempCDA->dynArray = tempArray;
	tempCDA->display = NULL;
	tempCDA->free = NULL;
	return tempCDA;
}

void setCDAdisplay(CDA *items,void(*show)(void*,FILE*)){
	items->display = show;
}

void setCDAfree(CDA *items,void(*remove)(void*)){
	items->free = remove;
}

static void growArray(CDA *items){
	int cap = items->capacity * 2;
	void **newArray = malloc(sizeof(void*) * cap);
	assert(newArray != 0);
	for(int i=0;i<items->size;i++){
		newArray[i] = getCDA(items,i);
	}
	free(items->dynArray);
	items->dynArray = newArray;
	items->zeroIndex = 0;
	items->capacity = cap;
	return;
}

void insertCDA(CDA *items,int index,void *value){
	assert(index >= 0);
	assert(index <= items->size);
	//check size at start
	if(items->size == items->capacity){
		growArray(items);
	}
	if(index == 0){
		if(items->size == 0){
			setCDA(items,0,value);
		}
		else{
			setCDA(items,-1,value);
		} 
	}
	else{
		if(index < (items->size/2)){
			void *temp = getCDA(items,0);
			for(int i=0;i<index-1;i++){
				setCDA(items,i,getCDA(items,i+1));
			}
			setCDA(items,index-1,value);
			setCDA(items,-1,temp);
		}
		else{
			for(int i=items->size;i>index;i--){
				setCDA(items,i,getCDA(items,i-1));	
			}
			setCDA(items,index,value);
		}
	}
}

static void shrinkArray(CDA *items){
	if(items->size == 0){
		items->dynArray = realloc(items->dynArray,sizeof(void*));
		items->capacity = 1;
		items->zeroIndex = 0;
	}
	else{
		void **temp = malloc(sizeof(void*) * items->capacity/2);
		assert(temp != 0);
		for(int i=0;i<items->size;i++){
			temp[i] = getCDA(items,i);
		}
		free(items->dynArray);		
		items->dynArray = temp;
		items->capacity = items->capacity/2;
		items->zeroIndex = 0;
	}
}

void *removeCDA(CDA *items,int index){
	assert(index >= 0);
	assert(index < items->size);
	void *temp = getCDA(items,index);	
	if(index < items->size/2){
		for(int i=index;i>0;i--){
			setCDA(items,i,getCDA(items,i-1));
		}
		items->zeroIndex++;
		if(items->zeroIndex == items->capacity){
			items->zeroIndex -= items->capacity;
		}
	}
	else{
		for(int i=index;i<items->size-1;i++){
			setCDA(items,i,getCDA(items,i+1));
		}
	}
	items->size--;
	double check = items->size/(double)items->capacity;
	if((items->size == 0)||(check < 0.25)){
		shrinkArray(items);
	}
	return temp;
}

void unionCDA(CDA *recipient,CDA *donor){
	while((sizeCDA(recipient)+sizeCDA(donor)) >= recipient->capacity){
		growArray(recipient);
	}
	for(int i=0;i<donor->size;i++){
		setCDA(recipient,sizeCDA(recipient),getCDA(donor,i));
	}
	donor->size = 0;
	donor->zeroIndex = 0;
	donor->dynArray = realloc(donor->dynArray,sizeof(void*));
	donor->capacity = 1;
}

void *getCDA(CDA *items,int index){
	assert(index >= 0);
	assert(index < items->size);
	return items->dynArray[(index+items->zeroIndex)%items->capacity];
}

void *setCDA(CDA *items,int index,void *value){
	assert(index >= -1);
	assert(index <= items->size);
	void *temp = NULL;
	if((index == items->size)||(index == -1)){
		if(items->size == items->capacity){
			growArray(items);
		}
		if(index == -1){
			items->zeroIndex--;
			if(items->zeroIndex < 0){
				items->zeroIndex = items->capacity + items->zeroIndex;
			}
			items->dynArray[items->zeroIndex] = value;
		}
		else{
			int realEnd = (index+items->zeroIndex)%items->capacity;
			items->dynArray[realEnd] = value;
		}
		items->size++;
	}
	else{
		int realSpot = (index+items->zeroIndex)%items->capacity;
		temp = items->dynArray[realSpot];
		items->dynArray[realSpot] = value;
	}
	return temp;
}

int sizeCDA(CDA *items){
	return items->size;
}

void displayCDA(CDA *items,FILE *fp){
	//fprintf(fp, "size - %d  capacity - %d", items->size,items->capacity);
	fprintf(fp,"(");
	for(int i=0;i<items->size;i++){
		if(items->display){
			items->display(getCDA(items,i),fp);
	    }
	    else{
	    	fprintf(fp, "@%p", getCDA(items,i));	    	
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
		fprintf(fp,"(%d)", emptySlots);
	}
	fprintf(fp, ")");
}

int debugCDA(CDA *items,int level){
	int temp = items->debugFlag;
	items->debugFlag = level;	
	return temp;
}

void freeCDA(CDA *items){
	if(items->free != NULL){
		while(items->size){
			items->free(removeCDAfront(items));
		}
	}
	free(items->dynArray);
	free(items);
}
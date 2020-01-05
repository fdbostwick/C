#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int *makeTable(int s){
	int *t = malloc(sizeof(int) * s);
	assert(t != 0);
	return t;
}

int getTable(int i,int *t){
	if(i == 0){
		return 0;
	}
	if(i == 1){
		return 1;
	}
	return t[i];
}

int fib(int n){
	int *t = makeTable(n+1);
	for(int i=0;i<=n;++i){
		t[i] = getTable(n-1,t) + getTable(n-2,t);
	}
	return t[n];
}


int main(void){
	int num = 0;
	fscanf("Enter an integer to get the fib value of it: %d",num);
	fprintf(stdout,"Fib number %d is %d",num,fib(num));
	return 0;
}
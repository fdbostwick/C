#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "string.h"

struct STRING{
    char *value;
};

STRING *newSTRING(char *x){
    STRING *p = malloc(sizeof(STRING));
    assert(p != 0);
    p->value = x;
    return p;
}

char* getSTRING(STRING *v){
    return v->value;
}

char* setSTRING(STRING *v,char *x){
    char *old = v->value;
    if(sizeof(x) <= sizeof(v->value)){
        v->value = x;
    }
    else{
        free(v->value);
        char *temp = malloc(sizeof(x)+1);
        v->value = temp;
    }
    return old;
}

void displaySTRING(void *v,FILE *fp){
    fprintf(fp,"%s",getSTRING((STRING *) v));
}

int compareSTRING(void *v,void *w){
    char *a = getSTRING(v);
    char *b = getSTRING(w);
    if(strcmp(a,b) == 0){
        return 0;
    }
    else if(strcmp(a,b) < 0){
        return -1;
    }
    return 1;
}

void freeSTRING(void *v){
    free((STRING *) v);
}
#include "gst.h"
#include "rbt.h"
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "options.h"
#include "string.h"
#include "scanner.h"

void readCorpusGST(FILE*,GST*);
void readCommandsGST(FILE*,GST*);
void readCorpusRBT(FILE*,RBT*);
void readCommandsRBT(FILE*,RBT*);
char *cleanWord(char*);

int main(int argc,char** argv){
	PROCESS *option = newPROCESS();
	int index = ProcessOptions(argc,argv,option);
	FILE *corpus = 0;
	FILE *commands = 0;
	if(AuthorOption(option)){
		fprintf(stdout, "Author - Frederick Bostwick\n");
		freePROCESS(option);
		return 0;
	}
	if(argc >= index){
		//fprintf(stdout, "%s\n",argv[index]);
		corpus = fopen(argv[index++],"r");
		assert(corpus != 0);
		if(argc >= index){
			//fprintf(stdout, "%s\n",argv[index]);
			commands = fopen(argv[index],"r");
			assert(commands != 0);
		}
	}
	if(TreeType(option) == 'g'){
		GST *gTree = newGST(compareSTRING);
		setGSTdisplay(gTree,displaySTRING);
		setGSTfree(gTree,freeSTRING);
		if(corpus != 0){
			readCorpusGST(corpus,gTree);
		}
		if(commands != 0){
			readCommandsGST(commands,gTree);
		}
		freeGST(gTree);
	}		
	else{
		RBT *rTree = newRBT(compareSTRING);
		setRBTdisplay(rTree,displaySTRING);
		setRBTfree(rTree,freeSTRING);
		if(corpus != 0){
			readCorpusRBT(corpus,rTree);
		}
		if(commands != 0){
			readCommandsRBT(commands,rTree);
		}
		freeRBT(rTree);
	}
	freePROCESS(option);		
	return 0;
}

void readCorpusGST(FILE *fp,GST *tree){
	char *phrase;
	while(1){
		if(stringPending(fp)){
			phrase = readString(fp);
		}
		else{
			phrase = readToken(fp);
		}
		if(phrase == 0){
			return;
		}
		phrase = cleanWord(phrase);
		if(phrase != 0){
			STRING *cleaned = newSTRING(cleanWord(phrase));
			insertGST(tree,cleaned);
		}
	}
	//displayGST(tree,fp);
}

void readCorpusRBT(FILE *fp,RBT *tree){
	char *phrase;
	while(1){
		if(stringPending(fp)){
			//fprintf(stdout, "got here!!\n");
			phrase = readString(fp);
		}
		else{
			phrase = readToken(fp);
		}
		if(phrase == 0){
			return;
		}
		phrase = cleanWord(phrase);
		if(phrase != 0){
			STRING *cleaned = newSTRING(cleanWord(phrase));
			insertRBT(tree,cleaned);
		}
	}
}

char *cleanWord(char *word){
	if(word[0] == 0){
		return 0;
	}
	int index = 0;
	char newWord[100];
	int newLength = 0;
	while(word[index] == ' '){
		index++;
	}
	while(word[index] != 0){
		//fprintf(stdout, "%c\n",word[index] );
		word[index] = tolower(word[index]);
		if(word[index] > 96 && word[index] < 123){
			newWord[newLength] = word[index];
			newLength++;
		}
		index++;
	}
	if(newLength > 0){
		newWord[newLength] = '\0';
		newLength++;
	//fprintf(stdout, "%s %d\n", newWord,newLength);
		for(int i=0;i<newLength;i++){
			word[i] = newWord[i];
		}
	//fprintf(stdout, "%s\n", word);
		return word;
	}
	return 0;
}

char commandHelperGST(FILE *fp,GST *tree,char check){
	if(check == 0)return 0;
	switch(check){
		case 'i':
			{
				char *inWord = 0;
				if(stringPending(fp)){
					inWord = readString(fp);
				}
				else{
					inWord = readToken(fp);
				}
				if(inWord == 0)return 0;
				inWord = cleanWord(inWord);
				if(inWord != 0){
					STRING *newWord = newSTRING(inWord);
					insertGST(tree,newWord);
					//displaySTRING(newWord,stdout);
				}
			}
			break;
		case 'd':
			{
				char *outWord = 0;
				if(stringPending(fp)){
					outWord = readString(fp);
				}
				else{
					outWord = readToken(fp);
				}
				if(outWord == 0){
					return 0;
				}
				outWord = cleanWord(outWord);
				STRING *newWord = newSTRING(outWord);
				if(getSTRING(newWord) == 0){
					freeSTRING(newWord);
					break;
				}
				//fprintf(stdout, "word - %s\n",getSTRING(newWord));
				if(locateGST(tree,newWord) == 0){
					fprintf(stdout,"Value ");
					displaySTRING(newWord,stdout);
					fprintf(stdout," not found.\n");
					freeSTRING(newWord);
				}
				else if(freqGST(tree,newWord) > 1){
					//fprintf(stdout, "freq should be > 1\n");
					deleteGST(tree,newWord);
				}
				else{
					//fprintf(stdout, "freq should be == 1\n");
					setGSTfree(tree,0);
					deleteGST(tree,newWord);
					setGSTfree(tree,freeSTRING);
					freeSTRING(newWord);
				}
			}
			break;
		case 'f':
			{
			char *freqWord = 0;
			if(stringPending(fp)){
				freqWord = readString(fp);
			}
			else{
				freqWord = readToken(fp);
			}
			if(freqWord == 0){
				return 0;
			}
			freqWord = cleanWord(freqWord);
			STRING *newWord = newSTRING(freqWord);
			if(getSTRING(newWord) ==  0){
				freeSTRING(newWord);
				break;
			}
			fprintf(stdout, "Frequency of ");
			displaySTRING(newWord,stdout);
			fprintf(stdout, ": %d\n",freqGST(tree,newWord));
			freeSTRING(newWord);
			}
			break;
		case 's':
			{
			if(sizeGST(tree) == 0){
				fprintf(stdout, "EMPTY\n");
			}
			else{
				displayGST(tree,stdout);
			}
			break;
			}
		case 'r':
			statisticsGST(tree,stdout);
			break;
	}
	return check;
}

void readCommandsGST(FILE *fp,GST *tree){
	char check = readChar(fp);
	while(isspace(check) != 0 && check != EOF){
		check = readChar(fp);
	}
	if(check == EOF){
		return;
	}
	check = commandHelperGST(fp,tree,check);
	if(check != 0){
		readCommandsGST(fp,tree);
	}
}

char commandHelperRBT(FILE *fp,RBT *tree,char check){
	switch(check){
		case 'i':
			{
			char *inWord;	
			if(stringPending(fp))
				inWord = readString(fp);
			else
				inWord = readToken(fp);
			if(inWord == 0)return 0;
			STRING *newWord = newSTRING(cleanWord(inWord));
			if(getSTRING(newWord) == 0){
				break;
			}
			insertRBT(tree,newWord);
			break;
			}
		case 'd':
			{
			char *outWord;
			if(stringPending(fp))
				outWord = readString(fp);
			else
				outWord = readToken(fp);
			if(outWord == 0)return 0;
			STRING *newWord = newSTRING(cleanWord(outWord));
			if(getSTRING(newWord) == 0){
				freeSTRING(newWord);
				break;
			}
			if(locateRBT(tree,newWord) == 0){
				fprintf(stdout,"Value ");
				displaySTRING(newWord,stdout);
				fprintf(stdout," not found.\n");
				freeSTRING(newWord);
			}
			else if(freqRBT(tree,newWord) == 1){
				STRING *val = findRBT(tree,newWord);
				setRBTfree(tree,0);
				deleteRBT(tree,newWord);
				setRBTfree(tree,freeSTRING);
				freeSTRING(val);
			}
			else{
				deleteRBT(tree,newWord);
			}
			break;
			}
		case 'f':
			{
			char *freqWord;	
			if(stringPending(fp))
				freqWord = readString(fp);
			else
				freqWord = readToken(fp);
			if(freqWord == 0)return 0;
			STRING *newWord = newSTRING(cleanWord(freqWord));
			if(getSTRING(newWord) == 0){
				freeSTRING(newWord);
				break;
			}
			fprintf(stdout, "Frequency of ");
			displaySTRING(newWord,stdout);
			fprintf(stdout, ": %d\n",freqRBT(tree,newWord));
			freeSTRING(newWord);
			break;
			}
		case 's':
			{
			if(sizeRBT(tree) == 0){
				fprintf(stdout, "EMPTY\n");
			}
			else{
				displayRBT(tree,stdout);
			}
			break;
			}
		case 'r':
			statisticsRBT(tree,stdout);
			break;
	}
	return check;
}

void readCommandsRBT(FILE *fp,RBT *tree){
	char check = readChar(fp);
	while(isspace(check) != 0 && check != EOF){
		check = readChar(fp);
	}
	if(check == EOF){
		return;
	}
	check = commandHelperRBT(fp,tree,check);
	if(check == 0)return;
	readCommandsRBT(fp,tree);
}


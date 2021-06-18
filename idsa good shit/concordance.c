#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 50

struct node {
    char *value;          
    int *Hz;
    int *location;
    struct node *R_L;
    struct node *R_R;
};

typedef int (*Compare)(const char *, const char *);

void insert(char* key, struct node** leaf, Compare cmp, int where)
{
    int res;
  	int tmp;
  	int which;
    if( *leaf == NULL ) {
    	//process the word that first meet
        *leaf = (struct node*) malloc( sizeof( struct node ) );
        (*leaf)->value = malloc( strlen (key) +1 );   //allocates memory
		(*leaf)->Hz = malloc( sizeof( int ) );   //allocates memory
		(*leaf)->location = malloc(sizeof(int)); //allocates memory
		tmp = 0;
		(*leaf)->Hz = tmp;
		(*leaf)->location[0] = where; //stores the first meet location
        strcpy ((*leaf)->value, key);                
        (*leaf)->R_L = NULL;
        (*leaf)->R_R = NULL;
        
    } else {
    	//process duplicated words
        res = cmp (key, (*leaf)->value); //stores the word
        if( res < 0){
            insert( key, &(*leaf)->R_L, cmp, where);
        }
		else if( res > 0){
            insert( key, &(*leaf)->R_R, cmp, where);
        }else{        
		//Stores location
		which = (*leaf)->Hz;
		which = (which/4)+1;//find out the frequency to input the location in correct slot
		(*leaf)->location = (int *)realloc((*leaf)->location, 8 *which ); //extend memory for next input
		(*leaf)->location[which] = where;
		//Stores Frequency
		(*leaf)->Hz = (*leaf)->Hz+1;
		}
    }
}

int CmpStr(const char *a, const char *b)
{
    return (strcmp (a, b));     // string comparison instead of pointer comparison
}


void chop(char *str) {
    int len = strlen(str);			//Sample code to chop out \n, just for copying purpose and referenced from Dr.Lam Sum, sams header file
    if (len > 0 && str[len-1] == '\n') str[len-1] = '\0';
}

//Output the concordance to Enddata.txt
void outputT(struct node *root, FILE*wfr)
{
	int k=0;		
	int which;
    if( root != NULL ) {
    
        outputT(root->R_L, wfr);
        //print word
        fprintf(wfr,"%s,\n", root->value);     
        //print Hz
        k = (root->Hz);
        k = (k/4)+1;
        fprintf(wfr,"Word shown %d time.\nLocation :\n", k);
        //print location
        k=0;
      	which = (root)->Hz;
		which = (which/4)+1;
		int newn =0 ;
		while(which>0){
			fprintf(wfr, " %d ", (root)->location[k]);
			if(newn>15){
				fprintf(wfr, "\n");
				newn = 0 ;
			}
			newn++;
			which--;
			k++;	
		}
   		fprintf(wfr, "\n");
        outputT(root->R_R, wfr);
    }
}
//remove space
int rmspace(){
	char b;
	int loop;
	int g = 0;
	int n = 0;
	FILE *fin = fopen("data.txt", "r");
	FILE *fout = fopen("ndata.txt","w");
	
	while (!feof(fin)){
		fscanf(fin, "%c", &b);
		if(b==' '||b=='\n'){
			g++;
			if(g==1){
				n++;
				fprintf(fout,"\n");
			
			}
		}else{
			fprintf(fout,"%c",b);
			g = 0;
		}
	}
	fclose(fin);
	fclose(fout);
	return n;
}

int main(){
	char spinner[] = "|-\\";
	char line[MAXLEN+1];  
	struct node *Rooot = NULL;
    char *value;
	int i,j=0;
	int n;
	int where;
	n = rmspace(); //remove the space
	FILE*rfr = fopen("ndata.txt", "r");
	while (!feof(rfr)) {
		where ++;
		fgets(line, sizeof line,rfr);
		strtok(line, "\n" );//remove '\n'
		value = line;
   	 insert(value,  &Rooot, (Compare)CmpStr, where); //using recursive 
	}
	fclose(rfr);
	//print out result
	FILE*wfr = fopen("Enddata.txt", "w+");
	outputT(Rooot, wfr); //using recursive 
	fclose(wfr);
	return 0;
}















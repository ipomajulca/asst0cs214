#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


//create a hast table to get all operator
#define HASHSIZE 42

struct operator {
    char *name;
    char *type;
    struct operator *next;
};

struct operator *hashtab[HASHSIZE];

unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}


struct operator *lookup(char *s)
{
    struct operator *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;
    return NULL;
}



//duplicate char
char *duplicateChar(char *s)
{
    char *p;
    p = (char *) malloc(strlen(s)+1);
    if (p != NULL)
        strcpy(p, s);
    return p;
}



struct operator *insert(char *name, char *type)
{
    struct operator *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) {
        np = (struct operator *) malloc(sizeof(*np));
        if (np == NULL || (np->name = duplicateChar(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else
        free((void *) np->type);
      if ((np->type = duplicateChar(type)) == NULL)
        return NULL;
    return np;
}



void getAllOperators() {
    insert("(","left parenthesis");
    insert(")","right parenthesis");
    


}

struct token {
    char *input; //
    char *inputType;
};

void printToken(struct token t) {
    if(t.input!="") {
        printf("%s", t.inputType);
        printf(" \"");
        printf("%s", t.input);
        printf("\"\n");
    }
}

int isDelimiter(char *str, char c) {
    if(strchr(str,c) != NULL)
        return 1;
    else
        return 0;
}

char * appendCharToTokenInput(char *input, char c) {
    size_t len = strlen(input);
    char *str2 = malloc(len + 2 );
    strcpy(str2, input);
    str2[len] = c;
    str2[len + 1] = '\0';
    return str2;
}

struct token newEmptyToken() {
    struct token t;
    t.input = "";
    t.inputType = "";
    return t;
}

int isEmptyToken(struct token t) {
    if(t.input=="" && t.inputType=="")
        return 1;
    else
        return 0;
}

char * getInputType(char c) {
   if(isdigit(c))
       return "decimal integer";
   else
       if(isalpha(c)) return "word";
}

struct token startNewToken(char c) {
   struct token t;
   t.input = appendCharToTokenInput(t.input,c);
   t.inputType = getInputType(c);
   return t;
}

int isOperator(struct token t) {
    return 0;
}

//function to get all tokens to display
void getTokens(char *input, int index, char* delimiters, struct token t) {
    if(input[index]!='\0') {
        //check if it is delimiter
        if(isDelimiter(delimiters,input[index])) {
            //get token
            printToken(t);
            //start new token
            t = newEmptyToken();
        }
        else {
            if(isEmptyToken(t))
            {
                //start a new token adding the char and getting the initial type
                t = startNewToken(input[index]);
                struct operator *operator = lookup(t.input);
                if(operator!=NULL) {
                    t.inputType = operator->type;
                }
            }
            else {
                //temp token input to check if it is 
                char * tempTokenInput = appendCharToTokenInput(t.input,input[index]);
                t.input = appendCharToTokenInput(t.input,input[index]);
            }
            //check if it is a RefCard token
           // struct operator *operator = lookup(t.input);
            //printf("%s", o->type);




        }
        getTokens(input,index+1,delimiters,t);

    }
    else
        {
            printToken(t);

    }

}

//function to read the input from the prompt
void readInput(char *input, char *delimiters) {
    struct token t;
    t.input = "";
    t.inputType="";
    getTokens(input,0,delimiters,t);
}




int main(int argc, char* argv[]) {
   // char *input = "123 stuff";
    char *input = "( ( ( ( 123";
    char *delimiters = " \t\v\f\n";   //define delimiters
    getAllOperators();                //define operators
    readInput(input,delimiters);      //get list of tokens
    return 0;
}

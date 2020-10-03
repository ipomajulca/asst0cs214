#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

//create a hast table to get all operator
#define HASHSIZE 100 //42 operators

//struct that will have all the operators
struct Operator {
    char *name;
    char *type;
    struct Operator *next;
};

//hashtable to help getting the operator
struct Operator *hashtab[HASHSIZE];

unsigned hash(char *s)
{
    unsigned value;
    for (value = 0; *s != '\0'; s++)
        value = *s + 13 * value;
    return value % HASHSIZE;
}

//function to search for an operator
struct Operator *lookup(char *s)
{
    struct Operator *op;
    for (op = hashtab[hash(s)]; op != NULL; op = op->next)
        if (strcmp(s, op->name) == 0)
            return op;
    return NULL;
}

//function to duplicate a char
char *duplicateChar(char *s)
{
    char *p;
    p = (char *) malloc(strlen(s)+1);
    if (p != NULL)
        strcpy(p, s);
    return p;
}

//function to insert an operator to the hashtable
struct Operator *insert(char *name, char *type)
{
    struct Operator *op;
    unsigned hashval;
    if ((op = lookup(name)) == NULL) {
        op = (struct Operator *) malloc(sizeof(*op));
        if (op == NULL || (op->name = duplicateChar(name)) == NULL)
            return NULL;
        hashval = hash(name);
        op->next = hashtab[hashval];
        hashtab[hashval] = op;
    } else
        free((void *) op->type);
    if ((op->type = duplicateChar(type)) == NULL)
        return NULL;
    return op;
}

//function to add all operators to a hash table
void setAllOperators() {
    insert("(","left parenthesis");
    insert(")","right parenthesis");
    insert("[","left bracket");
    insert("]","right bracket");
    insert(".", "structure member");
    insert("->","structure pointer");
    insert("sizeof","sizeof");
    insert(",","comma");
    insert("!","negate");
    insert("~","1s complement");
    insert(">>","shift right");
    insert("<<","shift left");
    insert("^","bitwise XOR");
    insert("|","bitwise OR");
    insert("++","increment");
    insert("--","decrement");
    insert("+","addition");
    insert("/","division");
    insert("||","logical OR");
    insert("&&","logical AND");
    insert("?","conditional true");
    insert(":","conditional false");
    insert("==","equality test");
    insert("!=","inequality test");
    insert("<","less than test");
    insert(">","greater than test");
    insert("<=","less than or equal test");
    insert(">=","greater than or equal test");
    insert("=","assignment");
    insert("+=","plus equals");
    insert("-=","minus equals");
    insert("*=","times equals");
    insert("/=","divide equals");
    insert("%=","mod equals");
    insert(">>=","shift right equals");
    insert("<<=","shift left equals");
    insert("&=","bitwise AND equals");
    insert("^=","bitwise XOR equals");
    insert("|=","bitwise OR equals");
    insert("&","AND/address operator");
    insert("-","minus/subtract operator");
    insert("*","multiply/dereference operator");
    insert("{","left brace");

}
//struct for the token
struct Token {
    char *name;
    char *type;
};

//function to check if the token name is empty
int isEmptyToken(struct Token *token) {
    if(strcmp(token->name,"")==0)
        return 1;
    else
        return 0;
}

//function to print a token
void print(struct Token *token) {
    if(!isEmptyToken(token)) {
        printf("%s:", token->type);
        printf(" \"");
        printf("%s", token->name);
        printf("\"\n");
    }
}

//function to detect if it is delimiter char
int isDelimiter(char *delimiters, char c) {
    if(strchr(delimiters,c) != NULL)
        return 1;
    else
        return 0;
}

char * appendCharToTokenName(char *tokenName, char c) {
    size_t len = strlen(tokenName);
    char *temp = malloc(len + 2 );
    strcpy(temp, tokenName);
    temp[len] = c;
    temp[len + 1] = '\0';
    return temp;
}

struct Token * newEmptyToken() {
    struct Token *token = (struct Token *)malloc(sizeof(token));
    token->name = "";
    token->type = "";
    return token;
}


int isOctalDigit(char c) {
  //  printf("numero: %d\n", c - '0' );
    if(isdigit(c) && c - '0' >= 0 && c - '0' < 8)
        return 1;
    else
        return 0;
}

int isHexadecimalDigit(char c) {
    char *hexadecimalChars = "ABCDEF";
    //printf("numero: %d\n", c - '0' );
    if(isdigit(c) && c - '0' >= 0 && c - '0' <= 9)
        return 1;
    else
    if(strchr(hexadecimalChars,toupper(c)) != NULL)
        return 1;
    else
        return 0;
}

struct Token * checkOperator(char *input, int index) {
    int state=0;
    struct Token *t = newEmptyToken();
    t->name = appendCharToTokenName(t->name, input[index]);
    struct Operator *operator = lookup(t->name);
    t = newEmptyToken();
    if(operator != NULL && state==0) {
        int i = 0;
        while(operator!=NULL && index + i < strlen(input)) {
            t->name = appendCharToTokenName(t->name, input[index+i]);
            t->type = operator->type;
            i++;
            operator = lookup(appendCharToTokenName(t->name, input[index+i]));
        }
    }

    return t;
}

struct Token * checkNumber(char *input, int index) {
    int state = 0;
    struct Token *t = newEmptyToken();
    t->type = "decimal integer";
    int startWithZero = 0;
    int isScientificNotation = 0;
    int numberType = 0;  //0: decimal integer, 1: octal integer 2: hexadecimal integer 3: floating point
    while(input[index]!='\0'  && state==0) {

       if(startWithZero==0 && input[index]=='0')  //octal or hexadecimal integer
        {

            t->name = appendCharToTokenName(t->name,input[index]);
            index = index + 1;
            if(input[index]=='x' || input[index]=='X')  //hexadecimal
            {
                numberType = 2;
                t->name = appendCharToTokenName(t->name,input[index]);
                t->type = "hexadecimal integer";
                index = index + 1;
            }
            else if(isOctalDigit(input[index])) {
                numberType = 1;
                t->type = "octal integer";
                t->name = appendCharToTokenName(t->name,input[index]);
                index = index + 1;
            }
        }
        if(numberType==0) { //0: decimal integer
            if(isdigit(input[index])) {
                t->name = appendCharToTokenName(t->name,input[index]);

                index = index + 1;
            }
            else {
                //it may be a decimal
                if(input[index]=='.') {

                    if(isdigit(input[index+1]) )
                    {
                        t->name = appendCharToTokenName(t->name,input[index]);
                        t->type = "floating point";
                       // numberType=3;
                    }
                    else{
                        state = 1;
                    }

                    index = index + 1;


                }
                else if(input[index]=='e'){
                    if(strcmp(t->type,"floating point")==0) {
                        if(isdigit(input[index+1]) || input[index+1] =='+' || input[index+1] =='-' )
                        {
                            t->name = appendCharToTokenName(t->name,input[index]);
                            isScientificNotation = 1;
                        }
                        else{
                            state = 1;
                        }
                    }
                    else {

                        state = 1;
                    }
                    index = index + 1;

                }
                else if(input[index]=='-' || input[index]=='+' ){
                    if(isScientificNotation==1) {
                        t->name = appendCharToTokenName(t->name,input[index]);

                    }
                    else {
                        state = 1;
                    }

                    index = index + 1;
                }
                else {
                    state = 1;
                }
            }

        }
        else
        if(numberType==2) {  //2: hexadecimal integer

            if(isHexadecimalDigit(input[index])) {
                t->name = appendCharToTokenName(t->name,input[index]);
                index = index + 1;
            }
            else {
                state=1;
            }

        }
        else if (numberType==1) {  //1: octal integer
            if(isOctalDigit(input[index])) {
                t->name = appendCharToTokenName(t->name,input[index]);
                index = index + 1;
            }
            else {

                //it may be a decimal
                if(input[index]=='.') {

                    if(isdigit(input[index+1]) )
                    {
                        t->name = appendCharToTokenName(t->name,input[index]);
                        t->type = "floating point";
                        // numberType=3;
                    }
                    else{
                        state = 1;
                    }

                    index = index + 1;


                }
                else if(input[index]=='e'){
                    if(strcmp(t->type,"floating point")==0) {
                        if(isdigit(input[index+1]) || input[index+1] =='+' || input[index+1] =='-' )
                        {
                            t->name = appendCharToTokenName(t->name,input[index]);
                            isScientificNotation = 1;
                        }
                        else{
                            state = 1;
                        }
                    }
                    else {

                        state = 1;
                    }
                    index = index + 1;

                }
                else if(input[index]=='-' || input[index]=='+' ){
                    if(isScientificNotation==1) {
                        t->name = appendCharToTokenName(t->name,input[index]);

                    }
                    else {
                        state = 1;
                    }

                    index = index + 1;
                }
                else {
                    state = 1;
                }
            }

        }

        startWithZero =1;
   }
   return t;
}

struct Token * checkWord(char *input, int index) {
    int state = 0;
    int startWithChar = 0;
    struct Token *tWord = newEmptyToken();
    if(isalpha(input[index])) startWithChar = 1;
    while(input[index]!='\0'  && state==0 && startWithChar==1) {
        if(isalpha(input[index]) || isdigit(input[index])) {
            tWord->name = appendCharToTokenName(tWord->name, input[index]);
        }
        else {
            state=1;
        }
        index = index + 1;
    }
    tWord->type = "word";
    return tWord;
}


//Function to inspect the input from the prompt and print all the tokens found
void inspectInput(char *input, char *delimiters) {
    //struct Token *token = newEmptyToken();
    //getTokens(input,0,delimiters,token);
    int index=0;
    struct Token *token = newEmptyToken();
    int state=0;
    while(input[index]!='\0' && state==0) {
        if(isDelimiter(delimiters,input[index])) {
          //  print(token);
            //start next token
            token = newEmptyToken();
            index++;
        }
        else {
            //check if it is word
          if(isalpha(input[index])) {
              token = checkWord(input, index);

          }
          else
           if(isdigit(input[index])) {
               //check if it is integer
               token = checkNumber(input, index);
           }
           else
             {
                token = checkOperator(input, index);
            }


            if (!isEmptyToken(token)) {
                print(token);
                index = index + strlen(token->name);
            }
            else {
                state = 1;
            }


        }
    }
}

int main(int argc, char* argv[]) {
    char *delimiters = " \t\v\f\n";   //define delimiters
    setAllOperators();                //define operators
    inspectInput(argv[1],delimiters);    //inspect input string to get tokens
    return 0;
}

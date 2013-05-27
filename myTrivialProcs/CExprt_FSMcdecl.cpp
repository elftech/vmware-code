// CExprt_FSMcdecl.cpp : Defines the entry point for the console application.
//
#include <stdio.h>  
#include <string.h>  
#include <ctype.h>  
#include "stdlib.h" 

#define MAXTOKENS 100  
#define MAXTOKENLEN 64  

enum type_tag { IDENTIFIER, QUALIFIER, TYPE };  
 
struct token { 
    char type;  
    char string[MAXTOKENLEN];  
};  

int top = -1;  
 
/* holds all the tokens before first identifier */  
struct token stack[MAXTOKENS];  
 
/* holds the token just read */  
struct token _this;  
 
#define pop stack[top--]  
#define push(s) stack[++top]=s  

enum type_tag  
classify_string(void)  

/* figure out the identifier type */  
{ 
    char *s = _this.string;  
    if (!strcmp(s, "const")) { 
        strcpy(s, "read-only");  
        return QUALIFIER;  
    }  
    if (!strcmp(s, "volatile")) return QUALIFIER;  
    if (!strcmp(s, "void")) return TYPE;  
    if (!strcmp(s, "char")) return TYPE;  
	if (!strcmp(s, "signed")) return TYPE;  
    if (!strcmp(s, "unsigned")) return TYPE;  
    if (!strcmp(s, "short")) return TYPE;  
    if (!strcmp(s, "int")) return TYPE;  
    if (!strcmp(s, "long")) return TYPE;  
    if (!strcmp(s, "float")) return TYPE;  
    if (!strcmp(s, "double")) return TYPE;  
    if (!strcmp(s, "struct")) return TYPE;  
    if (!strcmp(s, "union")) return TYPE;  
    if (!strcmp(s, "enum")) return TYPE;  
    return IDENTIFIER;  
}  

void gettoken(void)  
{             
	/* read next token into "this" */  
    char *p = _this.string;  
 
    /* read past any spaces */  
    while ((*p = getchar()) == ' ');  
 
    if (isalnum(*p)) { 
        /* it starts with A-Z,1-9 read in identifier */  
        while (isalnum(*++p = getchar()));  
        ungetc(*p, stdin);  
        *p = '\0';  
        _this.type = classify_string();  
        return;  
    }  
    _this.string[1] = '\0';  
    _this.type = *p;  
    return;  
}  
 
void initialize(),get_array(), get_params(), get_lparen(), get_ptr_part(), get_type();  

void (*nextstate)(void) = initialize;  

int main(int argc, char* argv[])
{
/* Cdecl written as a finite state machine */  
/* transition through the states, until the pointer 
is null */  
   while (nextstate != NULL)  
       (*nextstate)();  
	return 0;
}

void initialize()  
{ 
    gettoken();  
    while (_this.type != IDENTIFIER) { 
        push(_this);  
        gettoken();  
    }  
    printf("%s is ", _this.string);  
    gettoken();  
    nextstate = get_array;  
}  

void get_array()  
{ 
    nextstate = get_params;  
    while (_this.type == '[') { 
        printf("array ");  
        gettoken();/* a number or ']' */  
        if (isdigit(_this.string[0])) { 
            printf("0..%d ", atoi(_this.string) - 1);  
            gettoken();/* read the ']' */  
        }  
        gettoken();/* read next past the ']' */  
        printf("of ");  
        nextstate = get_lparen;  
    }  
}  

void get_params()  
{ 
    nextstate = get_lparen;  
    if (_this.type == '(') { 
        while (_this.type != ')') { 
            gettoken();  
        }  
        gettoken();  
        printf("function returning ");
    }  
}  
 
void get_lparen()  
{ 
    nextstate = get_ptr_part;  
    if (top >= 0) 
	{ 
        if (stack[top].type == '(')
		{ 
            pop;  
            gettoken();/* read past ')' */  
            nextstate = get_array;  
        }  
    }  
}  
 
void get_ptr_part()  
{ 
    nextstate = get_type;  
    if (stack[top].type == '*') 
	{ 
        printf("pointer to ");  
        pop;  
        nextstate = get_lparen;  
    } 
	else if (stack[top].type == QUALIFIER) 
	{ 
        printf("%s ", pop.string);  
        nextstate = get_lparen;  
    }  
}  

void get_type()  
{ 
    nextstate = NULL;  
    /* process tokens that we stacked while reading to 
identifier */  
    while (top >= 0) 
	{ 
        printf("%s ", pop.string);  
    }  
    printf("\n");  
} 



























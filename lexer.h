/*
Aryan Barapatre 2020B4A70833P
Bhuvan Gupta    2020B4A71654P
Dhruv Singh     2020B4A70969P
Pulkit          2020B2A71932P
Jash Naik       2020B3A71958P
Siripuram Surya 2020B2A71406P
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <sys/time.h>
#ifndef LEXER_H
#define LEXER_H
#define BUFFER_SIZE 128
#define HASHTABLE_SIZE 70
#define BIG_PRIME 2147483647
typedef enum
{
    BUFFER_0,
    BUFFER_1
} Buffer;

typedef struct buffer_pointer
{
    int ind;    // Index of the buffer
    Buffer buf; // Denotes where pointer lies (i.e in buffer0 or buffer1)
} BUFFER_POINTER;

typedef struct twin_buffer
{
    char *BUFFER0;
    char *BUFFER1;
    struct buffer_pointer *forward_ptr;
    struct buffer_pointer *current_ptr;
    int siz;
    int buff0siz;
    int buff1siz;
    int line;
    int lastread;
} TWIN_BUFFER;

TWIN_BUFFER *buf_init();
void allocate_buffer(FILE *fp, TWIN_BUFFER *buffer);
char getNext(FILE *fp, TWIN_BUFFER *buffer);
void retract(TWIN_BUFFER *buffer);
char *getLexeme(TWIN_BUFFER *buffer);




// #include "hashtable.h"
typedef struct SYMBOL
{
    char *key;
    void *value;
    struct SYMBOL *Next;
} Symbol;

typedef struct HASHTABLE
{
    Symbol **Table;
    int size;
} HashTable;

typedef enum {
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RUID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_DOLLAR,
    TK_ERROR,
    TK_EPSILON
} TOKEN_ID;

typedef struct TOKEN{
    TOKEN_ID tkId;
    char* lexeme;
    int lineno;
}Token;
TOKEN_ID getEnum(const char * str);
char* token_str(TOKEN_ID id);

// #include "linkedlist.h"
typedef struct LISTNODE{
    Token* mytoken;
    struct LISTNODE* next;
    struct LISTNODE* prev;
}ListNode;

typedef struct dll {
    struct LISTNODE* head;
    struct LISTNODE* tail;
    struct LISTNODE* current;
    int num;
} List;

List* createList();
ListNode* insertToList(List* list, Token* token);
void deleteList(List* list);
void printList(List* list, FILE* fp);
void deleteListNode(List* list);
void insert2ListInOrder(List*list,Token* tk);



int polyHash(char *str, int n);
HashTable *initializeHashTable();
HashTable *insert2HashTable(HashTable *hash_tbl, char *lexeme, int n, void *value);
Symbol *search4HashTable(HashTable *hash_tbl, char *lexeme, int n);
HashTable *initializeLookupTable();
TOKEN_ID lookUp(HashTable *lookupTable, char *lexeme, int n, int state);
void freeHashTable(HashTable *hash_tbl);
//char *token_str(TOKEN_ID id);

//#include "grammar.h"
typedef struct
{
    int noRHS;   // Number of symbols in RHS
    char ***rhs; // Right-hand side of the production
} ProductionRuleRHS;

extern char **nonTerminals, **terminals;

typedef struct
{
    char **nonTerminals, **terminals;
    int terminalsCount, nonTerminalsCount;
} Grammar;

typedef struct
{
    char *lhs;
    char **rhs;
} First, Follow;

typedef struct
{
    HashTable *first, *follow;
} FirstAndFollow;


int unionSet(char **a, char *b);
void unionSet2(char **a, char **b);
char **  allocateProdRule(char* line,Grammar* gram);
int calculateFirst(HashTable* gramTable, char* key,int ind, int n, char ** firstSet,int recDepth);
void calculateFollow(HashTable* gramTable, char* key, int n, char ** followSet,HashTable* firstset, char ** detectRecursion);
Grammar* init_grammar();
FirstAndFollow* init_FirstAndFollow();
HashTable* LoadProductionRules(FILE* f,Grammar* gram);
Token *getToken(FILE *fp, TWIN_BUFFER *tbuff, HashTable *lookup_tbl);
void free_buffer(TWIN_BUFFER *tbuff);
void freeLookUpTable(HashTable* lookup_tbl);
char* mallocString(char* s);
#endif
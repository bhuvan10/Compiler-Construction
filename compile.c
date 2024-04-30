
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

/*
Aryan Barapatre 2020B4A70833P
Bhuvan Gupta    2020B4A71654P
Dhruv Singh     2020B4A70969P
Pulkit          2020B2A71932P
Jash Naik       2020B3A71958P
Siripuram Surya 2020B2A71406P
*/
//#include "lexer.h"
#define LEX_ERROR_STATE 62

TWIN_BUFFER *buf_init()
{
    TWIN_BUFFER *tbuff = (TWIN_BUFFER *)malloc(sizeof(TWIN_BUFFER));
    tbuff->BUFFER0 = (char *)malloc(BUFFER_SIZE);
    tbuff->BUFFER1 = (char *)malloc(BUFFER_SIZE);
    tbuff->forward_ptr = (BUFFER_POINTER *)malloc(sizeof(BUFFER_POINTER));
    tbuff->current_ptr = (BUFFER_POINTER *)malloc(sizeof(BUFFER_POINTER));
    tbuff->buff0siz=0;
    tbuff->buff1siz=0;
    tbuff->lastread=-1;
    return tbuff;
}
void free_buffer(TWIN_BUFFER *tbuff)
{
    free(tbuff->BUFFER0);
    free(tbuff->BUFFER1);
    free(tbuff->forward_ptr);
    free(tbuff->current_ptr);
    free(tbuff);
}

void allocate_buffer(FILE *fp, TWIN_BUFFER *buffer)
{     long int initial_position = ftell(fp);
    buffer->siz = fread(buffer->BUFFER0, BUFFER_SIZE, sizeof(char), fp);
    // Get final position
    long int final_position = ftell(fp);
    buffer->siz = final_position - initial_position;
    buffer->buff0siz=buffer->siz;
    buffer->forward_ptr->ind = 0;
    buffer->current_ptr->ind = 0;
    buffer->forward_ptr->buf = BUFFER_0;
    buffer->current_ptr->buf = BUFFER_0;
    buffer->lastread=0;
    buffer->line = 1;
}
char getNext(FILE *fp, TWIN_BUFFER *buffer)
{
    char *curBuf;
    if (buffer->forward_ptr->buf == BUFFER_0)
    {
        curBuf = buffer->BUFFER0;
    }
    else
    {
        curBuf = buffer->BUFFER1;
    }
    char c = curBuf[buffer->forward_ptr->ind];
    buffer->forward_ptr->ind++;
    if (buffer->forward_ptr->ind == BUFFER_SIZE && buffer->forward_ptr->buf == BUFFER_0&&buffer->lastread==0)
    {
        long int initial_position = ftell(fp);
        buffer->siz = fread(buffer->BUFFER1, BUFFER_SIZE, sizeof(char), fp);
        long int final_position = ftell(fp);
        buffer->siz = final_position - initial_position;
        buffer->buff1siz=buffer->siz;
        buffer->forward_ptr->ind = 0;
        buffer->forward_ptr->buf = BUFFER_1;
        buffer->lastread=1;
    }
    else if (buffer->forward_ptr->ind == BUFFER_SIZE && buffer->forward_ptr->buf == BUFFER_1&&buffer->lastread==1)
    {
        long int initial_position = ftell(fp);
        buffer->siz = fread(buffer->BUFFER0, BUFFER_SIZE, sizeof(char), fp);
        long int final_position = ftell(fp);
        buffer->siz = final_position - initial_position;
        buffer->buff0siz=buffer->siz;
        buffer->forward_ptr->ind = 0;
        buffer->forward_ptr->buf = BUFFER_0;
        buffer->lastread=0;
    }
    else if(buffer->forward_ptr->ind == BUFFER_SIZE && buffer->forward_ptr->buf == BUFFER_0&&buffer->lastread==1)
    {
        buffer->forward_ptr->ind = 0;
        buffer->forward_ptr->buf = BUFFER_1;
    }
    else if(buffer->forward_ptr->ind == BUFFER_SIZE && buffer->forward_ptr->buf == BUFFER_1&&buffer->lastread==0)
    {
        buffer->forward_ptr->ind = 0;
        buffer->forward_ptr->buf = BUFFER_0;
    }
    if (c == '\n')
        buffer->line++;
    return c;
}
void retract(TWIN_BUFFER *buffer)
{
    buffer->forward_ptr->ind--;
    if (buffer->forward_ptr->ind < 0)
    {
        if (buffer->forward_ptr->buf == BUFFER_0)
        {
            buffer->forward_ptr->buf = BUFFER_1;
            buffer->forward_ptr->ind = BUFFER_SIZE - 1;
        }
        else
        {
            buffer->forward_ptr->buf = BUFFER_0;
            buffer->forward_ptr->ind = BUFFER_SIZE - 1;
        }
    }
    if (buffer->forward_ptr->buf == BUFFER_0 && buffer->BUFFER0[buffer->forward_ptr->ind] == '\n')
    {
        buffer->line--;
    }
    else if (buffer->forward_ptr->buf == BUFFER_1 && buffer->BUFFER1[buffer->forward_ptr->ind] == '\n')
    {
        buffer->line--;
    }
}


char *getLexeme(TWIN_BUFFER *buffer)
{
    char *lexeme = (char *)malloc(BUFFER_SIZE);
    int size = 0;
    if (buffer->forward_ptr->buf == BUFFER_0 && buffer->current_ptr->buf == BUFFER_0)
    {
        memcpy(lexeme, buffer->BUFFER0 + buffer->current_ptr->ind, buffer->forward_ptr->ind - buffer->current_ptr->ind);
        size = buffer->forward_ptr->ind - buffer->current_ptr->ind;
    }
    else if (buffer->forward_ptr->buf == BUFFER_1 && buffer->current_ptr->buf == BUFFER_1)
    {
        memcpy(lexeme, buffer->BUFFER1 + buffer->current_ptr->ind, buffer->forward_ptr->ind - buffer->current_ptr->ind);
        size = buffer->forward_ptr->ind - buffer->current_ptr->ind;
    }
    else if (buffer->forward_ptr->buf == BUFFER_0 && buffer->current_ptr->buf == BUFFER_1)
    {
        memcpy(lexeme, buffer->BUFFER1 + buffer->current_ptr->ind, BUFFER_SIZE - buffer->current_ptr->ind);
        memcpy(lexeme + BUFFER_SIZE - buffer->current_ptr->ind, buffer->BUFFER0, buffer->forward_ptr->ind);
        size = BUFFER_SIZE - buffer->current_ptr->ind + buffer->forward_ptr->ind;
    }
    else if (buffer->forward_ptr->buf == BUFFER_1 && buffer->current_ptr->buf == BUFFER_0)
    {
        memcpy(lexeme, buffer->BUFFER0 + buffer->current_ptr->ind, BUFFER_SIZE - buffer->current_ptr->ind);
        memcpy(lexeme + BUFFER_SIZE - buffer->current_ptr->ind, buffer->BUFFER1, buffer->forward_ptr->ind);
        size = BUFFER_SIZE - buffer->current_ptr->ind + buffer->forward_ptr->ind;
    }
    lexeme[size] = '\0';
    // lexeme = (char *)realloc(lexeme, size + 1);
    return lexeme;
}


// int polyHash(char* str, int n){
//     unsigned long long hash = 0;
//     int small_prime = 31;
//     for (int i=n-1; i>=0; i--){
//         hash = (hash*small_prime + str[i]) % BIG_PRIME;
//     }
//     return hash % HASHTABLE_SIZE;
// }
int polyHash(char * str,int n)
{
    long hashVal = 0;
	for(int i=0;i<n;i++) {
		hashVal = (hashVal << 4) + *(str+i);
		long g = hashVal & 0xF0000000L;
		if (g != 0) hashVal ^= g >> 24;
		hashVal &= ~g;
	}
	return hashVal%HASHTABLE_SIZE;
}

HashTable* initializeHashTable(){
    HashTable* hash_tbl = (HashTable*)malloc(sizeof(HashTable));
    hash_tbl->Table = (Symbol**)calloc(HASHTABLE_SIZE, sizeof(Symbol*));
    hash_tbl->size = 0;
    return hash_tbl;
}

HashTable* insert2HashTable(HashTable* hash_tbl, char* key, int n, void* value){
    int hash = polyHash(key, n);
    Symbol* new_symbol = (Symbol*)malloc(sizeof(Symbol));
    // new_symbol->key = malloc(n+2*sizeof(char));
    // strcpy(new_symbol->key, key);
    new_symbol->key = key;
    new_symbol->value = value;

    new_symbol->Next = NULL;
    
    if (hash_tbl->Table[hash] == 0){
        hash_tbl->Table[hash] = new_symbol;
    }
    else{
        //collision++;
        
        // printf("Collision detected for key: %s\n", key);
        Symbol* current_ptr = hash_tbl->Table[hash];
        while (current_ptr->Next != NULL){
            current_ptr = current_ptr->Next;
        }
        current_ptr->Next = new_symbol;
    }
    hash_tbl->size++;
    return hash_tbl;
}

Symbol* search4HashTable(HashTable* hash_tbl, char*key, int n){
    int hash = polyHash(key, n);
    Symbol* current_ptr = hash_tbl->Table[hash];
    while (current_ptr){
        if(strcmp(key, current_ptr->key)==0){
            return current_ptr;
        }
        current_ptr = current_ptr->Next;
    }
    // printf(" Key Not found in HashTable: %s\n", key);
    return NULL;
}

void freeHashTable(HashTable* hash_tbl){
    for (int i=0; i<HASHTABLE_SIZE; i++){
        Symbol* curr = hash_tbl->Table[i];
        while (curr != NULL){
            Symbol* prev = curr;
            curr = curr->Next;
            free(prev);
        }
    }
    free(hash_tbl->Table);
    free(hash_tbl);
}

char* mallocString(char* s){
    char* temp = (char*)malloc(sizeof(char)*(strlen(s)+1));
    strcpy(temp, s);
    return temp;
}

void freeLookUpTable(HashTable* lookup_tbl){
    for (int i=0; i<HASHTABLE_SIZE; i++){
        Symbol* curr = lookup_tbl->Table[i];
        while (curr != NULL){
            Symbol* prev = curr;
            curr = curr->Next;
            if(prev->key){free(prev->key); prev->key=NULL;}
            free(prev);
            
        }
    }
    free(lookup_tbl->Table);
    free(lookup_tbl);
}

HashTable* initializeLookupTable(){
    HashTable* hash_tbl = initializeHashTable();
    insert2HashTable(hash_tbl, (char*)mallocString("with"),        4, TK_WITH);
    insert2HashTable(hash_tbl, (char*)mallocString("parameters"), 10, TK_PARAMETERS);
    insert2HashTable(hash_tbl, (char*)mallocString("end"),         3, TK_END);
    insert2HashTable(hash_tbl, (char*)mallocString("while"),       5, TK_WHILE);
    insert2HashTable(hash_tbl, (char*)mallocString("union"),       5, TK_UNION);
    insert2HashTable(hash_tbl, (char*)mallocString("endunion"),    8, TK_ENDUNION);
    insert2HashTable(hash_tbl, (char*)mallocString("definetype"), 10, TK_DEFINETYPE);
    insert2HashTable(hash_tbl, (char*)mallocString("as"),          2, TK_AS);
    insert2HashTable(hash_tbl, (char*)mallocString("type"),        4, TK_TYPE);
    insert2HashTable(hash_tbl, (char*)mallocString("_main"),       5, TK_MAIN);
    insert2HashTable(hash_tbl, (char*)mallocString("global"),      6, TK_GLOBAL);
    insert2HashTable(hash_tbl, (char*)mallocString("parameter"),   9, TK_PARAMETER);
    insert2HashTable(hash_tbl, (char*)mallocString("list"),        4, TK_LIST);
    // insert2HashTable(hash_tbl, mallocString([",        1, TK_SQL);
    // insert2HashTable(hash_tbl, "]",        1, TK_SQR);
    insert2HashTable(hash_tbl, (char*)mallocString("input"),       5, TK_INPUT);
    insert2HashTable(hash_tbl, (char*)mallocString("output"),      6, TK_OUTPUT);
    insert2HashTable(hash_tbl, (char*)mallocString("int"),         3, TK_INT);
    insert2HashTable(hash_tbl, (char*)mallocString("real"),        4, TK_REAL);
    insert2HashTable(hash_tbl, (char*)mallocString("endwhile"),    8, TK_ENDWHILE);
    insert2HashTable(hash_tbl, (char*)mallocString("if"),          2, TK_IF);
    insert2HashTable(hash_tbl, (char*)mallocString("then"),        4, TK_THEN);
    insert2HashTable(hash_tbl, (char*)mallocString("endif"),       5, TK_ENDIF);
    insert2HashTable(hash_tbl, (char*)mallocString("read"),        4, TK_READ);
    insert2HashTable(hash_tbl, (char*)mallocString("write"),       5, TK_WRITE);
    insert2HashTable(hash_tbl, (char*)mallocString("return"),      6, TK_RETURN);
    insert2HashTable(hash_tbl, (char*)mallocString("call"),        4, TK_CALL);
    insert2HashTable(hash_tbl, (char*)mallocString("record"),      6, TK_RECORD);
    insert2HashTable(hash_tbl, (char*)mallocString("endrecord"),   9, TK_ENDRECORD);
    insert2HashTable(hash_tbl, (char*)mallocString("else"),        4, TK_ELSE);
    // insert2HashTable(hash_tbl, mallocString("&&&",      3, TK_AND);
    // insert2HashTable(hash_tbl, "@@@",      3, TK_OR);
    // insert2HashTable(hash_tbl, "~",        1, TK_NOT);
    // insert2HashTable(hash_tbl, "$",        1, TK_DOLLAR);
    return hash_tbl;
}
void insert2ListInOrder(List*list,Token* tk){  
    ListNode* curr_element = list->head;
    if(list->head==NULL){
        ListNode * h = (ListNode*)malloc(sizeof(ListNode));
        h->mytoken=tk;
        h->prev=NULL;
        h->next=NULL;
        list->head=h;
        return;
    }
    if(curr_element->mytoken->lineno>tk->lineno)
    {
        ListNode * h = (ListNode*)malloc(sizeof(ListNode));
        h->mytoken=tk;
        list->head=h;
        h->next=curr_element;
        curr_element->prev=h;
        h->prev=NULL;
        return;
    }
    while( curr_element->next!= NULL && curr_element->next->mytoken->lineno <= tk->lineno){
        curr_element= curr_element->next;
    }

    ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
    temp->mytoken = tk;
    temp->prev = curr_element;
    temp->next = curr_element->next;
    curr_element->next = temp;
    if(temp->next != NULL){
        temp->next->prev = temp;
    }
}

TOKEN_ID lookUp(HashTable* lookupTable, char* lexeme, int n,int state){
    Symbol* symbol = search4HashTable(lookupTable, lexeme, n);
    if (symbol == NULL){
        char *st = (char*)mallocString(lexeme);
        if(state==61)
        {
            insert2HashTable(lookupTable, st,n,TK_FIELDID);
            return TK_FIELDID;
        }
        if (lexeme[0] == '_'){
            insert2HashTable(lookupTable, st, n, TK_FUNID);
            return TK_FUNID;
        }
        else{
            insert2HashTable(lookupTable, st, n, TK_ID);
            return TK_ID;
        }
    }

    return (TOKEN_ID)symbol->value;
}

Token *getToken(FILE *fp, TWIN_BUFFER *tbuff, HashTable *lookup_tbl)
{
    Token *tk = (Token *)malloc(sizeof(Token));
    tk->lexeme = (char *)calloc(6, sizeof(char));
    char * lex;
    int state = 0;
    char c;
    char s[100];
    tk->lineno = tbuff->line;
    while (1)
    {
        switch (state)
        {
        case 0:
            c = getNext(fp, tbuff);
            if (c == '[')
            {
                state = 1;
            }
            else if (c == ']')
            {
                state = 2;
            }
            else if (c == ',')
            {
                state = 3;
            }
            else if (c == ';')
            {
                state = 4;
            }
            else if (c == '>')
            {
                state = 5;
            }
            else if (c == ':')
            {
                state = 8;
            }
            else if (c == '.')
            {
                state = 9;
            }
            else if (c == '(')
            {
                state = 10;
            }
            else if (c == ')')
            {
                state = 11;
            }
            else if (c == '+')
            {
                state = 12;
            }
            else if (c == '-')
            {
                state = 13;
            }
            else if (c == '*')
            {
                state = 14;
            }
            else if (c == '/')
            {
                state = 15;
            }
            else if (c == '~')
            {
                state = 16;
            }
            else if (c == '!')
            {
                state = 17;
            }
            else if (c == '&')
            {
                state = 19;
            }
            else if (c == '@')
            {
                state = 22;
            }
            else if (c == '=')
            {
                state = 25;
            }
            else if (c == '%')
            {
                state = 27;
            }
            else if (c == '<')
            {
                state = 29;
            }
            else if (c == '#')
            {
                state = 36;
            }
            else if (c >= '0' && c <= '9')
            {
                state = 39;
            }
            else if (c == '_')
            {
                state = 50;
            }
            else if (c == ' ' || c == '\t' | c == '\n')
            {
                // todo : line number to be incremented
                state = 54;
            }
            else if (c >= 'b' && c <= 'd')
            {
                state = 56;
            }
            else if (c == 'a' || (c >= 'e' && c <= 'z'))
            {
                state = 60;
            }
            else
            {
                state = 100;
                lex =  getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Symbol <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 1:
            tk->tkId = TK_SQL;
            // s = "[");
            strcpy(s,"[");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 2:
            tk->tkId = TK_SQR;
            strcpy(s,"]");
            strcpy(tk->lexeme, s);

            return tk;
            break;
        case 3:
            tk->tkId = TK_COMMA;
            strcpy(s,",");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 4:
            tk->tkId = TK_SEM;
            strcpy(s,";");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 5:
            c = getNext(fp, tbuff);
            if (c == '=')
            {
                state = 7;
            }
            else
            {
                state = 6;
            }
            break;
        case 6:
            retract(tbuff);
            tk->tkId = TK_GT;
            strcpy(s,">");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 7:
            tk->tkId = TK_GE;
            strcpy(s,">=");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 8:
            tk->tkId = TK_COLON;
            strcpy(s,":");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 9:
            tk->tkId = TK_DOT;
            strcpy(s,".");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 10:
            tk->tkId = TK_OP;
            strcpy(s,"(");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 11:
            tk->tkId = TK_CL;
            strcpy(s,")");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 12:
            tk->tkId = TK_PLUS;
            strcpy(s,"+");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 13:
            tk->tkId = TK_MINUS;
            strcpy(s,"-");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 14:
            tk->tkId = TK_MUL;
            strcpy(s,"*");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 15:
            tk->tkId = TK_DIV;
            strcpy(s,"/");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 16:
            tk->tkId = TK_NOT;
            strcpy(s,"~");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 17:
            c = getNext(fp, tbuff);
            if (c == '=')
            {
                state = 18;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
            }
            break;
        case 18:
            tk->tkId = TK_NE;
            strcpy(s,"!=");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 19:
            c = getNext(fp, tbuff);
            if (c == '&')
            {
                state = 20;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 20:
            c = getNext(fp, tbuff);
            if (c == '&')
            {
                state = 21;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;

        case 21:
            tk->tkId = TK_AND;
            strcpy(s,"&&&");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 22:
            c = getNext(fp, tbuff);
            if (c == '@')
            {
                state = 23;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 23:
            c = getNext(fp, tbuff);
            if (c == '@')
            {
                state = 24;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line,lex);
                free(lex);
            }
            break;
        case 24:
            tk->tkId = TK_OR;
            strcpy(s,"@@@");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 25:
            c = getNext(fp, tbuff);
            if (c == '=')
                state = 26;
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 26:
            tk->tkId = TK_EQ;
            strcpy(s,"==");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 27:
            c = getNext(fp, tbuff);
            while (c != '\n')
            {
                c = getNext(fp, tbuff);
            }
            state = 28;
            break;
        case 28:
            tk->tkId = TK_COMMENT;
            strcpy(s,"%");
            strcpy(tk->lexeme, s);
            return tk;
        case 29:
            c = getNext(fp, tbuff);
            if (c == '=')
            {
                state = 31;
            }
            else if (c == '-')
            {
                state = 32;
            }
            else
            {
                state = 30;
            }
            break;
        case 30:
            retract(tbuff);
            tk->tkId = TK_LT;
            strcpy(s,"<");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 31:
            tk->tkId = TK_LE;
            strcpy(s,"<=");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 32:
            c = getNext(fp, tbuff);
            if (c == '-')
            {
                state = 34;
            }
            else
            {
                state = 33;
            }
            break;
        case 33:
            retract(tbuff);
            retract(tbuff);
            tk->tkId = TK_LT;
            strcpy(s,"<");
            strcpy(tk->lexeme, s);
            return tk;
            break;
      case 34:
            c = getNext(fp, tbuff);
            if (c == '-')
            {
                state = 35;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 35:
            tk->tkId = TK_ASSIGNOP;
            strcpy(s,"<---");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 36:
            c = getNext(fp, tbuff);
            if (c >= 'a' && c <= 'z')
            {
                state = 37;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 37:
            c = getNext(fp, tbuff);
            while (c >= 'a' && c <= 'z')
            {
                c = getNext(fp, tbuff);
            }
            state = 38;
            break;
        case 38:
            retract(tbuff);
            lex = getLexeme(tbuff);
           strcpy(s,lex);
           free(lex);
            tk->tkId = TK_RUID;
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 39:
            c = getNext(fp, tbuff);
            if (c >= '0' && c <= '9')
            {
                state = 39;
            }
            else if (c == '.')
            {
                state = 41;
            }
            else
            {
                state = 40;
            }
            break;
        case 40:
            retract(tbuff);
            lex = getLexeme(tbuff);
            strcpy(s,lex);
            free(lex);
            tk->tkId = TK_NUM;
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 41:
            c = getNext(fp, tbuff);
            if (c >= '0' && c <= '9')
            {
                state = 42;
            }
            else
            {
                // state 43
                retract(tbuff);
                retract(tbuff);
                lex = getLexeme(tbuff);
                strcpy(s,lex);
                free(lex);
                tk->tkId = TK_NUM;
                tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
                strcpy(tk->lexeme, s);
                return tk;
            }
            break;
        case 42:
            c = getNext(fp, tbuff);
            if (c >= '0' && c <= '9')
            {
                state = 44;
            }
            else
            {
                // state=43;
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 44:
            c = getNext(fp, tbuff);
            if (c >= 'E')
            {
                state = 46;
            }
            else
            {
                state = 45;
            }
            break;
        case 45:
            retract(tbuff);
            lex = getLexeme(tbuff);
           strcpy(s,lex);
           free(lex);
            tk->tkId = TK_RNUM;
            tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 46:
            c = getNext(fp, tbuff);
            if (c == '+' || c == '-')
            {
                state = 47;
            }
            else if (c >= '0' && c <= '9')
            {
                state = 48;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 47:
            c = getNext(fp, tbuff);
            if (c >= '0' && c <= '9')
            {
                state = 48;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
    
            }
            break;
        case 48:
            c = getNext(fp, tbuff);
            if (c >= '0' && c <= '9')
            {
                state = 49;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;

        case 49:
            lex = getLexeme(tbuff);
           strcpy(s,lex);
           free(lex);
           tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
            tk->tkId = TK_RNUM;
            strcpy(tk->lexeme, s);
            return tk;
            break;

        case 50:
            c = getNext(fp, tbuff);
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            {
                state = 51;
            }
            else
            {
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 51:
            c = getNext(fp, tbuff);
            while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                c = getNext(fp, tbuff);
            if (c >= '0' && c <= '9')
            {
                state = 52;
            }
            else
            {
                state = 53;
            }
            break;
        case 52:
            c = getNext(fp, tbuff);
            while (c >= '0' && c <= '9')
                c = getNext(fp, tbuff);
            state = 53;
            break;
        case 53:
            retract(tbuff);
            lex = getLexeme(tbuff);
           strcpy(s,lex);
           free(lex);
           tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
            if (strlen(s) <= 30)
            {
                strcpy(tk->lexeme ,s);
                tk->tkId = lookUp(lookup_tbl, s, strlen(s), state);
                return tk;
            }
            else
            {
                //s = "function Identifier is longer than the prescribed length of 30 characters.";
                sprintf(s,"Line %-3d function Identifier is longer than the prescribed length of 30 characters.",tbuff->line);
                state = 100;
            }
            break;
        case 54:
            c = getNext(fp, tbuff);
            while (c == ' ' || c == '\t' || c == '\n')
                c = getNext(fp, tbuff);
            retract(tbuff);
            free(tk->lexeme);
            free(tk);
            return NULL;
            break;
        case 55:
            retract(tbuff);
            tk->tkId = TK_COMMENT;
            strcpy(s,"%");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 56:
            c = getNext(fp, tbuff);
            if (c >= '2' && c <= '7')
                state = 57;
            else if (c >= 'a' && c <= 'z')
                state = 60;
            else
                state = 61;
            break;
        case 57:
            c = getNext(fp, tbuff);
            while (c >= 'b' && c <= 'd')
                c = getNext(fp, tbuff);
            if (c >= '2' && c <= '7')
                state = 58;
            else
                state = 59;
            break;
        case 58:
            c = getNext(fp, tbuff);
            while (c >= '2' && c <= '7')
                c = getNext(fp, tbuff);
            state = 59;
            break;
        case 59:
            retract(tbuff);
            lex = getLexeme(tbuff);
           strcpy(s,lex);
           free(lex);
           tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
            int len = strlen(s);
            if (len >= 2 && len <= 20)
            {
                tk->tkId = TK_ID;
                strcpy(tk->lexeme, s);
                return tk;
            }
            else
            {
                sprintf(s,"Line %-3d Variable Identifier is longer than the prescribed length of 20 characters.",tbuff->line);
                state = 100;
            }
            break;
        case 60:
            c = getNext(fp, tbuff);
            while (c >= 'a' && c <= 'z')
                c = getNext(fp, tbuff);
            state = 61;
            break;
        case 61:
            retract(tbuff);
            lex = getLexeme(tbuff);
           strcpy(s,lex);
           free(lex);
            tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
            strcpy(tk->lexeme, s);
            tk->tkId = lookUp(lookup_tbl, s, strlen(s), state);
            return tk;
            break;
        default:
            tk->tkId = TK_ERROR;
            tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
            strcpy(tk->lexeme,s);
            return tk;
        }
    }
}

/*
Aryan Barapatre 2020B4A70833P
Bhuvan Gupta    2020B4A71654P
Dhruv Singh     2020B4A70969P
Pulkit          2020B2A71932P
Jash Naik       2020B3A71958P
Siripuram Surya 2020B2A71406P
*/
#ifndef PARSEDEF_H
#define PARSEDEF_H

typedef struct STACKNODE {
        char* data;
        struct STACKNODE* next;
    } StackNode;

typedef struct Stack {
        struct STACKNODE* top;
        struct STACKNODE* tail;
    } Stack;

Stack* createStack();
void push(Stack* stack,  char* item);
void pushArray(Stack*stack, char** itemArray, int n);
char* pop(Stack* stack);
int isEmpty(Stack* stack);
int length(Stack* stack) ;
typedef struct TREENODE{
    char* id;
    int line_number;
    struct TREENODE ** childNodes;
    int numChildNodes;
}TreeNode;

TreeNode* createTreeNode(char* id);
void FreeParseTree(TreeNode* Root);
TreeNode *createParseTree(List *ll, HashTable *parseTable,List* errorlist);
void printErrorList(List* list);
#define ROW_SIZE 256
HashTable *init_parsetable(Grammar *grammer);
void createParseTable(FirstAndFollow *fnf, HashTable *parsetable, HashTable *gram_table, Grammar *grammar);
TOKEN_ID getEnum(const char *str);
void prettyPrintTable (char*** table, int maxColSize[], int n, int m);
void printParseTable(Grammar* gram,     HashTable* parsetable);
void printProdRules(Grammar* grammar, HashTable* gram_table);
void printParseTree(TreeNode *root, TreeNode *parent);
void TreePrinting(TreeNode *root,TreeNode *parent);
void freeStack(Stack* stack);
void freeList(List* list);
void freeParseTable(HashTable* table,int n);
void freeParseTree(TreeNode* root);
void freeGrammar(Grammar* grammar);
void freeHashTable(HashTable* table);
void freeFirstAndFollow(FirstAndFollow* fnf);
void freeProductionRules(HashTable* table);
void freeList(List* list);
#endif

/*
Aryan Barapatre 2020B4A70833P
Bhuvan Gupta    2020B4A71654P
Dhruv Singh     2020B4A70969P
Pulkit          2020B2A71932P
Jash Naik       2020B3A71958P
Siripuram Surya 2020B2A71406P
*/

//#include "lexer.h"
//#include "parserDef.h"

Stack* createStack() {
    Stack* stack = (Stack*)calloc(1, sizeof(Stack));
    stack->top = NULL;
    stack->tail = NULL;
    return stack;
}

void push(Stack* stack,  char* item) { 
    StackNode* newNode = (StackNode*)calloc(1, sizeof(StackNode));
    newNode->data = item; 
    newNode->next = stack->top;
    stack->top = newNode;
    if (!stack->tail) {
        stack->tail = newNode;
    }
}


void pushArray(Stack*stack, char** itemArray, int n){
    if(itemArray[n] == NULL)
        return;
    else{
        pushArray(stack, itemArray, n+1);
        push(stack, itemArray[n]);
    }

}

char* pop(Stack* stack) {
    if (stack->top == NULL) {
        printf("Stack is empty\n");
        return NULL;
    }
    char* item = stack->top->data;
    StackNode* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    if (stack->top == NULL) {
        stack->tail = NULL;
    }
    return item;
}

int isEmpty(Stack* stack) {
    return stack->top == NULL;
}

int length(Stack* stack) {
    int count = 0;
    StackNode* current = stack->top;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void printStack(Stack* stack){
    StackNode* snode =stack->top;
    for (int i=0; snode!= NULL; i++){
        printf("%d: %s\n",i, snode->data);
        snode = snode->next;
    }
}

//linkedlist.c added
List* createList(){
    List* list=(List*)calloc(1,sizeof(List));
    list->head=NULL;
    list->tail=NULL;
    return list;
}

ListNode* insertToList(List* list,Token* token){
    ListNode* newNode=(ListNode*)calloc(1,sizeof(ListNode));
    newNode->mytoken=token;
    if(list->head==NULL){
        newNode->next=NULL;
        newNode->prev=NULL;
        list->head=newNode;
        list->tail=newNode;
        list->current=newNode;
        list->num=1;
        return list->head;
    }
    ListNode* temp;
    temp=list->tail;
    temp->next= newNode;
    list->tail= newNode;
    newNode->next= NULL;
    newNode->prev= temp;
    list->num++;
    return list->head;
}


void printList(List* list, FILE* outputPointer){
    ListNode* temp=list->head;
    while(temp!=NULL){
        Token* tk=temp->mytoken;
        // printf("%s",outputPointer);
        fprintf(outputPointer,"Line no. %d Lexeme: %s TK_ID: %s \n",tk->lineno,tk->lexeme,token_str(tk->tkId));
        temp=temp->next;
    }
}
void printErrorList(List* list){
    if(list->head!=NULL){
        printf("==============----------------------><><><ERROR LIST><><><-------------------------=============\n\n");
    }
    ListNode* temp=list->head;
    while(temp!=NULL){
        Token* tk=temp->mytoken;
        // printf("%s",outputPointer);
        printf("%s\n", tk->lexeme);
        temp=temp->next;
    }
    printf("\n");
}

// void deleteListNode(List* list){
//     ListNode* temp = list->head;
//     if(temp != NULL){
//         list->head = temp->next;
//         if(temp->next != NULL){
//             temp->next->prev = NULL;
//         }
//         //free(temp->mytoken);
//         free(temp);
//         list->num--;
//     }
// }


#define BIG_PRIME 2147483647
#define ROW_SIZE 256
int collision = 0;


int unionSet(char ** a, char * b){
    int i=0;
    while(a[i] != NULL){
        if(strcmp(a[i], b) == 0){
            return 0;
        }
        i++;
    }a[i] = b;
    return 1;
}

void unionSet2(char ** a, char ** b){
    int i=0;
    while(b[i] != NULL){
        if(strcmp(b[i],"TK_EPSILON")!=0)unionSet(a, b[i]);//skip epsilon
        i++;
    }
}


char* token_str(TOKEN_ID id) {
    switch(id) {
        case TK_ASSIGNOP: return "TK_ASSIGNOP";
        case TK_COMMENT: return "TK_COMMENT";
        case TK_FIELDID: return "TK_FIELDID";
        case TK_ID: return "TK_ID";
        case TK_NUM: return "TK_NUM";
        case TK_RNUM: return "TK_RNUM";
        case TK_FUNID: return "TK_FUNID";
        case TK_RUID: return "TK_RUID";
        case TK_WITH: return "TK_WITH";
        case TK_PARAMETERS: return "TK_PARAMETERS";
        case TK_END: return "TK_END";
        case TK_WHILE: return "TK_WHILE";
        case TK_UNION: return "TK_UNION";
        case TK_ENDUNION: return "TK_ENDUNION";
        case TK_DEFINETYPE: return "TK_DEFINETYPE";
        case TK_AS: return "TK_AS";
        case TK_TYPE: return "TK_TYPE";
        case TK_MAIN: return "TK_MAIN";
        case TK_GLOBAL: return "TK_GLOBAL";
        case TK_PARAMETER: return "TK_PARAMETER";
        case TK_LIST: return "TK_LIST";
        case TK_SQL: return "TK_SQL";
        case TK_SQR: return "TK_SQR";
        case TK_INPUT: return "TK_INPUT";
        case TK_OUTPUT: return "TK_OUTPUT";
        case TK_INT: return "TK_INT";
        case TK_REAL: return "TK_REAL";
        case TK_COMMA: return "TK_COMMA";
        case TK_SEM: return "TK_SEM";
        case TK_COLON: return "TK_COLON";
        case TK_DOT: return "TK_DOT";
        case TK_ENDWHILE: return "TK_ENDWHILE";
        case TK_OP: return "TK_OP";
        case TK_CL: return "TK_CL";
        case TK_IF: return "TK_IF";
        case TK_THEN: return "TK_THEN";
        case TK_ENDIF: return "TK_ENDIF";
        case TK_READ: return "TK_READ";
        case TK_WRITE: return "TK_WRITE";
        case TK_RETURN: return "TK_RETURN";
        case TK_PLUS: return "TK_PLUS";
        case TK_MINUS: return "TK_MINUS";
        case TK_MUL: return "TK_MUL";
        case TK_DIV: return "TK_DIV";
        case TK_CALL: return "TK_CALL";
        case TK_RECORD: return "TK_RECORD";
        case TK_ENDRECORD: return "TK_ENDRECORD";
        case TK_ELSE: return "TK_ELSE";
        case TK_AND: return "TK_AND";
        case TK_OR: return "TK_OR";
        case TK_NOT: return "TK_NOT";
        case TK_LT: return "TK_LT";
        case TK_LE: return "TK_LE";
        case TK_EQ: return "TK_EQ";
        case TK_GT: return "TK_GT";
        case TK_GE: return "TK_GE";
        case TK_NE: return "TK_NE";
        case TK_ERROR: return "TK_ERROR";
        case TK_DOLLAR: return "TK_DOLLAR";
        default: return "UNKNOWN_TOKEN";
    }
}

char **  allocateProdRule(char* line,Grammar* gram){
    char** string_array = (char**) calloc(9, sizeof(char*));
    char * rest_sub_rhs;
    int num_of_strings = 0;
    char  *sub_token;
    while ((sub_token = strtok_r(line, " ", &rest_sub_rhs))) {
                int n = strlen(sub_token);
                if(sub_token[n-1] == '\n')
                    sub_token[n-1] = '\0';
                if(strcmp(sub_token,"")==0) continue;
                // printf("sub_token: %s\n", sub_token);
                char* single_string ;
                single_string = (char*) calloc(n+1, sizeof(char));
                strcpy(single_string, sub_token);
                if (single_string[strlen(single_string)-1] == '\n')
                    single_string[strlen(single_string)-1] = '\0';
                single_string[n] = '\0';
                string_array[num_of_strings++]=single_string;
                line = NULL; // NULL the rhs pointer for the next call
                if(single_string[0] == 'T' && single_string[1] == 'K' && single_string[2] == '_' ){
                    gram->terminalsCount+= unionSet(gram->terminals, single_string);
                }
                else{
                    gram->nonTerminalsCount+=unionSet(gram->nonTerminals, single_string);
                }
    }
    // printf("Num of strings: %d\n", num_of_strings);
    string_array  = realloc(string_array, (num_of_strings +1)*sizeof(char*));
    return string_array;
}

int calculateFirst(HashTable* gramTable, char* key,int ind, int n, char ** firstSet,int recDepth){    int ret=0;
    Symbol* symb= search4HashTable(gramTable, key, n);
    if(symb == NULL){
        printf("KEY NOT FOUND for calc First : %s", key );
        return 0;
    }else{
        char*** prodRulez = ((ProductionRuleRHS*)symb->value)->rhs;
        int i =0;
        while(prodRulez[i] != NULL){
            // printf("\tRULE %d : ", i+1);
            char** curr_arr = prodRulez[i];
            int j=0;
            if(strcmp(curr_arr[0], "TK_EPSILON") == 0){       
                    if(recDepth==0){
                        unionSet(firstSet,  curr_arr[0]);
                    }
                    ret= 1;
            }
             else if( curr_arr[j][0] == 'T' && curr_arr[j][1] == 'K' && curr_arr[j][2] == '_' ){
                unionSet(firstSet,  curr_arr[j]);
            }
            else {
                // printf("Non Terminal: %s\n", curr_arr[j]);
                int next =1;
                while(curr_arr[j] != NULL && next == 1){
                    next = calculateFirst(gramTable, curr_arr[j],ind, strlen(curr_arr[j]), firstSet, recDepth+1);
                    if(next == 1){
                        j++;
                            if( curr_arr[j][0] == 'T' && curr_arr[j][1] == 'K' && curr_arr[j][2] == '_' ){
                                unionSet(firstSet, curr_arr[j]);
                                return 0;
                            }
                    }
                    else{
                        break;  
                    }
                }
            }
            i++;
        }
    }
    return ret;
}

void calculateFollow(HashTable* gramTable, char* key, int n, char ** followSet,HashTable* firstset, char ** detectRecursion){ 
    if(strcmp(key,"<program>")==0)
    {
        followSet[0]="TK_DOLLAR";
        return;
    }
    // printf("Calculating Follow for: %s\n", key);
    if(unionSet(detectRecursion, key)==0)
        return;
    for(int i=0;i<HASHTABLE_SIZE;i++){
        if(gramTable->Table[i] != NULL){
            Symbol* symb = gramTable->Table[i];
            while(symb != NULL){
                for(int j=0;j<((ProductionRuleRHS*)symb->value)->noRHS;j++){
                    char** curr_arr = ((ProductionRuleRHS*)symb->value)->rhs[j];
                    int k=0;
                    int keyPresent=0;
                    while(curr_arr[k] != NULL){
                        if(strcmp(key,curr_arr[k])==0){
                               keyPresent=1;
                               break;
                        }
                        k++;
                    }
                    if(keyPresent==1)
                    {
                        k++;
                        if(curr_arr[k]==NULL){
                                    if (strcmp(key, symb->key)!=0)
                                        calculateFollow(gramTable, symb->key, strlen(symb->key), followSet,firstset,detectRecursion); 
                        }
                        else if(curr_arr[k][0] == 'T' && curr_arr[k][1] == 'K' && curr_arr[k][2] == '_' ){
                                        unionSet(followSet, curr_arr[k]);
                        }
                        else{
                            int flag=0;
                            while(curr_arr[k] != NULL){
                                flag=0;
                                char* key = curr_arr[k];
                                if(curr_arr[k][0] == 'T' && curr_arr[k][1] == 'K' && curr_arr[k][2] == '_' ){
                                        unionSet(followSet, curr_arr[k]);
                                        break;
                                }
                                Symbol* symb= search4HashTable(firstset, key, strlen(key));
                                if(symb == NULL){
                                    printf("Key not found: %s", symb->key );
                                }else{
                                    char**  set2 = (char**)symb->value;
                                    unionSet2(followSet, set2);
                                    int x=0;
                                    while(set2[x] != NULL)
                                    {
                                        if(strcmp(set2[x],"TK_EPSILON")==0)
                                            {
                                            flag=1;
                                            break;
                                            }
                                        x++;
                                    }
                                    if(flag!=1)
                                    {
                                        break;
                                    }
                                }  
                                k++;
                            }
                            if(flag==1){
                                calculateFollow(gramTable, symb->key, strlen(symb->key), followSet,firstset,detectRecursion); 
                            }
                        }
                    }
                }
                symb = symb->Next;
            }
        }
    }
}

Grammar* init_grammar()
{
    Grammar* gram=(Grammar *)malloc(sizeof(Grammar));
    gram->nonTerminals=calloc(70,sizeof(char*));
    gram->terminals=calloc(70,sizeof(char*));
    gram->terminalsCount=0;
    gram->nonTerminalsCount=0;
    return gram;
}
FirstAndFollow* init_FirstAndFollow()
{
    FirstAndFollow* firstAndFollow=(FirstAndFollow *)malloc(sizeof(FirstAndFollow));
    firstAndFollow->first=initializeHashTable();
    firstAndFollow->follow=initializeHashTable();
    return firstAndFollow;
}
HashTable* LoadProductionRules(FILE* f,Grammar* gram)
{
    char line [256];
    HashTable* gramTable=initializeHashTable();
    while (fgets(line, sizeof(line), f)) {
        char *lhs, *rhs;
        char *rest_rhs;
        lhs = strtok_r(line, "=", &rest_rhs);
        ProductionRuleRHS* productionRule=malloc(sizeof(ProductionRuleRHS));
        productionRule->noRHS=0;
        productionRule->rhs=calloc(9,sizeof(char**));

        while ((rhs = strtok_r(rest_rhs, "|", &rest_rhs))) {
            productionRule->rhs[productionRule->noRHS++]=allocateProdRule(rhs,gram);
        }
        productionRule->rhs = realloc(productionRule->rhs, (productionRule->noRHS+1)*sizeof(char**));

        char * key= (char*)calloc(strlen(lhs)+1,sizeof(char));
        strcpy(key,lhs);
        insert2HashTable(gramTable,key,strlen(key),productionRule);
        Symbol* symb= search4HashTable(gramTable, lhs, strlen(lhs));
        if(symb == NULL){
            printf("KEY NOT FOUND  LoadProductionRules: %s", lhs );
        }else{
            gram->nonTerminalsCount+= unionSet(gram->nonTerminals,symb->key);
            // char*** prodRulez = ((ProductionRuleRHS*)symb->value)->rhs;
        }
    }
    // printf("Terminals: %d\n", gram->terminalsCount);
    // printf("Non Terminals: %d\n", gram->nonTerminalsCount);
    return gramTable;
}

TOKEN_ID getEnum(const char * str){
    if (strcmp(str, "TK_ASSIGNOP") == 0) return TK_ASSIGNOP;
    else if (strcmp(str, "TK_COMMENT") == 0) return TK_COMMENT;
    else if (strcmp(str, "TK_FIELDID") == 0) return TK_FIELDID;
    else if (strcmp(str, "TK_ID") == 0) return TK_ID;
    else if (strcmp(str, "TK_NUM") == 0) return TK_NUM;
    else if (strcmp(str, "TK_RNUM") == 0) return TK_RNUM;
    else if (strcmp(str, "TK_FUNID") == 0) return TK_FUNID;
    else if (strcmp(str, "TK_RUID") == 0) return TK_RUID;
    else if (strcmp(str, "TK_WITH") == 0) return TK_WITH;
    else if (strcmp(str, "TK_PARAMETERS") == 0) return TK_PARAMETERS;
    else if (strcmp(str, "TK_END") == 0) return TK_END;
    else if (strcmp(str, "TK_WHILE") == 0) return TK_WHILE;
    else if (strcmp(str, "TK_UNION") == 0) return TK_UNION;
    else if (strcmp(str, "TK_ENDUNION") == 0) return TK_ENDUNION;
    else if (strcmp(str, "TK_DEFINETYPE") == 0) return TK_DEFINETYPE;
    else if (strcmp(str, "TK_AS") == 0) return TK_AS;
    else if (strcmp(str, "TK_TYPE") == 0) return TK_TYPE;
    else if (strcmp(str, "TK_MAIN") == 0) return TK_MAIN;
    else if (strcmp(str, "TK_GLOBAL") == 0) return TK_GLOBAL;
    else if (strcmp(str, "TK_PARAMETER") == 0) return TK_PARAMETER;
    else if (strcmp(str, "TK_LIST") == 0) return TK_LIST;
    else if (strcmp(str, "TK_SQL") == 0) return TK_SQL;
    else if (strcmp(str, "TK_SQR") == 0) return TK_SQR;
    else if (strcmp(str, "TK_INPUT") == 0) return TK_INPUT;
    else if (strcmp(str, "TK_OUTPUT") == 0) return TK_OUTPUT;
    else if (strcmp(str, "TK_INT") == 0) return TK_INT;
    else if (strcmp(str, "TK_REAL") == 0) return TK_REAL;
    else if (strcmp(str, "TK_COMMA") == 0) return TK_COMMA;
    else if (strcmp(str, "TK_SEM") == 0) return TK_SEM;
    else if (strcmp(str, "TK_COLON") == 0) return TK_COLON;
    else if (strcmp(str, "TK_DOT") == 0) return TK_DOT;
    else if (strcmp(str, "TK_ENDWHILE") == 0) return TK_ENDWHILE;
    else if (strcmp(str, "TK_OP") == 0) return TK_OP;
    else if (strcmp(str, "TK_CL") == 0) return TK_CL;
    else if (strcmp(str, "TK_IF") == 0) return TK_IF;
    else if (strcmp(str, "TK_THEN") == 0) return TK_THEN;
    else if (strcmp(str, "TK_ENDIF") == 0) return TK_ENDIF;
    else if (strcmp(str, "TK_READ") == 0) return TK_READ;
    else if (strcmp(str, "TK_WRITE") == 0) return TK_WRITE;
    else if (strcmp(str, "TK_RETURN") == 0) return TK_RETURN;
    else if (strcmp(str, "TK_PLUS") == 0) return TK_PLUS;
    else if (strcmp(str, "TK_MINUS") == 0) return TK_MINUS;
    else if (strcmp(str, "TK_MUL") == 0) return TK_MUL;
    else if (strcmp(str, "TK_DIV") == 0) return TK_DIV;
    else if (strcmp(str, "TK_CALL") == 0) return TK_CALL;
    else if (strcmp(str, "TK_RECORD") == 0) return TK_RECORD;
    else if (strcmp(str, "TK_ENDRECORD") == 0) return TK_ENDRECORD;
    else if (strcmp(str, "TK_ELSE") == 0) return TK_ELSE;
    else if (strcmp(str, "TK_AND") == 0) return TK_AND;
    else if (strcmp(str, "TK_OR") == 0) return TK_OR;
    else if (strcmp(str, "TK_NOT") == 0) return TK_NOT;
    else if (strcmp(str, "TK_LT") == 0) return TK_LT;
    else if (strcmp(str, "TK_LE") == 0) return TK_LE;
    else if (strcmp(str, "TK_EQ") == 0) return TK_EQ;
    else if (strcmp(str, "TK_GT") == 0) return TK_GT;
    else if (strcmp(str, "TK_GE") == 0) return TK_GE;
    else if (strcmp(str, "TK_NE") == 0) return TK_NE;
    else if (strcmp(str, "TK_ERROR") == 0) return TK_ERROR;
    else if (strcmp(str, "TK_DOLLAR") == 0) return TK_DOLLAR;
    else return TK_EPSILON; // If none matched
}
HashTable* init_parsetable(Grammar * grammer ){
    HashTable* parsetable = initializeHashTable();
    for (int i=0; i<grammer->nonTerminalsCount; i++){
        char * nonTerminal= grammer->nonTerminals[i];
        char*** TerminalRow = calloc(grammer->terminalsCount+2, sizeof(char**));
        insert2HashTable(parsetable, nonTerminal, strlen(nonTerminal), TerminalRow);     
    }return parsetable;
}

void createParseTable(FirstAndFollow* fnf, HashTable* parsetable, HashTable* gram_table, Grammar* grammar){
    
    char ** synTokenArr = (char**)calloc(1, sizeof(char*));
    synTokenArr[0]="TK_SYN";
    for (int i=0 ;i<grammar->nonTerminalsCount; i++){
        char* nonTerminal = grammar->nonTerminals[i];
        int len = strlen(nonTerminal);
        Symbol* parseSymbol = search4HashTable(parsetable,nonTerminal, len);
        char** followSet = (char**)search4HashTable(fnf->follow,nonTerminal, len)->value;

        ProductionRuleRHS* productionRulezset = (ProductionRuleRHS*)search4HashTable(gram_table, nonTerminal, len)->value;
        char*** productionRules=productionRulezset->rhs;
        int productionRulesCount=productionRulezset->noRHS;
        int flag=0;
        for(int j=0; j<productionRulesCount;j++)
        {
            int i=0;
            while(productionRules[j][i]!=NULL)
            {
            int flag2=0;
            char* s=productionRules[j][i];
            if(strcmp(s,"TK_EPSILON")==0)
            {
                int k=0;
                while(followSet[k]!=NULL)
                {
                    ((char***)parseSymbol->value)[getEnum(followSet[k])]=productionRules[j];
                    k++;
                }
                flag=1;
            }
            else if (strlen(s)>=2&&(s[0]=='T' && s[1] == 'K' && s[2]=='_'))
            {
                    ((char***)parseSymbol->value)[getEnum(s)]=productionRules[j];
            }
            else
            {
                char** newfirstSet =(char**)search4HashTable(fnf->first,s, strlen(s))->value;
                int k=0;
                while(newfirstSet[k]!=NULL)
                {
                    if(strcmp(newfirstSet[k],"TK_EPSILON")==0)
                        flag2=1;
                    ((char***)parseSymbol->value)[getEnum(newfirstSet[k])]=productionRules[j];
                    k++;
                }
            }
            if(flag2!=1)
            break;
            i++;
            }
        }
        if(flag!=1)
        {
            int k=0;
            while(followSet[k]!=NULL)
                {
                    if(((char***)parseSymbol->value)[getEnum(followSet[k])]==NULL)
                    ((char***)parseSymbol->value)[getEnum(followSet[k])]=synTokenArr;
                    //printf("Syn Token for %s: %s\n",followSet[k],synToken);
                    k++;
                }
        }
    }   
}
void freeParseTable(HashTable* parsetable,int n){
    for(int i=0;i<HASHTABLE_SIZE;i++){
        if(parsetable->Table[i] != NULL){
            Symbol* symb = parsetable->Table[i];
            while(symb != NULL){

                free(symb->value);
                symb = symb->Next;
            }
        }
    }
    freeHashTable(parsetable);
}
void freeList(List* list){
    ListNode* curr = list->head;
    while (curr != NULL){
        ListNode* temp = curr;
        curr = curr->next;
        free(temp->mytoken->lexeme);
        free(temp->mytoken);
        free(temp);
    }
    free(list);
}
void freeFirstAndFollow(FirstAndFollow* fnf){
    HashTable * hash_tbl = fnf->first;
    for (int i=0; i<HASHTABLE_SIZE; i++){
        Symbol* curr = hash_tbl->Table[i];
        while (curr != NULL){
            Symbol* prev = curr;
            curr = curr->Next;
            free(prev->value);
            free(prev);
        }
    }
    free(hash_tbl->Table);
    free(hash_tbl);
    hash_tbl = fnf->follow;
    for (int i=0; i<HASHTABLE_SIZE; i++){
        Symbol* curr = hash_tbl->Table[i];
        while (curr != NULL){
            Symbol* prev = curr;
            curr = curr->Next;
            free(prev->value);
            free(prev);
        }
    }
    free(hash_tbl->Table);
    free(hash_tbl);
    free(fnf);
}
void freeGrammar(Grammar* gram){
    free(gram->nonTerminals);
    free(gram->terminals);
    free(gram);
}

void freeProductionRules(HashTable* table){
    for(int i=0;i<HASHTABLE_SIZE;i++){
        if(table->Table[i] != NULL){
            Symbol* symb = table->Table[i];
            while(symb != NULL){
                Symbol* temp = symb;
                symb = symb->Next;
                char*** currProdRulez = ((ProductionRuleRHS*)temp->value)->rhs;
                int i=0;
                while(currProdRulez[i] != NULL){
                        for(int j=0; currProdRulez[i][j]!= NULL; j++){
                            free(currProdRulez[i][j]);
                            currProdRulez[i][j] = NULL;
                        }
                        free(currProdRulez[i]);
                        currProdRulez[i] = NULL;
                        i++;
                }
                free(currProdRulez);
                free(temp->value);
                free(temp->key);
                free(temp);
            }
        }
    }
    free(table->Table);
    free(table);
}



void freeParseTree(TreeNode* node){
    if(node != NULL){ 
        for(int i=0;i<node->numChildNodes;i++){
            if(node->childNodes[i] != NULL){
                freeParseTree(node->childNodes[i]);
                node->childNodes[i] = NULL;
            }
        }
        if(node->id && strcmp(node->id, "<program>") == 0) {free(node->id);}
        free(node->childNodes);
        node->childNodes = NULL;
        free(node);
        node = NULL;
    }
}

void freeStack(Stack* stack){
    StackNode* temp = stack->top;
    while(temp != NULL){
        stack->top = stack->top->next;
        // free(temp->data);
        free(temp);
        temp = stack->top;
    }
    free(stack);
}


int isStopWord(TOKEN_ID tkId){
    return (tkId == TK_END || tkId == TK_ENDIF || tkId == TK_ENDRECORD || tkId == TK_ENDUNION || tkId == TK_ENDWHILE || tkId == TK_SEM || tkId == TK_THEN || tkId ==TK_ELSE);
}

TreeNode *createTreeNode(char *id)
{
    TreeNode *node = (TreeNode *)calloc(1,sizeof(TreeNode));
    node->id = id;
    node->childNodes = (TreeNode **)calloc(10, sizeof(TreeNode *));
    node->numChildNodes = 0;
    return node;
}

int allocateParseTree(List *ll, HashTable *parseTable, TreeNode *currentNode, Stack *st,List *errorlist)
{
    if (isEmpty(st)||(strcmp(st->top->data,"TK_DOLLAR")==0))
        return 0;
    else if (ll == NULL||ll->current==NULL)
    {
        printf("list current is empty\n");
        printf("ERROR stack top is : %s\n",st->top->data);
        printStack(st);
        return -1;
    }
    ListNode *listHead = ll->current;

    Token *tk = listHead->mytoken;
    TOKEN_ID currentId = tk->tkId;
    currentNode->line_number=tk->lineno;
    char *currentTop = pop(st);
    if (strcmp("TK_EPSILON", currentTop) == 0)
    {
        return 0;
    }
    else if (currentTop[0] == 'T' && currentTop[1] == 'K' && currentTop[2] == '_')
    {
        
        if(strcmp(currentTop, token_str(currentId)) != 0){
            //Send a panic signal '1' to Parent Non-terminal node for error recovery
            // For an error, terminal contains first child as "SYN_ERROR" and second child as the lexeme causing error
            char* buf = calloc(120, sizeof(char));
            sprintf(buf, "Line %-3d Error: The token %s for lexeme %s  does not match with the expected token %s",tk->lineno, token_str(tk->tkId), tk->lexeme, currentTop);
            Token* errortoken=(Token*)malloc(sizeof(Token));
            errortoken->lineno=tk->lineno;
            errortoken->tkId=TK_ERROR;
            errortoken->lexeme=buf;
            insert2ListInOrder(errorlist,errortoken);
            // char* temp = (char*)malloc(sizeof(char)*9);
            // strcpy(temp,"TK_ERROR");
            currentNode->childNodes[0] = createTreeNode("TK_ERROR");
            currentNode->numChildNodes = 1;
            if(!isStopWord(tk->tkId))
                ll->current = ll->current->next;//deleteListNode(ll);
            return 1;
        }
        currentNode->childNodes[currentNode->numChildNodes] = createTreeNode(tk->lexeme);
        currentNode->numChildNodes++;
        //deleteListNode(ll);
        ll->current = ll->current->next;
    }
    else
    {
        Symbol *symb = search4HashTable(parseTable, currentTop, strlen(currentTop));
        char **ProductionRule = ((char ***)symb->value)[currentId]; 
        if (ProductionRule == NULL)
        {   
            while (ll->num != 0)
            {
                Token *t = ll->current->mytoken;
                char **a = ((char ***)symb->value)[t->tkId];
                if (isStopWord(t->tkId) || (a!= NULL && ((strcmp(a[0], "TK_EPSILON") == 0) || strcmp(a[0], "TK_SYN") == 0) ))
                {
                    break;
                }
                //deleteListNode(ll);
                ll->current = ll->current->next;
            }
            if(ll->current!=NULL)
            {
            //Line 13 Error: Invalid token TK_OP encountered with value ( stack top logicalOp
            char* buf = (char*)malloc(sizeof(char)*120);
            Token* t=ll->current->mytoken;
            sprintf(buf,"Line %-3d Error: Invalid token %s encountered with value %s stack top %s",t->lineno,token_str(t->tkId),t->lexeme,currentTop);
            //strcpy(buf,"TK_ERROR");
            Token* errortoken=(Token*)malloc(sizeof(Token));
            errortoken->lineno=tk->lineno;
            errortoken->tkId=TK_ERROR;
            errortoken->lexeme=buf;
            insert2ListInOrder(errorlist,errortoken);
            // char* temp = (char*)malloc(sizeof(char)*9);
            // strcpy(temp,"TK_ERROR");
            currentNode->childNodes[0] = createTreeNode("TK_ERROR");
            currentNode->numChildNodes++;

            }
        }
        else if (strcmp(ProductionRule[0], "TK_SYN") != 0)
        {
            pushArray(st, ProductionRule, 0);
            int error_skip = 0;
            for (int i = 0; ProductionRule[i] != NULL; i++)
            {
                currentNode->childNodes[i] = createTreeNode(ProductionRule[i]);
                currentNode->numChildNodes++;
                if(error_skip ==1 && !isStopWord(getEnum(ProductionRule[i]))){
                    TreeNode* child = currentNode->childNodes[i];
                    // char* temp = (char*)malloc(sizeof(char)*9);
                    // strcpy(temp,"TK_ERROR");
                    // currentNode->childNodes[0] = createTreeNode("TK_ERROR");
                    child->childNodes[0] = createTreeNode("TK_ERROR");
                    child->numChildNodes=1;
                    pop(st);
                    continue;
                }
                int detect_error = allocateParseTree(ll, parseTable, currentNode->childNodes[i], st,errorlist);
                if (detect_error == 1){ //Enter panic mode, del child node until syn of linenumber changes
                    error_skip = 1;
                    int curr_line_no = ll->current->mytoken->lineno;
                    TOKEN_ID currentTokenId = ll->current->mytoken->tkId;
                    Symbol* sy=search4HashTable(parseTable, currentTop, strlen(currentTop));
                    char** prodRule = ((char***)(sy->value))[currentTokenId];
                    while (ll->num > 1 && ll->current->mytoken->lineno == curr_line_no && !isStopWord(ll->current->mytoken->tkId)
                                        &&( prodRule == NULL || (strcmp(prodRule[0], "TK_SYN") != 0) ) ){
                        ll->current = ll->current->next;//deleteListNode(ll); 
                        prodRule = ((char***)(sy->value))[ll->current->mytoken->tkId];
                    }
                }
            }
        }
        else{
            if(ll->current!=NULL)
            {
            char* buf = (char*)malloc(sizeof(char)*128);
            Token* t=ll->current->mytoken;
            sprintf(buf,"Line %-3d Error: Invalid token %s encountered with value %s stack top %s",t->lineno,token_str(t->tkId),t->lexeme,currentTop);
            // buf=realloc(buf,strlen(buf)+1);
            Token* errortoken=(Token*)malloc(sizeof(Token));
            errortoken->lineno=t->lineno;
            errortoken->tkId=TK_ERROR;
            errortoken->lexeme=buf;
            insert2ListInOrder(errorlist,errortoken);
            // char* temp = (char*)malloc(sizeof(char)*9);
            // strcpy(temp,"TK_ERROR");
            currentNode->childNodes[0] = createTreeNode("TK_ERROR");
            currentNode->numChildNodes++;
            }
        }
    }
    return 0;
}

TreeNode *createParseTree(List *ll, HashTable *parseTable,List* errorlist)
{
    if(ll->head == NULL){
        printf("Empty list\n");
        return NULL;
    }
    if(parseTable == NULL){
        printf("Empty parseTable\n");
        return NULL;
    }

    Stack *st = createStack();
    char *startString = malloc(sizeof(char) * strlen("<program>")+1);
    strcpy(startString, "<program>");
    push(st, startString);
    TreeNode* startNode;
    startNode= createTreeNode(startString);
    allocateParseTree(ll, parseTable, startNode, st,errorlist);
    // free(startString);
    freeStack(st);
    return startNode;
}

/*
Aryan Barapatre 2020B4A70833P
Bhuvan Gupta    2020B4A71654P
Dhruv Singh     2020B4A70969P
Pulkit          2020B2A71932P
Jash Naik       2020B3A71958P
Siripuram Surya 2020B2A71406P
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
//#include "lexer.h"
//#include "parserDef.h"
#include <time.h>
FILE *outputPointer = NULL;
int max(int a, int b)
{
    return (a > b) ? a : b;
}

void prettyPrintTable(char ***table, int maxColSize[], int n, int m);

void printParseTable(Grammar *gram, HashTable *parsetable)
{
    char ***strArr = (char ***)calloc(gram->nonTerminalsCount + 1, sizeof(char **));
    for (int i = 0; i <= gram->nonTerminalsCount; i++)
        strArr[i] = calloc(gram->terminalsCount + 1, sizeof(char *));
    int *maxColSize = (int *)calloc(gram->terminalsCount + 1, sizeof(int));
    char *key_not_found, *tk_error;
    key_not_found = (char *)malloc(sizeof(char) * strlen("key not found"));
    tk_error = (char *)malloc(sizeof(char) * strlen("TK_ERROR"));
    strcpy(key_not_found, "key not found");
    strcpy(tk_error, "TK_ERROR");
    // memset(&maxColSize, 0, gram->terminalsCount+1);  // Print the top row header (Terminals)
    // printf("%-22s", " "); // Placeholder for the left top corner
    maxColSize[0] = 0;
    for (int j = 0; j < gram->terminalsCount; j++)
    {
        const char *s = token_str(j);
        char *st = malloc(sizeof(char *) * strlen(s));
        strcpy(st, s);
        strArr[0][j + 1] = st;
        maxColSize[j + 1] = strlen(strArr[0][j + 1]);
        // printf("| %-55s ", token_str(j)); // Adjust the width as needed
    }

    // Iterate through nonTerminals (Rows)
    for (int i = 0; i < gram->nonTerminalsCount; i++)
    {
        char *key = gram->nonTerminals[i];
        strArr[i + 1][0] = key;
        maxColSize[0] = max(maxColSize[0], strlen(key));
        Symbol *symb = search4HashTable(parsetable, key, strlen(key));
        if (symb == NULL)
        {
            for (int j = 1; j <= gram->terminalsCount; j++)
            {
                strArr[i + 1][j] = key_not_found;
            }
        }
        else
        {
            char ***parseRow = (char ***)symb->value;

            // Print the row header
            // printf("%-22s", key); // Adjust the width as needed to match the header
            strArr[i + 1][0] = key;
            maxColSize[0] = max(maxColSize[0], strlen(key));
            // Print the row values
            for (int j = 0; j < gram->terminalsCount; j++)
            {
                if (parseRow[j] == NULL)
                {
                    strArr[i + 1][j + 1] = tk_error;
                    // printf("| %-55s ", "TK_ERROR"); // Print NULL for empty cells
                }
                else if (strcmp(parseRow[j][0], "TK_SYN") == 0)
                {
                    strArr[i + 1][j + 1] = parseRow[j][0];
                }
                else
                {
                    if (j == 11 && i == 3)
                    {
                        printf("");
                    }
                    char *buff = malloc((ROW_SIZE + 1) * sizeof(char));
                    sprintf(buff, "%s -> %s", key, parseRow[j][0]);
                    int k = strlen(buff);
                    for (int l = 1; parseRow[j][l] != NULL && k <= ROW_SIZE; l++)
                    {
                        sprintf(buff, "%s %s", buff, parseRow[j][l]);
                        k += strlen(parseRow[j][l]);
                    }
                    strArr[i + 1][j + 1] = buff;
                    // Adjust the width as needed
                }
                maxColSize[j + 1] = max(maxColSize[j + 1], strlen(strArr[i + 1][j + 1]));
            }
        }
    }
    prettyPrintTable(strArr, maxColSize, gram->nonTerminalsCount + 1, gram->terminalsCount + 1);
}

void prettyPrintTable(char ***table, int *maxColSize, int n, int m)
{
    fprintf(outputPointer, "+");
    for (int j = 0; j < m; j++)
    {
        for (int k = 0; k <= maxColSize[j]; k++)
            fprintf(outputPointer, "-");
        fprintf(outputPointer, "+");
    }
    fprintf(outputPointer, "\n");
    fprintf(outputPointer, "|");
    for (int j = 0; j < m; j++)
    {
        fprintf(outputPointer, "%-*s", maxColSize[j] + 1, table[0][j]);
        fprintf(outputPointer, "|");
    }
    fprintf(outputPointer, "\n");
    fprintf(outputPointer, "+");
    for (int j = 0; j < m; j++)
    {
        for (int k = 0; k < maxColSize[j] + 1; k++)
            fprintf(outputPointer, "-");
        fprintf(outputPointer, "+");
    }
    fprintf(outputPointer, "\n");
    for (int i = 1; i < n; i++)
    {
        fprintf(outputPointer, "|");
        for (int j = 0; j < m; j++)
        {
            fprintf(outputPointer, "%-*s", maxColSize[j] + 1, table[i][j]);
            fprintf(outputPointer, "|");
        }
        fprintf(outputPointer, "\n");
    }
    fprintf(outputPointer, "+");
    for (int j = 0; j < m; j++)
    {
        for (int k = 0; k < maxColSize[j] + 1; k++)
            fprintf(outputPointer, "-");
        fprintf(outputPointer, "+");
    }
}

void printProdRules(Grammar *grammar, HashTable *gram_table)
{
    for (int i = 0; i < grammar->nonTerminalsCount; i++)
    {
        char *lhs = grammar->nonTerminals[i];
        Symbol *symb = search4HashTable(gram_table, lhs, strlen(lhs));
        if (symb == NULL)
        {
            fprintf(outputPointer, "Key not found: %s", symb->key);
        }
        else
        {
            fprintf(outputPointer, "LHS = %s\n", symb->key);
            // gram->nonTerminalsCount+= unionSet(gram->nonTerminals,symb->key);
            char ***prodRulez = ((ProductionRuleRHS *)symb->value)->rhs;
            int i = 0;
            while (prodRulez[i] != NULL)
            {
                fprintf(outputPointer, "\tRULE %d : ", i + 1);
                char **curr_arr = prodRulez[i];
                int j = 0;
                while (curr_arr[j] != NULL)
                {
                    fprintf(outputPointer, "%s ", curr_arr[j]);
                    j++;
                }
                fprintf(outputPointer, "\n");
                i++;
            }
        }
    }
}
void TreePrinting(TreeNode *root, TreeNode *parent)
{
    if (root == NULL)
        return;
    // printf("%s\n",root->id);
    // lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol
    char *lexeme = root->id;
    if (root->numChildNodes > 0)
    {
        if (strcmp(root->childNodes[0]->id, "TK_ERROR") == 0)
            return;
        if (lexeme[0] == 'T' && lexeme[1] == 'K' && lexeme[2] == '_')
        {
            if (strcmp(lexeme, "TK_NUM") == 0)
                fprintf(outputPointer, "%20s %10d %15s %15d %30s %20s %30s\n", "----", root->line_number, lexeme, atoi(root->childNodes[0]->id), parent->id, "yes", "----");
            else if (strcmp(lexeme, "TK_RNUM") == 0)
                fprintf(outputPointer, "%20s %10d %15s %15f %30s %20s %30s\n", "----", root->line_number, lexeme, atof(root->childNodes[0]->id), parent->id, "yes", "----");
            else
                fprintf(outputPointer, "%20s %10d %15s %15s %30s %20s %30s\n", root->childNodes[0]->id, root->line_number, lexeme, "----", parent->id, "yes", "----");
            return;
        }
        TreePrinting(root->childNodes[0], root);
    }
    if (parent == NULL)
    {
        // lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol

        fprintf(outputPointer, "%20s %10d %15s %15s %30s %20s %30s\n", "----", root->line_number, "----", "----", "----", "no", lexeme);
    }
    else
    {
        fprintf(outputPointer, "%20s %10d %15s %15s %30s %20s %30s\n", "----", root->line_number, "----", "----", parent->id, "no", lexeme);
    }

    for (int i = 1; i < root->numChildNodes; i++)
    {
        TreePrinting(root->childNodes[i], root);
    }
    
}
void printParseTree(TreeNode *root, TreeNode *parent)
{
        fprintf(outputPointer, "=============================================================== PARSE TREE ========================================================================\n\n");
        fprintf(outputPointer, "%20s%10s%15s%15s%30s%20s%30s\n", "lexeme", "lineno", "tokenName", "valueIfNumber", "parentNodeSymbol", "isLeafNode(yes/no)", "NodeSymbol");
        fprintf(outputPointer, "---------------------------------------------------------------------------------------------------------------------------------------------------\n");
        TreePrinting(root,parent);
        fprintf(outputPointer,"================================================================ END ===============================================================================\n");

    
}

void PrintTree(TreeNode *ParentNode, int depth, int *blanks)
{
    if (ParentNode->childNodes[0] == NULL)
        return;
    blanks[depth] = 0;
    for (int i = 0; i < ParentNode->numChildNodes - 1; i++)
    {
        for (int j = 0; j < depth; j++)
        {
            fprintf(outputPointer, "%s  ", (blanks[j]) ? " " : "│");
        }
        fprintf(outputPointer, "├──Node %d: %s\n", i, ParentNode->childNodes[i]->id);
        PrintTree(ParentNode->childNodes[i], depth + 1, blanks);
    }
    blanks[depth] = 1;
    for (int j = 0; j < depth; j++)
    {
        fprintf(outputPointer, "%s  ", (blanks[j]) ? " " : "│");
    }
    fprintf(outputPointer, "└──Node %d: %s\n", ParentNode->numChildNodes - 1, ParentNode->childNodes[ParentNode->numChildNodes - 1]->id);
    PrintTree(ParentNode->childNodes[ParentNode->numChildNodes - 1], depth + 1, blanks);
}

void PrintTreeRoot(TreeNode *root)
{
    fprintf(outputPointer, "Root : %s\n", root->id);
    int *blanks = (int *)malloc(20 * sizeof(int));
    blanks[0] = 0;
    PrintTree(root, 1, blanks);
}

void removeComments(char *testcaseFile, char *cleanFile)
{
    /* should check that argc > 1 */
    FILE *file = fopen(testcaseFile, "r"); /* should check the result */
    FILE *fp = fopen(cleanFile, "w");
    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        char s[256] = "";
        int flag = 0;
        int i = 0;
        while (line[i] != '\0')
        {
            if (line[i] == '%')
            {
                memcpy(s, line, i);
                s[i] = '\n';
                flag = 1;
                break;
            }
            i++;
        }
        if (flag != 1)
            fprintf(fp, line, strlen(line));
        else
            fprintf(fp, s, strlen(line));
    }
    // fclose(file);
    // fclose(fp);
}

void callParser(List *list,List* errorlist)
{
    if (list == NULL)
    {
        printf("Lexer not called\n");
        return;
    }
    FILE *f = fopen("productions.txt", "r");
    if (f == NULL)
    {
        printf("Error opening productions.txt/ productions.txt missing \n");
        return;
    }

    Grammar *gram = init_grammar();
    HashTable *gramTable = LoadProductionRules(f, gram);
    FirstAndFollow *firstAndFollow = init_FirstAndFollow();
    printf("FIRST and FOLLOW sets computed using functions..\n");

    for (int i = 0; i < gram->nonTerminalsCount; i++)
    {
        char *key = gram->nonTerminals[i];
        char **value = calloc(30, sizeof(char *));
        calculateFirst(gramTable, key, i, strlen(key), value, 0);
        insert2HashTable(firstAndFollow->first, key, strlen(key), value);
    }

    int count = gram->nonTerminalsCount;
    for (int i = 0; i < count; i++)
    {
        char **detectRecursion = calloc(30, sizeof(char *));
        char *key = gram->nonTerminals[i];
        char **value = calloc(30, sizeof(char *));
        calculateFollow(gramTable, key, strlen(key), value, firstAndFollow->first, detectRecursion);
        insert2HashTable(firstAndFollow->follow, key, strlen(key), value);
        free(detectRecursion);

    }
    // printf("Non terminals : %d,terminals: %d \n",gram->nonTerminalsCount,gram->terminalsCount);
    HashTable *parsetable = init_parsetable(gram);
    printf("parsetable initialized\n");
    createParseTable(firstAndFollow, parsetable, gramTable, gram);
    printf("Parse table created\n");
    TreeNode *root = createParseTree(list, parsetable,errorlist);
    printf("Parse tree constructed\n");
    printParseTable(gram,parsetable);
    printParseTree(root, NULL);
    printf("Printed the parse tree to output File\n");
    printf("\n");
    freeParseTable(parsetable,gram->nonTerminalsCount);
    freeParseTree(root);
    freeList(list);
    freeFirstAndFollow(firstAndFollow);
    freeGrammar(gram);
    freeProductionRules(gramTable);
}

List *callLexer(FILE *filePointer,List* errorlist)
{
    fseek(filePointer, 0, SEEK_SET);
    TWIN_BUFFER *tbuff = buf_init();
    allocate_buffer(filePointer, tbuff);
    HashTable *lookup_tbl = initializeLookupTable();
    List *list = createList();
    while ((tbuff->current_ptr->buf == BUFFER_0 && tbuff->current_ptr->ind < tbuff->buff0siz) || (tbuff->current_ptr->buf == BUFFER_1 && tbuff->current_ptr->ind < tbuff->buff1siz))
    {
        Token *tk = getToken(filePointer, tbuff, lookup_tbl);
        if (tk && tk->tkId != TK_COMMENT && tk->tkId != TK_ERROR)
        {
            insertToList(list, tk);
            // printf("%d  %s  %s",tk->lineno,token_str(tk->tkId),tk->lexeme);
        }
        else if (tk &&tk->tkId==TK_ERROR)
        {
            insert2ListInOrder(errorlist,tk);
            //fprintf(outputPointer, "%d  %s  %s\n", tk->lineno, token_str(tk->tkId), tk->lexeme);
        }
        else if (tk && tk->tkId == TK_COMMENT)
        {
           free(tk->lexeme);
        free(tk);
        }
        tbuff->current_ptr->ind = tbuff->forward_ptr->ind;
        tbuff->current_ptr->buf = tbuff->forward_ptr->buf;
    }
    free_buffer(tbuff);
    printList(list, outputPointer);
    freeLookUpTable(lookup_tbl);
    return list;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./driver <input_file> <output_file> \n");
        return 1;
    }
    char *fname = argv[1];
    FILE *filePointer;
    char *fout = argv[2];
    outputPointer = fopen(fout, "w");

    filePointer = fopen(fname, "rb");
    if (filePointer == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
    }

    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;

    int input;
    
    printf("Enter \n1. To Remove Comments \n2. For Lexical Analysis \n3. For Parser \n4. For Total time Taken \n0. To Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &input);
    List *list;
    while (input != 0)
    {   
        List* errorlist=(List*)calloc(1,sizeof(List));
        switch (input)
        {
        case 0:
            printf("\nExiting... \n");
            free(errorlist);
            break;
        case 1:
            printf("\nComment removed file logged to clean.txt\n");
            removeComments(argv[1], "clean.txt");
            free(errorlist);
            break;
        case 2:
            printf("\nStarting Lexical Analysis...\n\n");
            start_time = clock();
            list=callLexer(filePointer,errorlist);
            printErrorList(errorlist);
            freeList(list);
            freeList(errorlist);
            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            break;
        case 3:
            printf("\nStarting Parsing...\n\n");
            start_time = clock();
            list = callLexer(filePointer,errorlist);
            callParser(list,errorlist);
            printErrorList(errorlist);
            freeList(errorlist);
            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            break;
        case 4:
            printf("\nTotal CPU time: %f , Toal CPU time in seconds: %f\n\n", total_CPU_time, total_CPU_time_in_seconds);
            free(errorlist);
            break;
        default:
            printf("\nInvalid choice\n");
            free(errorlist);
            break;
        }
        printf("Enter \n1. To Remove Comments \n2. For Lexical Analysis \n3. For Parser \n4. For Total time Taken \n0. To Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &input);
    }
    
    printf("\nExiting... \n");
    fclose(filePointer);
}
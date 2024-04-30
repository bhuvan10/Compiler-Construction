/*
Aryan Barapatre 2020B4A70833P
Bhuvan Gupta    2020B4A71654P
Dhruv Singh     2020B4A70969P
Pulkit          2020B2A71932P
Jash Naik       2020B3A71958P
Siripuram Surya 2020B2A71406P
*/
#include "lexer.h"
#define LEX_ERROR_STATE 62

TWIN_BUFFER *buf_init()
{
    /*
        Action : Create and Initialize TwinBuffer
        InArgs : None
        OutArgs: TwinBuffer Pointer (TWIN_BUFFER*)
    */
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
    /*
        Action : Free TwinBuffer Structure
        InArgs : TwinBuffer Pointer (TWIN_BUFFER *)
        OutArgs: None
    */
    free(tbuff->BUFFER0);
    free(tbuff->BUFFER1);
    free(tbuff->forward_ptr);
    free(tbuff->current_ptr);
    free(tbuff);
}

void allocate_buffer(FILE *fp, TWIN_BUFFER *buffer)
{   
    /*
        Action : Fill up both the buffers of TwinBuffer with file in FilePointer
        InArgs : File Pointer & TwinBuffer Pointer(FILE *, TWIN_BUFFER *)
        OutArgs: None
    */  
    long int initial_position = ftell(fp);
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
    /*
        Action : Gets Next char for Lexer from forward pointer, Also reallocates the buffers
        InArgs : File Pointer & TwinBuffer Pointer (FILE *, TWIN_BUFFER *)
        OutArgs: None
    */
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
    /*
        Action : Move the forward Pointer back by one step
        InArgs : TwinBuffer Pointer (TWIN_BUFFER *)
        OutArgs: None
    */
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
    /*
        Action : Get the Lexeme by forward_ptr - current_ptr
        InArgs : TwinBuffer Pointer (TWIN_BUFFER *)
        OutArgs: Lexeme (char*)
    */
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
    /*
        Action : Calculate Hash Value of a string
        InArgs : String and size of string (char*, int)
        OutArgs: HashValue (int)
    */
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
    /*
        Action : Initialize HashTable of HASHTABLE_SIZE
        InArgs : None
        OutArgs: HashTable Pointer (HashTable*)
    */
    HashTable* hash_tbl = (HashTable*)malloc(sizeof(HashTable));
    hash_tbl->Table = (Symbol**)calloc(HASHTABLE_SIZE, sizeof(Symbol*));
    hash_tbl->size = 0;
    return hash_tbl;
}

HashTable* insert2HashTable(HashTable* hash_tbl, char* key, int n, void* value){
    /*
        Action : Insert a key-value pair to HashTable
        InArgs : Hashtable, key, size of key, value (HashTable*, char*, int, void*)
        OutArgs: HashTable Pointer (HashTable*)
    */
    
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
    /*
        Action : Search a key in HashTable
        InArgs : Hashtable, key, size of key (HashTable*, char*, int)
        OutArgs: Symbol Pointer (Symbol*)
    */

    int hash = polyHash(key, n);
    Symbol* current_ptr = hash_tbl->Table[hash];
    while (current_ptr){
        if(strcmp(key, current_ptr->key)==0){
            return current_ptr;
        }
        current_ptr = current_ptr->Next;
    }
    return NULL;
}

void freeHashTable(HashTable* hash_tbl){
    /*
        Action : free HashTable
        InArgs : Hashtable (HashTable*)
        OutArgs: None
    */
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
    /*
        Action : create a duplicate malloc of a string
        InArgs : String (char*)
        OutArgs: String (char*)
    */
    char* temp = (char*)malloc(sizeof(char)*(strlen(s)+1));
    strcpy(temp, s);
    return temp;
}

void freeLookUpTable(HashTable* lookup_tbl){
    /*
        Action : Free LookUp Table and its keys
        InArgs : LookUp Table (HashTable*)
        OutArgs: None
    */
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
    /*
        Action : Initialize LookUp Table with some values
        InArgs : None
        OutArgs: LookUp Table (HashTable*)
    */
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
    /*
        Action : Insert to a Token List in sorted Oder of linenumber
        InArgs : Token List & Token to be inserted (List*, Token*)
        OutArgs: None
    */  
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
    /*
        Action : Performs LookUp od a lexeme and return correct TOKEN from lookup table
        InArgs : LookUp Table, lexeme, length of lexeme, state currently in 
        OutArgs: LookUp Table (HashTable*)
    */
    Symbol* symbol = search4HashTable(lookupTable, lexeme, n);
    if (symbol == NULL){
        char *st = (char*)mallocString(lexeme);
        if(state==61) // TK_FIELD State
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
    /*
        Action : Tokenizes and gives a struct with TOKEN_ID and Lexeme
        InArgs : FilePointer, TwinBuffer, LookUp Table (FILE*, TWIN_BUFFER *, HashTable *)
        OutArgs: Tokenized Lexeme (Token*)
    */
    Token *tk = (Token *)malloc(sizeof(Token)); // initialized Token
    tk->lexeme = (char *)calloc(6, sizeof(char)); // initiliazed lexeme of the token
    char * lex; // storage of lexeme for freeing
    int state = 0; //current State
    char c; // char from getNext()
    char s[100]; // storage of current lexeme
    tk->lineno = tbuff->line; // intialize token's linenumber
    while (1)
    {
        switch (state)
        {
        case 0: // Start State
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
            { // Unknown Token Encountered
                state = 100;
                lex =  getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Symbol <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 1: // Final State of TK_SQL
            tk->tkId = TK_SQL;
            // s = "[");
            strcpy(s,"[");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 2: // Final State of TK_SQR
            tk->tkId = TK_SQR;
            strcpy(s,"]");
            strcpy(tk->lexeme, s);

            return tk;
            break;
        case 3: // Final State of TK_COMMA
            tk->tkId = TK_COMMA;
            strcpy(s,",");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 4: // Final State of TK_SEM
            tk->tkId = TK_SEM;
            strcpy(s,";");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 5:// Currently in Lexeme : '>'
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
        case 6: // Final State of TK_GT
            retract(tbuff);
            tk->tkId = TK_GT;
            strcpy(s,">");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 7: // Final State of TK_GE
            tk->tkId = TK_GE;
            strcpy(s,">=");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 8: // Final State of TK_COLON
            tk->tkId = TK_COLON;
            strcpy(s,":");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 9: // Final State of TK_DOT
            tk->tkId = TK_DOT;
            strcpy(s,".");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 10: // Final State of TK_OP
            tk->tkId = TK_OP;
            strcpy(s,"(");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 11: // Final State of TK_CL
            tk->tkId = TK_CL;
            strcpy(s,")");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 12: // Final State of TK_PLUS
            tk->tkId = TK_PLUS;
            strcpy(s,"+");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 13: // Final State of TK_MINUS
            tk->tkId = TK_MINUS;
            strcpy(s,"-");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 14: // Final State of TK_MUL
            tk->tkId = TK_MUL;
            strcpy(s,"*");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 15: // Final State of TK_DIV
            tk->tkId = TK_DIV;
            strcpy(s,"/");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 16: // Final State of TK_NOT
            tk->tkId = TK_NOT;
            strcpy(s,"~");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 17: // Currently in Lexeme : '!'
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
        case 18: // Final State of TK_NE
            tk->tkId = TK_NE;
            strcpy(s,"!=");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 19: // Currently in Lexeme : '&'
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
        case 20: // Currently in Lexeme : '&&'
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

        case 21: // Final State of TK_AND
            tk->tkId = TK_AND;
            strcpy(s,"&&&");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 22: // Currently in Lexeme : '@'
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
        case 23: // Currently in Lexeme : '@@'
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
        case 24: // Final State of TK_OR
            tk->tkId = TK_OR;
            strcpy(s,"@@@");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 25: // Currently in Lexeme : '='
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
        case 26: // Final State of TK_EQ
            tk->tkId = TK_EQ;
            strcpy(s,"==");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 27: // Currently in Lexeme : '%'
            c = getNext(fp, tbuff);
            while (c != '\n')
            {
                c = getNext(fp, tbuff);
            }
            state = 28;
            break;
        case 28: // Final State of TK_COMMENT
            tk->tkId = TK_COMMENT;
            strcpy(s,"%");
            strcpy(tk->lexeme, s);
            return tk;
        case 29: // Currently in Lexeme : '<'
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
        case 30: //Final State of TK_LT
            retract(tbuff);
            tk->tkId = TK_LT;
            strcpy(s,"<");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 31: // Final State of TK_LE
            tk->tkId = TK_LE;
            strcpy(s,"<=");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 32: // Currently in Lexeme : '<-'
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
        case 33: // Final State of TK_LT
            retract(tbuff);
            retract(tbuff);
            tk->tkId = TK_LT;
            strcpy(s,"<");
            strcpy(tk->lexeme, s);
            return tk;
            break;
      case 34: // Currently in Lexeme : '<--'
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
        case 35: // Final State of TK_ASSIGNOP
            tk->tkId = TK_ASSIGNOP;
            strcpy(s,"<---");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 36: // Currently in lexeme '#' 
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
        case 37: // Current lexeme of TK_RUID
            c = getNext(fp, tbuff);
            while (c >= 'a' && c <= 'z')
            {
                c = getNext(fp, tbuff);
            }
            state = 38;
            break;
        case 38: // Final State of TK_RUID
            retract(tbuff);
            lex = getLexeme(tbuff);
           strcpy(s,lex);
           free(lex);
            tk->tkId = TK_RUID;
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 39: // Current Lexeme of TK_NUM, TK_RNUM
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
        case 40: //Final State of TK_NUM
            retract(tbuff);
            lex = getLexeme(tbuff);
            strcpy(s,lex);
            free(lex);
            tk->tkId = TK_NUM;
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 41: // Current Lexeme of TK_RNUM w/o E
            c = getNext(fp, tbuff);
            if (c >= '0' && c <= '9')
            {
                state = 42;
            }
            else
            {
                // state 43, Final State of TK_NUM
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
        case 42: // Current Lexeme of TK_RNUM w/o E
            c = getNext(fp, tbuff);
            if (c >= '0' && c <= '9')
            {
                state = 44;
            }
            else
            {
                // state=43, -> changed to Error State
                state=100;
                retract(tbuff);
                lex = getLexeme(tbuff);
                sprintf(s,"Line %-3d Unknown Pattern <%s>",tbuff->line, lex);
                free(lex);
            }
            break;
        case 44: // Current Lexeme of TK_RNUM w/o E
            c = getNext(fp, tbuff);
            if (c == 'E')
            { // QUESTION Why >= 'E' and Not 'E'
                state = 46;
            }
            else
            {
                state = 45;
            }
            break;
        case 45: // Current Lexeme of TK_RNUM with E
            retract(tbuff);
            lex = getLexeme(tbuff);
           strcpy(s,lex);
           free(lex);
            tk->tkId = TK_RNUM;
            tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 46:// Current Lexeme of TK_RNUM with E
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
        case 47:// Current Lexeme of TK_RNUM with E
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
        case 48:// Current Lexeme of TK_RNUM with E
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

        case 49:// Final State of TK_RNUM
            lex = getLexeme(tbuff);
           strcpy(s,lex);
           free(lex);
           tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
            tk->tkId = TK_RNUM;
            strcpy(tk->lexeme, s);
            return tk;
            break;

        case 50://Currently in Lexeme '_'
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
        case 51://Currently Lexeme of TK_MAIN or TK_FUNID
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
        case 52://Currently Lexeme of TK_FUNID
            c = getNext(fp, tbuff);
            while (c >= '0' && c <= '9')
                c = getNext(fp, tbuff);
            state = 53;
            break;
        case 53:// Final State of TK_FUNID and TK_MAIN
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
        case 54: // WHITESPACE, TAB, NEWLINE REMOVAL
            c = getNext(fp, tbuff);
            while (c == ' ' || c == '\t' || c == '\n')
                c = getNext(fp, tbuff);
            retract(tbuff);
            free(tk->lexeme);
            free(tk);
            return NULL;
            break;
        case 55: // Retract and return TK_COMMENT
            retract(tbuff);
            tk->tkId = TK_COMMENT;
            strcpy(s,"%");
            strcpy(tk->lexeme, s);
            return tk;
            break;
        case 56: // Currently a Lexeme of a Keyword or TK_ID or TK_FIELDID
            c = getNext(fp, tbuff);
            if (c >= '2' && c <= '7')
                state = 57;
            else if (c >= 'a' && c <= 'z')
                state = 60;
            else
                state = 61;
            break;
        case 57: // Currently a Lexeme of TK_ID
            c = getNext(fp, tbuff);
            while (c >= 'b' && c <= 'd')
                c = getNext(fp, tbuff);
            if (c >= '2' && c <= '7')
                state = 58;
            else
                state = 59;
            break;
        case 58: // Currently a Lexeme of TK_ID
            c = getNext(fp, tbuff);
            while (c >= '2' && c <= '7')
                c = getNext(fp, tbuff);
            state = 59;
            break;
        case 59: // Final State of of TK_ID
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
        case 60: // Currently a lexeme of a Keyword or TK_FIELDID
            c = getNext(fp, tbuff);
            while (c >= 'a' && c <= 'z')
                c = getNext(fp, tbuff);
            state = 61;
            break;
        case 61: // Final State of a Keyword or TK_FIELDID
            retract(tbuff);
            lex = getLexeme(tbuff);
           strcpy(s,lex);
           free(lex);
            tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
            strcpy(tk->lexeme, s);
            tk->tkId = lookUp(lookup_tbl, s, strlen(s), state);
            return tk;
            break;
        default: // Final State of TK_ERROR / Lexical Error Encountered
            tk->tkId = TK_ERROR;
            tk->lexeme = realloc(tk->lexeme, strlen(s) + 1);
            strcpy(tk->lexeme,s);
            return tk;
        }
    }
}

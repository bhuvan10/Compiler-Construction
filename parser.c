/*
Aryan Barapatre 2020B4A70833P
Bhuvan Gupta    2020B4A71654P
Dhruv Singh     2020B4A70969P
Pulkit          2020B2A71932P
Jash Naik       2020B3A71958P
Siripuram Surya 2020B2A71406P
*/

#include "lexer.h"
#include "parserDef.h"

Stack* createStack() {
    /*
        Action : Creates a stacks
        InArgs : None
        OutArgs: Returns a pointer to the stack (Stack*)
    */
    Stack* stack = (Stack*)calloc(1, sizeof(Stack));
    stack->top = NULL;
    stack->tail = NULL;
    return stack;
}

void push(Stack* stack,  char* item) { 
    /*
        Action : Pushes Item Onto the stack
        InArgs : Stack and String item (Stack*, char*)
        OutArgs: None
    */
    StackNode* newNode = (StackNode*)calloc(1, sizeof(StackNode));
    newNode->data = item; 
    newNode->next = stack->top;
    stack->top = newNode;
    if (!stack->tail) {
        stack->tail = newNode;
    }
}


void pushArray(Stack*stack, char** itemArray, int n){
    /*
        Action : Pushes Array of strings onto the stack in decending order (i.e A[0] is on top and A[n-1] is at bottom)
        InArgs : Stack, Array of String, length of Array  (Stack*, char*)
        OutArgs: None
    */
    if(itemArray[n] == NULL)
        return;
    else{
        pushArray(stack, itemArray, n+1);
        push(stack, itemArray[n]);
    }

}

char* pop(Stack* stack) {
    /*
        Action : Pops the top of the stack returning the top element
        InArgs : Stack (Stack*)
        OutArgs: Returns previous top of stack (char*)
    */
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
    /*
        Action : Checks if Stack is empty
        InArgs : Stack (Stack*)
        OutArgs: Returns 0 if false, else True (int)
    */
    return stack->top == NULL;
}

int length(Stack* stack) {
    /*
        Action : Returns number of elements on Stack
        InArgs : Stack (Stack*)
        OutArgs: Integer
    */
    int count = 0;
    StackNode* current = stack->top;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void printStack(Stack* stack){
    /*
        Action : Prints Top of the stack in <position> : <element> format
        InArgs : Stack (Stack*)
        OutArgs: Stdout printf
    */
    StackNode* snode =stack->top;
    for (int i=0; snode!= NULL; i++){
        printf("%d: %s\n",i, snode->data);
        snode = snode->next;
    }
}

//linkedlist.c added
List* createList(){
    /*
        Action : Creates new List of *Tokens*
        InArgs : None
        OutArgs: List Pointer (List*)
    */
    List* list=(List*)calloc(1,sizeof(List));
    list->head=NULL;
    list->tail=NULL;
    return list;
}

ListNode* insertToList(List* list,Token* token){
    /*
        Action : Inserts a Token To the Back of the List using Tail
        InArgs : List and Token to be inserted (List*, Token*)
        OutArgs: List's Head Pointer (ListNode*)
    */
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
    /*
        Action : Prints the list of tokens onto the FilePointer
        InArgs : List and File Pointer (List*, FILE*)
        OutArgs: Prints List onto file
    */
    ListNode* temp=list->head;
    while(temp!=NULL){
        Token* tk=temp->mytoken;
        // printf("%s",outputPointer);
        fprintf(outputPointer,"Line no. %-3d Lexeme: %-30s Token: %s \n",tk->lineno,tk->lexeme,token_str(tk->tkId));
        temp=temp->next;
    }
}
void printErrorList(List* list){
    /*
        Action : Prints the error list of tokens onto stdout
        InArgs : List (List*)
        OutArgs: Prints List on STDOUT
    */
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
    /*
        Action : Adds string b onto the list of string a, outputs 1 if a doesn't contain b else 0
        InArgs : List of String and String (char **, char*)
        OutArgs: 0 if a contains b, 1 if a doesn't contain b
    */
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
    /*
        Action : Adds list of string b onto the list of string a only if b[i] is not TK_EPSILON
        InArgs : List of String and List of String (char **, char**)
        OutArgs: None
    */
    int i=0;
    while(b[i] != NULL){
        if(strcmp(b[i],"TK_EPSILON")!=0)unionSet(a, b[i]);//skip epsilon
        i++;
    }
}


char* token_str(TOKEN_ID id) {
    /*
        Action : Returns the String representing the enum TOKEN_ID
        InArgs : Token in enum format (TOKEN_ID)
        OutArgs: String of Token *NOT LEXEME OF TOKEN, just enum in string format*
    */
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
    /*
        Action : 
            i) Gets the rhs of Production Rule in line
            ii) Updates Non-Terminals and Terminals in Grammar 
        InArgs : Buffer String, Grammer Structure (char *, Grammar*)
        OutArgs: List of String represents an RHS of a production rule (char**)
    */
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

int calculateFirst(HashTable* gramTable, char* key,int ind, int n, char ** firstSet,int recDepth){
    /*
        Action : Calculates First Set of key by recursion and stores it in firstSet Array
        InArgs : 
            a) HashTable of all Production Rules with KEY as LHS (char*) and VALUE as List of RHS (char***) (HashTable*)
            b) Non-Terminal for which to calculate the First Set (char *)
            c) Index of the Non-Terminal in Grammer for which the function is first called *USE UNKNOWN* (int)
            d) Length of the Non-Terminal (int)
            e) Array of String in which to store the First Set, corresponding to First Set of ind (char**)
            f) Depth of the Recursion (int)
        OutArgs: integer for control in calculating first of *key* (int)
    */    
    int ret=0;
    Symbol* symb= search4HashTable(gramTable, key, n);
    if(symb == NULL){
        printf("KEY NOT FOUND for calc First : %s", key );
        return 0;
    }else{
        char*** prodRulez = ((ProductionRuleRHS*)symb->value)->rhs; // List of RHS
        int i =0; // index for a particular RHS in prodRulez
        while(prodRulez[i] != NULL){
            char** curr_arr = prodRulez[i]; // A particular RHS
            int j=0; // index for a particular Terminal/Non-Terminal in an RHS
            if(strcmp(curr_arr[0], "TK_EPSILON") == 0){       
                    if(recDepth==0){ // insert TK_EPSILON, only if called first
                        unionSet(firstSet,  curr_arr[0]);
                    }
                    ret= 1; // indicates to parent call to increment j and search first for next Terminal/Non-Terminal in Production Rule
            }
             else if( curr_arr[j][0] == 'T' && curr_arr[j][1] == 'K' && curr_arr[j][2] == '_' ){ // Simply Add if Terminal
                unionSet(firstSet,  curr_arr[j]);
            }
            else { // Recursively Call 
                int next =1; // initialized to one to enter the while loop
                while(curr_arr[j] != NULL && next == 1){
                    next = calculateFirst(gramTable, curr_arr[j],ind, strlen(curr_arr[j]), firstSet, recDepth+1);
                    if(next == 1){ // if 1, epsilon present in first of curr_arr[j], hence increment j
                        j++;
                            if( curr_arr[j][0] == 'T' && curr_arr[j][1] == 'K' && curr_arr[j][2] == '_' ){ // if terminal, add to first set and stop calculation of first for this particular Non-Terminal
                                unionSet(firstSet, curr_arr[j]);
                                break;//return 0; // QUESTION : Why not break from while loop ?, else other prod rules of the non-terminals will not have first calculated Example: S->AaBC;S->RST; A->Epsilon; R->b
                            }
                    }
                    else{
                        break;  
                    }
                }
            }
            i++; // increment i for next RHS of prodRulez
        }
    }
    return ret;
}

void calculateFollow(HashTable* gramTable, char* key, int n, char ** followSet,HashTable* firstset, char ** detectRecursion){ 
    /*
        Action : Calculates Follow Set of key by recursion and stores it in followSet Array
        InArgs : 
            a) HashTable of all Production Rules with KEY as LHS (char*) and VALUE as List of RHS (ProductionRuleRHS*) (HashTable*)
            b) Non-Terminal for which to calculate the Follow Set (char *)
            c) length of the Non-Terminal in Grammer for which the function is first called *USE UNKNOWN* (int)
            d) Array of String in which to store the Follow Set (char**)
            e) HashTable of all First Sets with KEY as Non-Terminal (char*) and VALUE as List of Terminals (char**) (HashTable*)
            f) Array of Non-Terminals for which Follow has been calculated for (used to detect recursion) (int)
        OutArgs: None
    */  
    if(strcmp(key,"<program>")==0) // if Non-Terminal == <program>, Add only TK_DOLLAR to the followset
    {
        followSet[0]="TK_DOLLAR";
        return;
    }
    // printf("Calculating Follow for: %s\n", key);
    if(unionSet(detectRecursion, key)==0) // if recursion detected, terminate call
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
                    /*
                    Above code searches for Non-Terminal *key* in all Production Rules,
                        k gives the index of Non terminal *key*,
                        curr_arr gives the RHS as list of string, 
                        curr_arr[m] gives Terminal/Non-Terminal at index m of the Production Rule
                        symb->key gives LHS
                    */
                    if(keyPresent==1)
                    {
                        k++; // k now represents index next to Non-Terminal *key*
                        if(curr_arr[k]==NULL){ // Non-Terminal *key* Spotted at the end of the Production Rule, Add Follow of LHS to the Follow of *key*
                                    if (strcmp(key, symb->key)!=0) // To Avoid Direct Recursion
                                        calculateFollow(gramTable, symb->key, strlen(symb->key), followSet,firstset,detectRecursion); 
                        }
                        else if(curr_arr[k][0] == 'T' && curr_arr[k][1] == 'K' && curr_arr[k][2] == '_' ){ // Add Terminal Directly to follow set
                                        unionSet(followSet, curr_arr[k]);
                        }
                        else{
                            int flag=0;// 1 if TK_EPSILON in First Set of non-terminals next to non-terminal *key*
                            // NOT REQUIRED TO INIALIZE, curr_arr[k] != NULL, hence will always update value of flag to zero 
                            while(curr_arr[k] != NULL){
                                flag=0;
                                char* key = curr_arr[k]; // key now represents curr_arr[k]
                                if(curr_arr[k][0] == 'T' && curr_arr[k][1] == 'K' && curr_arr[k][2] == '_' ){ // Add Terminal to follow set and break
                                        unionSet(followSet, curr_arr[k]);
                                        break;
                                }
                                Symbol* symb= search4HashTable(firstset, key, strlen(key)); 
                                if(symb == NULL){
                                    printf("Key not found: %s", symb->key );
                                }else{
                                    char**  set2 = (char**)symb->value; // set2 = First set of Current Non-Terminal
                                    unionSet2(followSet, set2);
                                    int x=0;
                                    while(set2[x] != NULL)
                                    {
                                        if(strcmp(set2[x],"TK_EPSILON")==0) // if First Set contain TK_EPSILON, repeat process for the next non-terminal
                                            {
                                            flag=1;
                                            break;
                                            }
                                        x++;
                                    }
                                    if(flag!=1)
                                    { // if no TK_EPSILON in first of current Non-Terminal, then terminate the while loop 
                                        break;
                                    } 
                                }  
                                k++;
                            }
                            if(flag==1){ // If all Non-Terminals has epsilon in first, 
                                calculateFollow(gramTable, symb->key, strlen(symb->key), followSet,firstset,detectRecursion); 
                            }
                        }
                    }
                }
                symb = symb->Next; // Go to next entry for the hashvalue
            }
        }
    }
}

Grammar* init_grammar()
{
    /*
        Action : Returns initialized Grammar that contains all Terminals and NonTerminals
        InArgs : None
        OutArgs: Returns Grammar*
    */
    Grammar* gram=(Grammar *)malloc(sizeof(Grammar));
    gram->nonTerminals=calloc(70,sizeof(char*));
    gram->terminals=calloc(70,sizeof(char*));
    gram->terminalsCount=0;
    gram->nonTerminalsCount=0;
    return gram;
}
FirstAndFollow* init_FirstAndFollow()
{
    /*
        Action : Returns initialized FirstAndFollow that contains first and follow sets as HashTables
        InArgs : None
        OutArgs: Returns FirstAndFollow*
    */
    FirstAndFollow* firstAndFollow=(FirstAndFollow *)malloc(sizeof(FirstAndFollow));
    firstAndFollow->first=initializeHashTable();
    firstAndFollow->follow=initializeHashTable();
    return firstAndFollow;
}
HashTable* LoadProductionRules(FILE* f,Grammar* gram)
{
    /*
        Action : Returns a HashTable that has KEY as LHS (char*) and VALUE as List of RHS (ProductionRuleRHS*) (HashTable*)
        InArgs : File Pointer from where to read production rules, Grammar which contains Terminals and Non-Terminals
        OutArgs: Returns HashTable of Production Rules
    */
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
    /*
        Action : Converts Token in String Format to TOKEN_ID
        InArgs : String (const char * str)
        OutArgs: TOKEN_ID corrensponding to input String (NOT LEXEME, ONLY 'TK_' Format String)
    */
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
    /*
        Action : Initialize ParseTable in a form of Hashtable
        InArgs : Grammar that contains list of Non-Terminals and Terminals (Grammar*)
        OutArgs: HashTable with KEYS as nonTerminal and VALUE as Production Rules for each Terminal
    */
    HashTable* parsetable = initializeHashTable();
    for (int i=0; i<grammer->nonTerminalsCount; i++){
        char * nonTerminal= grammer->nonTerminals[i];
        char*** TerminalRow = calloc(grammer->terminalsCount+2, sizeof(char**));
        insert2HashTable(parsetable, nonTerminal, strlen(nonTerminal), TerminalRow);     
    }return parsetable;
}

void createParseTable(FirstAndFollow* fnf, HashTable* parsetable, HashTable* gram_table, Grammar* grammar, char**synTokenArr){
    /*
        Action : Fill up the ParseTable, Put each Production rule of a Non-Terminal to its correnponding Terminal
        InArgs : 
            a) First And Follow Sets (FirstAndFollow*)
            b) Already Initialized ParseTable (HashTable*)
            c) HashTable containing KEYS as LHS (char*) and VALUES as List of RHS (ProductionRuleRHS*)
            d) Grammar that contains list of Non-Terminals and Terminals (Grammar*)
        OutArgs: None
    */
    
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
            // For each Non-Terminal, get a list of production rules, now for each production rule 'productionRules[j]' 
            // get first(productionRules[j]), Fill up each first with production rule 'productionRules[j]'
            int i=0;
            while(productionRules[j][i]!=NULL)
            {
                int flag2=0; 
                char* s=productionRules[j][i]; // Current Terminal/Non-Terminal in ProductionRule
                if(strcmp(s,"TK_EPSILON")==0)
                { // if the Production Rule == TK_EPSILON, 
                  //For every Terminal in Follow Set, Set its ParseTable Entry to TK_EPSILON 
                    int k=0;
                    while(followSet[k]!=NULL)
                    {
                        ((char***)parseSymbol->value)[getEnum(followSet[k])]=productionRules[j];
                        k++;
                    }
                    flag=1; //Set Flag so that Follow Set are not entered with TK_SYN
                }
                else if (strlen(s)>=2&&(s[0]=='T' && s[1] == 'K' && s[2]=='_'))
                { // if Terminal, Set its ParseTable Entry to the current Production Rule
                        ((char***)parseSymbol->value)[getEnum(s)]=productionRules[j];
                }
                else
                { // if Non-Terminal, Find its First Set and for each first, Set its ParseTable Entry to the current Production Rule *s*
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
                if(flag2!=1) // if First of *s* doesn't have epsilon, break the while loop
                break;
                i++;
            }
        }
        if(flag!=1)
        {
            int k=0;
            while(followSet[k]!=NULL)
                { // Update the follow of a rule with TK_SYN
                    if(((char***)parseSymbol->value)[getEnum(followSet[k])]==NULL)
                        ((char***)parseSymbol->value)[getEnum(followSet[k])]=synTokenArr;
                    k++;
                }
        }
    }   
}
void freeParseTable(HashTable* parsetable,int n){
    /*
        Action : Free ParseTable
        InArgs : ParseTable and UNKNOWN (HashTable*, int)
        OutArgs: None
    */
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
    /*
        Action : Free Token List Structure and its Tokens and lexeme
        InArgs : List of Tokens (List*)
        OutArgs: None
    */
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
    /*
        Action : Free FirstAndFollow Structure and both the HashTable of First and Follow Sets
        InArgs : FirstAndFollow Structure (FirstAndFollow*)
        OutArgs: None
    */
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
    /*
        Action : Free Grammar Structure and its List of Non-Terminals and Terminals
        InArgs : Grammar Structure (Grammar*)
        OutArgs: None
    */
    free(gram->nonTerminals);
    free(gram->terminals);
    free(gram);
}

void freeProductionRules(HashTable* table){
    /*
        Action : Free Production Rule  Structure and each of its grammer rule
        InArgs : Production Rule Table (HashTable*)
        OutArgs: None
    */
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
    /*
        Action : Free ParseTree Structure
        InArgs : Root Node (TreeNode*)
        OutArgs: None
    */
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
    /*
        Action : Free Stack Structure
        InArgs : Root Node (TreeNode*)
        OutArgs: None
    */
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
    /*
        Action : Checks if the token in a StopWord
        InArgs : Token (TOKEN_ID)
        OutArgs: 0 if false, otherwise true
    */
    return (tkId == TK_END || tkId == TK_ENDIF || tkId == TK_ENDRECORD || tkId == TK_ENDUNION || tkId == TK_ENDWHILE || tkId == TK_SEM || tkId == TK_THEN || tkId ==TK_ELSE);
}

TreeNode *createTreeNode(char *id)
{   
    /*
        Action : Creates a TreeNode with a specific id
        InArgs : id to put in TreeNode
        OutArgs: TreeNode pointer (TreeNode*)
    */
    TreeNode *node = (TreeNode *)calloc(1,sizeof(TreeNode));
    node->id = id;
    node->childNodes = (TreeNode **)calloc(10, sizeof(TreeNode *));
    node->numChildNodes = 0;
    return node;
}

int allocateParseTree(List *ll, HashTable *parseTable, TreeNode *currentNode, Stack *st,List *errorlist)
{
    /*
        Action : Create a parsetree from the List of tokens tokenized by lexer
        InArgs : 
            a) List of Tokens by Lexer (List*)
            b) Parse Table (HashTable*)
            c) The Current being either populated with child Node or turning to leaf (if Terminal)
            d) Stack in Use (Stack*)
            e) List of TK_ERROR Tokens with lexeme as Error Message
        OutArgs: Panic Signal, if 1 Error spotted in code, -1 Stack not empty but no Tokens Left, 0 for no error
    */

    if ((isEmpty(st)||(strcmp(st->top->data,"TK_DOLLAR")==0))&&ll!=NULL&&ll->current!=NULL) // If stack empty or TK_DOLLAR on Top
        {
            char* buf = calloc(120, sizeof(char));
        sprintf(buf, "Line %-3d The stack is empty but List is not with lexeme %s",ll->current->mytoken->lineno,ll->current->mytoken->lexeme);

            //Initialize a Error Token & insert in error list
        Token* errortoken=(Token*)malloc(sizeof(Token));
        errortoken->lineno=ll->current->mytoken->lineno;
        errortoken->tkId=TK_ERROR;
        errortoken->lexeme=buf;
        insert2ListInOrder(errorlist,errortoken);
        return -1;
        }
    else if ((ll == NULL||ll->current==NULL) && !(isEmpty(st)||(strcmp(st->top->data,"TK_DOLLAR")==0))) // If list Empty but Stack Not Empty
    {
        char* buf = calloc(120, sizeof(char));
        sprintf(buf, "Line %-3d Error: The List is Empty but Stack is Not Empty with Top %s",ll->tail->mytoken->lineno, pop(st));
            //Initialize a Error Token & insert in error list
        Token* errortoken=(Token*)malloc(sizeof(Token));
        errortoken->lineno=ll->tail->mytoken->lineno;
        errortoken->tkId=TK_ERROR;
        errortoken->lexeme=buf;
        insert2ListInOrder(errorlist,errortoken);
        return -1;
    }
    if(isEmpty(st)||strcmp(st->top->data,"TK_DOLLAR")==0)
        return 0;
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
    {// If currentTop is Terminal
        
        if(strcmp(currentTop, token_str(currentId)) != 0){
            //Send a panic signal '1' to Parent Non-terminal node for error recovery
            // For an error, terminal contains first child as "SYN_ERROR" and second child as the lexeme causing error
            char* buf = calloc(120, sizeof(char));
            sprintf(buf, "Line %-3d Error: The token %s for lexeme %s  does not match with the expected token %s",tk->lineno, token_str(tk->tkId), tk->lexeme, currentTop);

            //Initialize a Error Token & insert in error list
            Token* errortoken=(Token*)malloc(sizeof(Token));
            errortoken->lineno=tk->lineno;
            errortoken->tkId=TK_ERROR;
            errortoken->lexeme=buf;
            insert2ListInOrder(errorlist,errortoken);

            //Set Child of current Token as TK_ERROR
            currentNode->childNodes[0] = createTreeNode("TK_ERROR");
            currentNode->numChildNodes = 1;
            if(!isStopWord(tk->tkId)) // If Stop Word, don't increment the current pointer 
                ll->current = ll->current->next;
            return 1;
        }
        //If Token matches, create child node as the lexeme
        currentNode->childNodes[currentNode->numChildNodes] = createTreeNode(tk->lexeme);
        currentNode->numChildNodes++;
        ll->current = ll->current->next;
    }
    else
    {// If Current Top is Non-Terminal
        Symbol *symb = search4HashTable(parseTable, currentTop, strlen(currentTop));
        char **ProductionRule = ((char ***)symb->value)[currentId]; 
        if (ProductionRule == NULL)
        {// TK_ERROR encounter in parseTable, Enter Panic Mode    
            while (ll->num != 0)
            { // till ll has tokens, keep skipping them until a stopword, or TK_EPSILON or TK_SYN encountered
                Token *t = ll->current->mytoken; //current token
                char **a = ((char ***)symb->value)[t->tkId]; //current Grammer Rule to be inserted in Tree
                if (isStopWord(t->tkId) || (a!= NULL && ((strcmp(a[0], "TK_EPSILON") == 0) || strcmp(a[0], "TK_SYN") == 0) ))
                { // stop skiping
                    break;
                }
                ll->current = ll->current->next;
            }
            if(ll->current!=NULL)
            { // if ll still has tokens after skipping, continue forward with tokenization

            // Format Error Message ---> Line 13 Error: Invalid token TK_OP encountered with value ( stack top logicalOp
            char* buf = (char*)malloc(sizeof(char)*120);
            Token* t=ll->current->mytoken;
            sprintf(buf,"Line %-3d Error: Invalid token %s encountered with value %s stack top %s",t->lineno,token_str(t->tkId),t->lexeme,currentTop);
            
            //Initialize a Error Token & insert in error list
            Token* errortoken=(Token*)malloc(sizeof(Token));
            errortoken->lineno=tk->lineno;
            errortoken->tkId=TK_ERROR;
            errortoken->lexeme=buf;
            insert2ListInOrder(errorlist,errortoken);

            //Populate TK_ERROR as a child of CurrentNode
            currentNode->childNodes[0] = createTreeNode("TK_ERROR");
            currentNode->numChildNodes++;

            }
        }
        else if (strcmp(ProductionRule[0], "TK_SYN") != 0)
        {// If Any other Terminal encouneter other than TK_SYN, Continue without error
            pushArray(st, ProductionRule, 0);
            int error_skip = 0; // var to initiate skipping if error encountered in child nodes.
            for (int i = 0; ProductionRule[i] != NULL; i++)
            {// Load Prouduction Rule from ParseTree as ChildNodes
                currentNode->childNodes[i] = createTreeNode(ProductionRule[i]);
                currentNode->numChildNodes++;

                // if error in previous nodes, begin error skipping unless Terminal is a stopword
                if(error_skip ==1 && !isStopWord(getEnum(ProductionRule[i]))){
                    // create Error Node and Pop the Stack
                    TreeNode* child = currentNode->childNodes[i];
                    child->childNodes[0] = createTreeNode("TK_ERROR");
                    child->numChildNodes=1;
                    pop(st);
                    continue;
                }
                int detect_error = allocateParseTree(ll, parseTable, currentNode->childNodes[i], st,errorlist);
                if (detect_error == 1){ //Enter panic mode, keep skipping until Stopword Encounter, TK_SYN Encounter, current is not null
                    error_skip = 1; // tell ahead pointers to skip
                    int curr_line_no = ll->current->mytoken->lineno;
                    TOKEN_ID currentTokenId = ll->current->mytoken->tkId;
                    Symbol* sy=search4HashTable(parseTable, currentTop, strlen(currentTop));
                    char** prodRule = ((char***)(sy->value))[currentTokenId];
                    while (ll->current != NULL && ll->current->mytoken->lineno == curr_line_no && !isStopWord(ll->current->mytoken->tkId)
                                        &&( prodRule == NULL || (strcmp(prodRule[0], "TK_SYN") != 0) ) ){
                        ll->current = ll->current->next;//deleteListNode(ll); 
                        prodRule = ((char***)(sy->value))[ll->current->mytoken->tkId]; // change ProdRule until TK_ERROR of TK_SYN encountered
                    }
                }
            }
        }
        else{ // If TK_SYN Encounter, enter panic mode without any skipping
            if(ll->current!=NULL)
            { // if tokens left to tokenize
            char* buf = (char*)malloc(sizeof(char)*128);
            Token* t=ll->current->mytoken;
            sprintf(buf,"Line %-3d Error: Invalid token %s encountered with value %s stack top %s",t->lineno,token_str(t->tkId),t->lexeme,currentTop);
            
            //Initialize a Error Token & insert in error list
            Token* errortoken=(Token*)malloc(sizeof(Token));
            errortoken->lineno=t->lineno;
            errortoken->tkId=TK_ERROR;
            errortoken->lexeme=buf;
            insert2ListInOrder(errorlist,errortoken);

            //Populate TK_ERROR as a child of currentNode
            currentNode->childNodes[0] = createTreeNode("TK_ERROR");
            currentNode->numChildNodes++;
            }
        }
    }
    return 0;
}

TreeNode *createParseTree(List *ll, HashTable *parseTable,List* errorlist)
{
    /*
        Action : Initialize creating a ParseTrees Root and call Allocate ParseTree
        InArgs : Token List, Parse Table, Error Token List (List*, HashTable*, List*)
        OutArgs: Root pointer (TreeNode*)
    */
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
    freeStack(st);
    return startNode;
}
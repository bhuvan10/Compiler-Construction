/*
Aryan Barapatre 2020B4A70833P
Bhuvan Gupta    2020B4A71654P
Dhruv Singh     2020B4A70969P
Pulkit          2020B2A71932P
Jash Naik       2020B3A71958P
Siripuram Surya 2020B2A71406P
*/
#ifndef PARSER_H
#define PARSER_H

    #define ROW_SIZE 256 

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



#endif
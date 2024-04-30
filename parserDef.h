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

    #include "parser.h"

    TreeNode* createTreeNode(char* id);
    void FreeParseTree(TreeNode* Root);
    TreeNode *createParseTree(List *ll, HashTable *parseTable,List* errorlist);
    void printErrorList(List* list);
    HashTable *init_parsetable(Grammar *grammer);
    void createParseTable(FirstAndFollow *fnf, HashTable *parsetable, HashTable *gram_table, Grammar *grammar, char** synTokenArr);
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
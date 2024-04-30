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
#include "lexer.h"
#include "parserDef.h"
#include <time.h>
FILE *outputPointer = NULL;
int max(int a, int b)
{
    return (a > b) ? a : b;
}

void prettyPrintTable(char ***table, int maxColSize[], int n, int m);

void printParseTable(Grammar *gram, HashTable *parsetable)
{
    /*
        Action : Prints the ParseTable
        InArgs : Grammar, ParseTable  (Grammar *, HashTable *)
        OutArgs: STDOUT ParseTable
    */
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
    /*
        Action : Pretty Prints a Table
        InArgs : 2D Table of Strings, Maximum size of each column, Number of rows, Number of columns  (char *** , int *, int, int)
        OutArgs: Pretty Print Table in the output File
    */
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
    /*
        Action : Prints Production Rules
        InArgs : Grammer, List of Production Rules  (Grammer*, HashTable*)
        OutArgs: Prints the Production Rules in the file
    */
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
    /*
        Action : Prints Parse Tree in Tabular Format
        InArgs : Root and the Parent  (TreeNode * , TreeNode *)
        OutArgs: Print Tree onto File
    */
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
    /*
        Action : Prints Parse Tree in Tabular Format
        InArgs : Root and the Parent  (TreeNode * , TreeNode *)
        OutArgs: Print Tree onto File
    */
        fprintf(outputPointer, "=============================================================== PARSE TREE ========================================================================\n\n");
        fprintf(outputPointer, "%20s%10s%15s%15s%30s%20s%30s\n", "lexeme", "lineno", "tokenName", "valueIfNumber", "parentNodeSymbol", "isLeafNode(yes/no)", "NodeSymbol");
        fprintf(outputPointer, "---------------------------------------------------------------------------------------------------------------------------------------------------\n");
        TreePrinting(root,parent);
        fprintf(outputPointer,"================================================================ END ===============================================================================\n");

    
}

void PrintTree(TreeNode *ParentNode, int depth, int *blanks)
{
    /*
        Action : Pretty Prints Parse Tree in inorder traversal
        InArgs : ParentNode, depth of tree, bool values of whether to blank or print line  (TreeNode * , int, int *)
        OutArgs: Pretty Prints Tree onto File
    */
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
    /*
        Action : Pretty Prints Parse Tree in inorder traversal
        InArgs : Root (TreeNode *)
        OutArgs: Pretty Prints Tree onto File
    */
    fprintf(outputPointer, "Root : %s\n", root->id);
    int *blanks = (int *)malloc(20 * sizeof(int));
    blanks[0] = 0;
    PrintTree(root, 1, blanks);
}

void removeComments(char *testcaseFile, char *cleanFile)
{
    /*
        Action : Removes Comments from a file
        InArgs : filename of the testcase file , filename of clean file (TreeNode * , int, int *)
        OutArgs: Removes comments from testcaseFile and puts it in cleanfile
    */

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
    /*
        Action : Calls Parser and parser the current file
        InArgs : Tokenized List from the Lexer, Error List of Tokens (List * , List *)
        OutArgs: Performs all necessary function for creating a Parse Tree
    */
    if (list == NULL)
    {
        printf("Lexer not called\n");
        return;
    }
    FILE *f = fopen("grammar.txt", "r");
    if (f == NULL)
    {
        printf("Error opening grammar.txt/ grammar.txt missing \n");
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
    char** synTokenArr = calloc(1, sizeof(char*));
    synTokenArr[0] = "TK_SYN";
    createParseTable(firstAndFollow, parsetable, gramTable, gram, synTokenArr);
    printf("Parse table created\n");
    TreeNode *root = createParseTree(list, parsetable,errorlist);
    printf("Parse tree constructed\n");
    // printParseTable(gram,parsetable);
    printParseTree(root, NULL);
    printf("Printed the parse tree to output File\n");
    printf("\n");
    freeParseTable(parsetable,gram->nonTerminalsCount);
    freeParseTree(root);
    freeList(list);
    freeFirstAndFollow(firstAndFollow);
    freeGrammar(gram);
    freeProductionRules(gramTable);
    free(synTokenArr);
}

List *callLexer(FILE *filePointer,List* errorlist)
{
    /*
        Action : Calls Lexer and Tokinizes the file
        InArgs : File Pointer, Error List of Tokens (List * , List *)
        OutArgs: Performs all necessary function for Tokenizing the file
    */
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
    /*
        Action : Drives the parser and lexer
        InArgs : Number of arguments, Filename provided at terminal (int, char*[])
        OutArgs: Performs task as instructed by user, outputs error in STDOUT,  clean file in clean.txt, and lexer and parser outputs on file 
    */
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
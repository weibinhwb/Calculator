#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define OVERSTACK -1
#define OK 1
#define ERROR 0
#define MAX 200
typedef int ElemType;
typedef int SElemp;
typedef int Status;
//binaryTree struck
typedef struct TreeNode {
    struct TreeNode *leftChild;
    struct TreeNode *rightChild;
    int isNum;
    ElemType data;
}TreeNode, *BiTree;

//stack struct
typedef struct StackNode {
    struct StackNode *next;
    SElemp data;
} StackNode, *pStack;
typedef struct {   
    pStack top;
} Stack;

//input
typedef struct {
    char data[MAX];
    char *num;
    char *opera;
    int numIndex;
    int operaIndex;
} String;

//stack
Status createStack(Stack *s);
Status push(Stack *s, SElemp e);
Status pop(Stack *s, SElemp *e);
Status travelStack(Stack *s);
//binaryTree
void createTree (BiTree *T, String *string);
int isNumbers(ElemType input);
int priority(int currentNode, int input);
void travel(BiTree T);
//calc 
int operator(int a, char opera, int b);
void calc(BiTree T, Stack *s);
//string
int nextInt(String *s);
char nextChar(String *s);
void createString(String *string);

//calc
void calculator();

//stack
Status createStack(Stack *s) {
    s->top = (pStack) malloc(sizeof(StackNode));
    if (!s->top) return ERROR;
    s->top->next = NULL;
    return OK;
}

Status push(Stack *s, SElemp e) {
    pStack node = (pStack) malloc(sizeof(StackNode));
    if (!node) return ERROR;
    // printf("push \t %d\n", e);
    node->data = e;
    node->next = s->top;
    s->top = node;
    return OK;
}

Status pop(Stack *s, SElemp *e) {
    pStack node = s->top;
    if (!node->next) return ERROR;
    *e = node->data;
    // printf("pop \t %d\n", *e);
    s->top = node->next;
    free(node);
}

Status travelStack(Stack *s) {
    pStack node = s->top;
    while (node->next) {
        printf("%d\n", node->data);
        node = node->next;
    }
    return OK;
}

//binaryTree
int isNumbers(ElemType input) {
    if (input != '+' && input != '-' && input != '*' && input != '/'
        && input != '(' && input != ')')
        return 1;
    return 0;
}

void createTree (BiTree *T, String *string) {
    BiTree node = (BiTree) malloc(sizeof(TreeNode));
    int isBracket = 0;
    if (!node) exit(OVERSTACK);
    printf("numIndex = %d\t operator = %d\n", string->numIndex, string->operaIndex);
    if (string->numIndex <= string->operaIndex && *(string->opera) != '(') {
        node->data = nextInt(string);
        // printf("in get int num %d\n", node->data);
        node->isNum = 1;
    } else {
        node->data = nextChar(string);
        node->isNum = 0;
    }
    node->rightChild = NULL;
    node->leftChild = NULL; 
    if ((*T) == NULL) {
        (*T) = node;
        createTree(T, string);
        return;
    }
    //特殊符号处理
    if (!node->isNum && node->data == '=') {
        return;
    } else if (!node->isNum && node->data == ')') {
        string->operaIndex--;
        return;
    } else if (!node->isNum && node->data == '(') {
        BiTree* bt = (BiTree*) malloc(sizeof(BiTree));
        *bt = NULL;
        isBracket = 1;
        string->operaIndex -= 1;
        createTree(bt, string);
        node = *bt;
        if (!(*T)->leftChild) {
            (*T)->leftChild = node;
        } else if (!(*T)->rightChild) {
            (*T)->rightChild = node;
        }     
    }
    if (!isBracket) {
        if (node->isNum) {
            if (!(*T)->rightChild) {
                (*T)->rightChild = node;
            } else {
                (*T)->rightChild->rightChild = node;
            }
        } else {
            if (node->data == '+' || node->data == '-') {
                node->leftChild = (*T);
                *T = node;
            } else if (node->data == '*' || node->data == '/') {
                BiTree temp = (*T);
                int rootData = temp->data;
                if (rootData == '*' || rootData == '/') {
                    node->leftChild = *T; 
                    *T = node;
                } else if (rootData == '+' || rootData == '-') {
                    node->leftChild = (*T)->rightChild;
                    (*T)->rightChild = node;
                } else {
                    node->leftChild = *T;
                    *T = node;
                }
            }
        }        
    }
    createTree(T, string);
}

void travel(BiTree T) {
    if (!T) return;
    travel(T->leftChild);
    travel(T->rightChild);
    printf("%d\t", T->data);
}

void calc(BiTree T, Stack *s) {
    if (!T) return;
    calc(T->leftChild, s);
    calc(T->rightChild , s);
    if (T->isNum) {
        push(s, T->data);
    } else {
        SElemp a, b;
        pop(s, &b);
        pop(s, &a);
        push(s, operator(a, T->data, b));
        printf("%d%c%d\n", a, T->data, b);
    }
}

int operator(int a, char opera, int b) {
    switch(opera) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b; 
    }
    return ERROR;
}

//string
int nextInt(String *s) {
    int results = 0;
    while (*(s->num) < '0' || *(s->num) > '9') {
        s->num++;
    }
    for (int i = 0; *(s->num) >= '0' && *(s->num) <= '9'; i++) {
        results = results * 10 + *(s->num) - '0';
        s->num++;
    }
    s->numIndex++;
    // printf("num = %d\n", results);
    s->num++;
    return results;
}

char nextChar(String *s) {
    while (*(s->opera) >= '0' && *(s->opera) <= '9') {
        s->opera++; 
    }
    s->operaIndex++;
    // s->operaIndex++;
    // printf("opera = %c\n", *(s->opera));
    return *(s->opera++);
}

void createString(String *string) {
    gets(string->data);
    string->num = &string->data[0];
    string->opera = &string->data[0]; 
    string->operaIndex = 0;
    string->numIndex = 0;
}

void calculator() {
    BiTree *t = (BiTree*) malloc(sizeof(BiTree));
    Stack s;
    String string;
    createString(&string);
    *t = NULL;
    createTree(t, &string);
    createStack(&s);
    calc(*t, &s);
    travelStack(&s);
}

int main () {
    calculator();    
    return 0;
}
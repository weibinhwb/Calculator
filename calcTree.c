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

//树的结点结构
typedef struct TreeNode {
    struct TreeNode *leftChild;
    struct TreeNode *rightChild;
    int isNum;
    ElemType data;
}TreeNode, *BiTree;

//栈的结点结构
typedef struct StackNode {
    struct StackNode *next;
    SElemp data;
} StackNode, *pStack;
typedef struct {   
    pStack top;
} Stack;

//表达式结构
typedef struct {
    char data[MAX];
    char *num;
    char *opera;
    int numIndex;
    int operaIndex;
} String;

//栈的函数
Status createStack(Stack *s);
Status push(Stack *s, SElemp e);
Status pop(Stack *s, SElemp *e);
Status travelStack(Stack *s);
//表达式树的函数
void createTree (BiTree *T, String *string);
int priority(int currentNode, int input);
void travel(BiTree T);
//计算函数
int operator(int a, char opera, int b);
void calc(BiTree T, Stack *s);
//表达式处理函数
int nextInt(String *s);
char nextChar(String *s);
void createString(String *string);
//计算器
void calculator();
//生成一个栈
Status createStack(Stack *s) {
    s->top = (pStack) malloc(sizeof(StackNode));
    if (!s->top) return ERROR;
    s->top->next = NULL;
    return OK;
}

//压栈
Status push(Stack *s, SElemp e) {
    pStack node = (pStack) malloc(sizeof(StackNode));
    if (!node) return ERROR;
    // printf("push \t %d\n", e);
    node->data = e;
    node->next = s->top;
    s->top = node;
    return OK;
}
//出栈
Status pop(Stack *s, SElemp *e) {
    pStack node = s->top;
    if (!node->next) return ERROR;
    *e = node->data;
    // printf("pop \t %d\n", *e);
    s->top = node->next;
    free(node);
}
//遍历栈
Status travelStack(Stack *s) {
    pStack node = s->top;
    while (node->next) {
        printf("%d\n", node->data);
        node = node->next;
    }
    return OK;
}
//生成表达式树,(*T)一直指向当前结点
void createTree (BiTree *T, String *string) {
    BiTree node = (BiTree) malloc(sizeof(TreeNode));
    //是否有括号，0为否
    int isBracket = 0;
    int isFirstNode = 0;
    if (!node) exit(OVERSTACK);
    if (string->numIndex <= string->operaIndex && *(string->opera) != '(') {
        node->data = nextInt(string);
        node->isNum = 1;
    } else {
        node->data = nextChar(string);
        node->isNum = 0;
    }
    node->rightChild = NULL;
    node->leftChild = NULL; 
    //第一个结点
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
        //生成新的子树
        createTree(bt, string);
        node = *bt;
        //这里需要判断，如果当前结点的右子树不为空，且右子树的值域是乘（除），则括号内的表达式树是乘（除）结点的右子树
        if ((*T)->rightChild != NULL && ((*T)->rightChild->data == '*' || (*T)->rightChild->data == '/')) {
            (*T)->rightChild->rightChild = node;
        } else {
            (*T)->rightChild = node;
        }  
    }
    //读到括号则跳过
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

//表达式中的下一个整数
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
    s->num++;
    return results;
}
//表达式中的下一个操作符
char nextChar(String *s) {
    while (*(s->opera) >= '0' && *(s->opera) <= '9') {
        s->opera++; 
    }
    s->operaIndex++;
    return *(s->opera++);
}
//生成表达式
void createString(String *string) {
    gets(string->data);
    string->num = &string->data[0];
    string->opera = &string->data[0]; 
    string->operaIndex = 0;
    string->numIndex = 0;
}
//计算器
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
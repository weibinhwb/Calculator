#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>

#define OK 1
#define ERROR 0

typedef int Status;
typedef int ElemType;
typedef struct StackNode {
    struct StackNode *next;
    ElemType data;
} StackNode, *pStack;

typedef struct Stack{
    pStack top;
} Stack;

Status createStack(Stack *s) {
    s->top = (pStack) malloc(sizeof(StackNode));
    if (!s->top) return ERROR;
    s->top->next = NULL;
    return OK;
}

Status push(Stack *s, ElemType e) {
    pStack node = (pStack) malloc(sizeof(StackNode));
    if (!node) return ERROR;
    // printf("push \t %d\n", e);
    node->data = e;
    node->next = s->top;
    s->top = node;
    return OK;
}

Status pop(Stack *s, ElemType *e) {
    pStack node = s->top;
    if (!node->next) return ERROR;
    *e = node->data;
    // printf("pop \t %d\n", *e);
    s->top = node->next;
    free(node);
}

Status travel(Stack *s) {
    pStack node = s->top;
    while (node->next) {
        printf("%d\n", node->data);
        node = node->next;
    }
    return OK;
}

ElemType getTop(Stack *s) {
    if(!s->top) return ERROR;
    return s->top->data;
}

int deep(Stack *s) {
    pStack node = s->top;
    int count = 0;
    while (node->next) {
        count++;
        node = node->next;
    }
    return count;
}

int precede(int ch1, int ch2) {
    // printf("ch1 == %c, ch2 == %c\n", ch1, ch2);
    if ((ch1 == '+' || ch1 == '-') && (ch2 == '+' || ch2 == '-' || ch2 == ')' || ch2 == '='))
        return '>';
    if ((ch1 == '*' || ch1 == '/') && (ch2 == '+' || ch2 == '-' || ch2 == ')' || ch2 == '=' || ch2 == '*' || ch2 == '/'))
        return '>';
    if (ch1 == ')' && (ch2 == '+' || ch2 == '-' || ch2 == '=' || ch2 == '*' || ch2 == '/' || ch2 == ')'))
        return '>';
    if (ch1 == '(' && ch2 == ')')
        return '=';
    if (ch1 == '#' && ch2 == '=')
        return '=';
    return '<';
}

int operater(int a, int operate, int b) {
    // printf("a = %d, b = %d, operate = %c\n", a, b, operate);
    switch(operate) {
        case '+':
            return a + b;
        case '-':
            // printf("a - b = %d\n", a - b);
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
    }
    return ERROR;
}

void EvaluateExpression() {
    Stack optr, opnd;
    createStack(&optr);
    createStack(&opnd);
    push(&optr, '#');
    int c = getchar();
    while (c != '=' || getTop(&optr) != '#') {
        if (c != '+' && c != '-' && c != '*' && c != '/' && c != '(' && c != ')' && c != '=' && c != '#') {
            push(&opnd, c - '0');
            c = getchar();
        } 
        else {
            ElemType x, operate, a, b;
            char ch = precede(getTop(&optr), c);
            switch(ch) {
                case '<':
                    push(&optr, c);
                    c = getchar();
                    break;
                case '=':
                    pop(&optr, &x);
                    c = getchar();
                    break;
                case '>':
                    pop(&optr, &operate);
                    pop(&opnd, &b);
                    pop(&opnd, &a);
                    push(&opnd, operater(a, operate, b));
                    break;
            }
        }
    }
    travel(&opnd);
}
int main(int argc, char const *argv[])
{
    EvaluateExpression();
    return 0;
}

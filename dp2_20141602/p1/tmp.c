#include <stdio.h>

#define MAX 100

int stack[MAX];        // 스택의 긴  통
int top;            // 스택의 상단

void init_stack(void){
    top = -1;
}

int push(int t){

    if (top >= MAX - 1){
        printf("\n    Stack overflow.");
        return -1;
    }

    stack[++top] = t;
    return t;
}

int pop(void){
    if (top < 0){
        printf("\n   Stack underflow.");
        return -1;
    }
    return stack[top--];
}

void print_stack(void){
    int i;
    printf("\n Stack contents : TOP ----> Bottom\n");
    for (i = top; i >= 0; i--){
        printf("%-6d", stack[i]);
    }
}

int get_stack_top(void){
    return (top < 0) ? -1 : stack[top];
}

int is_stack_empty(void){
    return (top < 0);
}

int is_operator(int k){
    return (k == '+' || k == '-' || k == '*' || k == '/');
}

int is_legal(char *s){
    int f = 0;
    while (*s){
        while (*s == ' '){
            s++;
        }

        if (is_operator(*s)){
            f--;
        }
        else{
            f++;
            while (*s != ' '){
                s++;
            }
        }
        if (f < 1) break;
        s++;
    }
    return (f == 1);
}

int precedence(int op){
    if (op == '(') return 0;
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    else return 3;
}

void postfix(char *dst, char *src){
    char c;
    init_stack();
    while (*src){
        if (*src == '('){
            push(*src);
            src++;
        }
        else if (*src == ')'){
            while (get_stack_top() != '('){
                *dst++ = pop();
                *dst++ = ' ';
            }
            pop();
            src++;
        }
        else if (is_operator(*src)){
            while (!is_stack_empty() &&
                precedence(get_stack_top()) >= precedence(*src)){
                *dst++ = pop();
                *dst++ = ' ';
            }
            push(*src);
            src++;
        }
        else if (*src >= '0' && *src <= '9'){
            do{
                *dst++ = *src++;
            } while (*src >= '0' && *src <= '9');
            *dst++ = ' ';
        }
        else{
            src++;
        }
    }

    while (!is_stack_empty()){
        *dst++ = pop();
        *dst++ = ' ';
    }
    dst--;
    *dst = 0;
}

int calc(char *p){
    int i;
    init_stack();
    while (*p){
        if (*p >= '0' && *p <= '9'){
            i = 0;
            do{
                i = i * 10 + *p - '0';
                p++;
            } while (*p >= '0' && *p <= '9');
            push(i);
        }
        else if (*p == '+'){
            push(pop() + pop());
            p++;
        }
        else if (*p == '*'){
            push(pop() * pop());
            p++;
        }
        else if (*p == '-'){
            i = pop();
            push(pop() - i);
            p++;
        }
        else if (*p == '/'){
            i = pop();
            push(pop() / i);
            p++;
        }
        else{
            p++;
        }
    }
    return pop();
}

void main(void){

    int r;
    char exp[256] = "2*3+6/2-4";
    char pf[256];

    postfix(pf, exp);
    printf("\nPostfix : %s", pf);

    if (!is_legal(pf)){
        printf("\n Exprssion is legal!");
        exit(1);
    }
    r = calc(pf);
    printf("\nAnswer : %d", r);

    return 0;
}


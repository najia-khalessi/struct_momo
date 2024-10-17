#include<stdio.h>
#include<stdlib.h>
//创建节点
typedef struct node {
    int date;
    struct node* next;
}Node;
//创建栈
typedef struct stack {
    Node* top;
    int count;
}Link_Stack;

//创建链表栈
Link_Stack *Creat_stack() {
    Link_Stack* p;
    p = (Link_Stack*)malloc(sizeof(Link_Stack));
    if (p == NULL) {
        printf("创建失败");
        exit(0);
    }
    p->count = 0;
    p->top = NULL;
    return p;
}

//入栈  push
Link_Stack *Push_stack  (Link_Stack* p, int elem) {
    if (p == NULL) {
        return NULL;
    }
    Node* temp;
    temp = (Node*)malloc(sizeof(Node));
    temp ->date = elem;
    temp->next = p->top;
    p->top = temp;
    p -> count++;
    return p;
}


//出栈 pop
Link_Stack *Pop_Stack (Link_Stack* p) {
    Node* temp;
    temp = p->top;
    if (p == NULL) {
        printf("error");
        return p;
    }else{
    p->top=p->top->next;
    free(temp);
    p->count--;
    return p;
    }
}

//遍历
int show_stack (Link_Stack* p) {
    Node* temp;
    temp = p->top;
    if(temp == NULL) {
        printf("error");
        return 0;
    }
    while (temp != NULL) {
        printf ("%d\n", temp->date);
        temp = temp ->next;
    }
    return 0;
}

int main(){
    Link_Stack* p;
    p = Creat_stack();
    int n = 5;
    int input[6] = {1,2,3,4,5,6};

    for (int i = 0; i < n; i++) {
        Push_stack (p, input[i]);
    }
    show_stack (p);
    printf("\n");
    Pop_Stack (p);
    show_stack (p);
    return 0; 
}
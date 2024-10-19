#include<stdio.h>
#include<stdlib.h>

typedef struct node {
    int date;
    struct node* next;
}Node;

typedef struct queue {
    Node* front;//头结点
    Node* rear;//尾结点
}queue;


//初始化结点
Node* int_node() {
    Node* n = (Node*)malloc(sizeof(Node));
    if (n == NULL) {
    exit(0);
    }
    return n;
}


//初始化队列
queue* int_queue () {
    queue* q = (queue*)malloc(sizeof(queue));
    if(q == NULL) {
    exit(0);
 }
    q->front = NULL;
    q->rear = NULL;
    return q;
}


//判断队列是否为空
int empty (queue* q) {
    return q->front == NULL;
}

//入队列
queue* push_queue (queue* q, int elem) {
    Node* n = int_node();
    n -> date = elem;
    n ->next = NULL;
    if (empty(q)) {
        q->front = n;
        q->rear = n;
    }else{
        q ->rear->next = n;
        q ->rear=n;
    }
    return q;
}

//出队
queue* out_queue (queue* q) {
    Node* n = q->front;
    if(empty(q)){
        return 0;
    }
    if (q->front == q->rear) {
        q->front = NULL;
        q->rear = NULL;
        free(n);
    }else{
        q->front = q->front->next;
        free(n);
    }
    return q;
}

//遍历
queue* show_queue(queue* q) {
    Node* n = int_node();
    n = q->front;
    if (empty(q)){
        return 0;
    }
    while (n !=NULL) {
        printf("%d", n->date);
        n = n->next;
    }
}



int main() {
    queue* q = int_queue();

    printf("入队\n");
    for (int i = 1; i <= 5; i++) {
        push_queue(q, i);
        show_queue(q);
        printf("\n");
    }
    printf("展示完整队列:");
    show_queue(q);
    printf("\n");

    printf("出队\n");
    for (int i = 1; i <= 5; i++) {
        out_queue(q);
        show_queue(q);
        printf("\n");
    }
    printf("展示完整队列:\n");
    show_queue(q);

    return 0;
}
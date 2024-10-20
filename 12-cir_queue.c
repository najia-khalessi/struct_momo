#define maxsize 10
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//循环队列的结构设计
typedef struct cir_queue {
    int date [maxsize];
    int front;
    int rear;
}cir_queue;


//初始化
cir_queue* init() {
    cir_queue* q = (cir_queue*)malloc(sizeof(cir_queue));
    if (q == NULL) {
        exit (0);
    }else{
    memset(q->date,0,sizeof(q->date));
    q->front  = 0;
    q->rear = 0;
    return q;}
}

//入队列 （加入一个date
void push (cir_queue* q, int date) {
    if (((q->rear+1)%maxsize) == q->front) {                   //先判断栈是否溢出
        printf("溢出");
        return ;
    }else{
        q->rear = (q->rear+1)%maxsize;
        q->date[q->rear] = date;
    }
}

//出队列  （仅仅出front的date
void pop (cir_queue* q) {
    //先判断队列是否为空
    if (q->rear==q->front) {  //两者都为0
        printf("队列为空，无法出队");
        return ;
    }else{
        q->date[q->front]=0;  //将第一个元素清空，如果不清空，可能会遇见逻辑错误
        q->front = (q->front+1)%maxsize;
    }

}


//遍历操作
void print (cir_queue* q ) {
    int i = q->front;
    while (q->rear != i) {
        i = (i+1)%maxsize;
        printf("%d",q->date[i]);
    }
    printf("\n");
}

int main() {
    cir_queue* q = init();

    printf("入队操作\n");
    for(int i=1; i<=6;i++){
        push(q,i);
    }
    print(q);


    printf("出队操作");
    for(int i=1; i<=3;i++){
        pop(q);
    print(q);
    }
    

    printf("遍历操作");
    print(q);

    return 0;
}
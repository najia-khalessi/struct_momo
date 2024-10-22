#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
    int date;
    struct Node *next;  //单链表的指针域
}Node,*LinkedList; //Node为节点类型，后面是指针类型名字，表示一个链表的结构


//初始化链表
LinkedList listinit{   
    Node *L;
    L = (Node*)malloc(sizeof(Node));  //开辟空间  
    if(L==NULL){
        printf("申请空间失败");
        //exit(0);
    }
    L->next=NULL;
}

//单链表--头插法
LinkedList LinkListCreatH( ){  //Link:istCreatH()是一个函数，用来创建一个带有头节点的链表并返回一个LinkList类型的值
    Node *L;   //Node是一个类型名，代表链表中的节点结构
               //L是指向节点的指针
    L = (Node*)malloc(sizeof(Node));  //计算Node节点的大小，以便分配节点的内存空间
    L->next = NULL;  //表示这是一个空链表
    int x;
    while(scanf("%d",&x)!=EOF);  //EOF文件结束符
        Node* p;  //这是用来表示新节点的指针，用于在每次循环迭代中创建新的节点。

    //程序会不断地从标准输入中读取整数，并将每个整数作为链表的一个节点插入到链表中，直到输入的整数遇到文件结束符为止。
}


//遍历
void PrintList (LinkedList L) {
    Node* p = L -> next;
    int i = 0;
    while (p ) {
        printf("第%d个元素是%d", ++i, p ->date);
        p = p->next;
    }
}

//元素的定向修改
LinkedList LinkedListReplace (LinkedList l, int x, int k) {
    Node* p = L->next;
    int i = 0;
    while (p) {
        if (p->date = x ) {
            p->date = k;
        }
        p = p->next;
    }
    return L;
}


//插入
LinkedList LinkedListInsert (LinkedList l, int i, int x) {
    Node* pre;
    pre = L;  //为了先找到位置i，所以设置一个前驱节点
    int tempi = 0;
    for (int tempi = 0; tempi < i; i++){
        pre = pre->next;
    }
    Node* p;
    p = (Node*)malloc(sizeof(Node));
    p->date = x;
    p->next = pre->next;
    pre->next = p;
    
    return L;
}

//删除
LinkedList LinkedListDelect (LinkedList L, int x){
    Node* pre,p;   //pre前驱节点，p查找节点
    p = L->next;
    while(p->date !=x){
        pre = p;
        p = p->next;
    }
    pre ->next =p-> next;
    free(p);
    
    return L;

}

int main(){
    
}
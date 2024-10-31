#include <stdio.h>
#include <stdlib.h>

#define SIZE 20    //定义hash大小

//定义结构体，用于存储哈希表数据项
struct DataItem {
    int data;      //数据项中的数据字段
    int key;       //数据项中的犍字段
};

struct DataItem* hashArray[SIZE];   //指向DataItem的指针数组，作为哈希表
struct DataItem* dummyItem;    //指针，表示已删除的项
struct DataItem* item;    //在插入和删除中使用


//定义函数，计算键的哈希值
int hashCode (int key ) {
    return key % SIZE;     //返回键对表大小饿取模值，作为哈希值
}


//通过键在哈希表中进行搜索操作
struct DataItem* search (int key ) {
    int hashIndex = hashCode (key );   //计算哈希值

    while (hashArray[hashIndex] != NULL) {
        if (hashArray[hashIndex] -> key == key){
            return hashArray[hashIndex];    // 如果找到了键，返回对应的数据项
        }

        ++hashIndex;   //移动到下一个哈希位置
        hashIndex %= SIZE;    // 如果到达表末尾，则回到表头
    }
    return NULL;   // 未找到键，返回 NULL
}


//定义函数，插入数据项
void insert (int key, int data) {
    struct DataItem* item = (struct DataItem*)malloc(sizeof(struct DataItem));

    item->data = data;
    item->key = key;
    int hashIndex = hashCode(key);

    while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
        ++hashIndex;
        hashIndex %=SIZE;
    }
    hashArray[hashIndex] = item;  //插入到空位置
}


//定义函数，删除数据项
struct DataItem* delete (struct DataItem* item) {
    int key = item->key;
    int hashindex = hashCode(key);

    while (hashArray[hashindex] != NULL) {
        if (hashArray[hashindex]->key == key) {
            struct DataItem* temp = hashArray[hashindex];  // 暂存要删除的数据项
            hashArray[hashindex] = dummyItem;    // 用删除标志替换原位置的数据项
            return temp;      // 返回删除的数据项
        }
    ++hashindex;
    hashindex %= SIZE;
    }
    return NULL;
}


//定义函数显示哈希表的内容
void display() {
    int i = 0;
    for (int i = 0; i < SIZE; i++) {
        if (hashArray[i] !=NULL) {    // 如果当前位置不为空，打印键和值
            printf ("(%d,%d)", hashArray[i]->key,  hashArray[i]->data);
        }else{
            printf("~~\n");    // 如果当前位置为空，打印占位符。
        }
    }
    printf("\n");
}


int main() {
    // 分配内存并初始化删除标志
    dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem)); 
    dummyItem->data = -1;  
    dummyItem->key = -1; 

    insert(1, 20);  // 插入多个数据项到哈希表。
    insert(2, 70);
    insert(42, 80);
    insert(7, 65);
    insert(12, 44);
    insert(14, 32);
    insert(17, 11);
    insert(13, 78);
    insert(37, 97);

    display();
    item = search(37);   // 搜索键为37的数据项

    if(item != NULL) {      // 如果找到了数据项，打印数据值。
      printf("Element found: %d\n", item->data);
    } else {                // 如果未找到数据项，打印未找到信息。
    printf("Element not found\n");
    }

    delete(item);  // 删除找到的数据项。
    item = search(37);  // 再次搜索键为37的数据项。

    if(item != NULL) {  // 如果找到了数据项，打印数据值。
       printf("Element found: %d\n", item->data);
    } else {  // 如果未找到数据项，打印未找到信息。
       printf("Element not found\n");
    }
}
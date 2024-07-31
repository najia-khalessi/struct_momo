//用二分查找（binary search）算法，在有序或无序的数组中查找指定的元素
//以 升序数组 为例，查找一个数，他每次考察当前部分的中间元素，如果中间元素刚好是要找的，就结束搜索，理论上时间复杂度O(1)
//平均时间复杂度，和最坏时间复杂度 均为O(log2 n)
//以前的数据库，用binary search去做检索
#include <stdio.h>

int binary_search(int arr[], int arraylong,int target){
    int left = 0; 
    int right = arraylong -1;
    int mid;

    if (left <= right) {
        printf("循环开始 -> 当前left: %d, right: %d\n", left, right);
        mid = left + (right - left)/2;
        printf("计算mid -> left: %d, right: %d, mid: %d\n", left, right, mid);
        
        if (arr[mid] == target) {
            printf("找到目标值 -> %d\n", target);
            return mid;
        } else if (arr[mid] > target){
            printf("找到比mid小的值\n");
            right = mid - 1;
        } else {
            printf("找比mid大的值\n");
            left = mid + 1;
        }

    }
    return -1;//表示未找到指定元素target;
}


void bubble_sort(int arr[], int arraylong) {
    int i, j, temp;
    for(i = 0; i<arraylong - 1; i++) {
        printf("第%d次外循环\n", i+1);
        for(j = 0; j<arraylong - i -1; j++) {
            if(arr[j]>arr[j+1]) {
                printf("swapping %d and %d\n", arr[j], arr[j+1]);
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
            printf("外循环%d次,内循环%d次", i+1, j+1);
            for(int k = 0; k<arraylong; k++) {
                printf("%d", arr[k]);
            }
            printf("\n");
        }
    }
}

int main() {
    const int arr[] = {5, 4, 3, 2, 1};
    const int arraylong = 5;
    const int target = 8;
    
    printf("原数组");
    for(const int i =0; i<arraylong; i++) {
        printf("%d", arr[i]);
    }
    printf("\n");

    bubble_sort(arr, arraylong);

    printf("排序后的数组");
    for(int i = 0; i<arraylong; i++) {
        printf("%d", arr[i]);
    }
    printf("\n");
    int result = binary_search(arr, arraylong, target);
    if(result != -1){
        printf("已找到目标值:%d",target);
    } else {
        printf("未找到目标值");
    }
    
    printf("\n");

    return 0;
}







































































































































































































































#include<stdio.h>
int binary(int arr[n], int target) {
    int low = 0;
    int high = n-1;
    mid = low + (high - low);
    while (low < high){
        if (target == arr[mid]){
            printf("%d",mid);
        }else if(arr[mid] < target){
            low = mid + 1;
        }else{
            high = mid - 1;
        }
    }
}

int main(){
    int arr[];
    int target,n;
    scanf("%d",n);
    for(int i = 0; i<n; i++){
        scanf("%d",a[i]);
    }
    scanf("%d", target);
    binary(arr[n], target);
    return 0;
}
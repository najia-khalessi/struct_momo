//交换数组中最大值和最小值的位置
#include <stdio.h>
int main(){
    int n;
    printf("请输入数组长度：");
    scanf ("%d",&n);
    int a[n];
    printf("请输入数组元素：");
    for(int i=0;i<n;i++){
        scanf( "%d",&a[i]);
    }


    int max = 0, min = 0;
    for(int i=1; i<n; i++){
        //找最大值
        if(a[i]>a[max]){
            max = i;
        }
//喜欢潇潇酱^_^
    }
    for(int i=1;i<n;i++){
        //找最小值
        if(a[i]<a[min]){
            min=i;
        }
    
    }

    int temp;
    temp=a[max];
    a[max]=a[min];
    a[min]=temp;

for(int i=0;i<n;i++){
    printf("%d",a[i]);
}

printf("\n");
return 0;
}
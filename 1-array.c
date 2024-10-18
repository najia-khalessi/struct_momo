//将数组中的数逆序存放 自己输入n个整数存入数组，将数组中的这n个数逆序存放后输出数组的元素
# include <stdio.h>
int main(){
    //提示用户输入数组长度
    int n;
    printf ( " 输入数字的数量 ");
    scanf("%d", &n);
    //定义数组
    int arr[n];
    //输入数组元素
    printf(" 请输入数组中的数字:");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        printf("arr[%d] = %d\n", i, arr[i]);
    }

//逆序存放数组元素
   /*for(int i=0;i<n;i++){
    int x,y,z;
    int u=0;
    int y=n;
    int arr[u]=x;
    int arr[y-1]=y;
    int z=arr[y-1];
    int arr[y-1]=x;
    int arr[u]=z;
    u++;
    y--;
    }*/
 
    //逆序存放数组元素
    printf("开始逆序存放数组\n");
    for(int i=0; i<n/2; i++){
        printf("交换 arr[%d]和arr[%d]\n", i, n-1-i);
        int temp;
        temp = arr[i];
        arr[i] = arr[n-1-i];
        arr[n-1-i] = temp;
        printf("交换结果:arr[%d] = %d, arr[%d] = %d\n", i, arr[i], n-1-i, arr[n-1-i]);
    } 



    //输出逆序后的数组元素
    for(int i = 0; i<n; i++){
        printf("%d",arr[i]);
    }
    printf("\n");
    return 0;
}
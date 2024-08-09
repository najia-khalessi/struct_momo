#include<stdio.h>
int add(int nums[], int arrlong, int target){
    for(int a = 0; a<arrlong; a++){
        for(int b = a + 1; b<arrlong; b++){
            if(target == nums[a]+ nums[b] && a!= b){
                printf("%d, %d", a, b);
            }else{
                prirntf("数组中没有元素相加等于目标值");
            }
        }
    }
    return 0;
}

int main() {
    int arrlong,target;
    printf("请输入数组长度：");
    scanf("%d",&arrlong);
    printf("请输入目标值：");
    scanf("%d",&target);
    int nums[arrlong];
    printf("请输入数组元素：");
    for(int i=0; i<arrlong; i++){
        scanf("%d", &nums[i]);
    }
    add(nums, arrlong, target);
    return 0;
}
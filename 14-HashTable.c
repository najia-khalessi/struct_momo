#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* 1512. 好数对的数目 
给你一个整数数组 nums 。
如果一组数字 (i,j) 满足 nums[i] == nums[j] 且 i < j ，就可以认为这是一组 好数对 。
返回好数对的数目。*/
int numIdenticalPairs(int* nums,int numsSize){
    int j, ans = 0;
    int hash[101];
    memset(hash, 0, sizeof(hash));
    for(j = 0; j < numsSize; j++) {
        ans += hash[ nums[j] ];
        hash[ nums[j]]++;
    }
        return ans;
}

int main (){
    int nums[] = {1,2,3,1,1,3};
    int numsSize = 6;
   
    int ans = numIdenticalPairs(nums,numsSize);
    printf("%d", ans);
    return 0;
}
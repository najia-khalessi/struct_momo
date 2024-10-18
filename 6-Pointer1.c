
/*1.用函数和指针完成下述程序功能：有两个整数a和b，由用户输入1,2或3。
如输入1，程序就给出a和b中大者，输入2，就给出a和b中小者，输入3，则求a与b之和。*/

#include <stdio.h>

void choice(int num, int a, int b, int *result){ //有点分不清*&的使用地方:*是用来提取变量名指向地址上的变量值，&是用来表示后面写的变量名所在的地址
    switch(num){                                    //*符号是写在会改变结果的变量前，&是写在要取地址的变量前
        case 1:
        *result=(a>b?a:b);
        printf("a,b中最大值为%d\n",*result);
        break;
        
        case 2:
        *result=(a<b?a:b);
        printf("a,b中最小值为%d\n",*result);
        break;
        
        case 3:
        *result=(a+b);
        printf("a,b的和为%d\n",*result);//为什么这里使用*？：这里表示*result指针所指向的内存位置的值
                                        //如果写成result或者&result，最后就会输出(a+b)的地址，而不是结果值
        break;
        
        default:
        printf("输入无效，请重新输入：\n"); //如果我这里要重新输入，应该怎么写？:可以加上一个do-while循环。
        break;
    }
}

int main(){
    int a=2;
    int b=3;
    int num,result;
    do{
        printf("请输入数字1or2or3:");
        scanf("%d",&num);  
        if(num>=1 && num<=3){
            choice(num, a, b, &result);//为什么这里是用&？&result表示变量result的内存地址。
                                    //将这个地址传递给函数后，函数内的指针result将指向这个地址，因此函数可以通过*result直接修改result变量的值。
                                    //如果不用&符号，那么函数中result值得改变并不会影响主函数中result值（有点像局部变量）
            break;
        }else{
            printf("请重新输入数字：");
        }
        }while(1);            
                              
    return 0;
}
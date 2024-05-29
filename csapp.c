#include <stdio.h>
/*typedef struct{
    int a[2];
    double d;
}struct_t;
    double fun(int i){
    volatile struct_t s;
    s.d=3.14;
    s.a[i]=1073741824;
    return s.d;
}*/

//蠢猪潇潇酱，哼大笨猪！
typedef struct {
    int a[2];
    double d;
} struct_t;

double fun(int i) {
    volatile struct_t s; //使用volatile修饰结构体变量，防止编译器对其进行优化
    s.d = 3.14;
    s.a[i] = 1073741824; //给结构体中数组的第 i 个元素赋值
    return s.d;
}

int main() {
    int i ;
    printf("i = ");
    for (i = 1; i <= 6; i++) {
        printf("%d", i);
    }
    
    printf("\n");

    printf("result: %lf\n", fun(i)); //调用 fun 函数并打印结果
    return 0;
}

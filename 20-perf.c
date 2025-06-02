//perf
//测试特点大小内存块，申请释放的速度
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>



//测试特定大小内存块，申请释放的速度
//参数1：申请内存的大小，参数2：申请内存的次数，返回值：单次申请释放内存的时间（ms）
int benchmark_memory(int size, int count){
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i = 0; i < count; i++){
        void *p = malloc(size);
        free(p);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long times = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
    return times;
}

int main(int argc, char *argv[])  {
    int size = 1024;   //2k对齐 因为page是4k
    int times = 0;
    int y = 0;

#if 0
    if (argc >= 2) {
        size = atoi(argv[1]);
    }
#endif
    for (y = 0; y <= 1000000; y++) {
    times = benchmark_memory(size, 1000000);
    printf("size %d bytes, time: %d ms\n", size, times);
    size += 1024;
    }
    return 0;
}

//malloc有两种申请内存的方式

//size 32506880 bytes < 32MB, time: 23 ms 
//通过brk()申请内存，然后free，但不是真正的释放
//因为brk是从一块缓冲池中申请的内存，所以free的时候，不是还给os，而是还给缓冲池
//所以，通过brk申请释放内存很快


//size 33555456 bytes > 32MB, time: 5951 ms
//通过mmap申请内存，然后free，
//mmap()申请内存，是从os申请的内存，所以free的时候，需要还给os
//申请内存是从page中申请的，所以申请内存的大小必须是page的整数倍
//free的时候，通过munmap()释放内存，这个时候，会发生page fault（中断），因为page中的信息被修改
//所以发生了tlb flush，所以释放内存很慢
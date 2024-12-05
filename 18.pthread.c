// 计算一个英语文本（默认为english.txt）中单词的数量
// 运用多线程，

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

#define NUM_THREADS 8        //定义线程数量，可根据情况变动
#define READ_SIZE 1048576    // 每次读取的块大小 1MB
#define LOOKAHEAD_SIZE 1024  // 额外读取的字节数以确保完整单词

struct ThreadData {
    int fd;           //文件描述打开的文件，能让多线程同时并行处理同一个文件
    off_t offset;     //文件偏移量：表示文件开头的位置，off_t专门用来定义文件偏移量
    size_t length;    //每个线程要读取的长度，最后一个线程可能有所调整
    int word_count;   //计算单词数量：用于统计每个线程中单词的数量
};

pthread_mutex_t lock;    //定义互斥锁


//每个线程中执行的函数：计算文本中单词的数量
void *count_words( void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;  // 将传入的参数转换成结构体,并且命名为data
    int fd = data->fd;                                  // 获取文件描述符
    off_t offset = data->offset;                        // 获取文件偏移量
    size_t length = data->length;                       // 获取要读取的长度
    int word_count = data->word_count;                                      // 初始化单词计数
    int in_word = 0;                                    // 标志是否在单词中

    char* buffer = (char*)malloc(READ_SIZE + LOOKAHEAD_SIZE + 1);   //为读取得内容分配内存
    if (buffer == NULL) {
        perror("Error allocating memory");
        return NULL;
    }

    while (length > 0) {
        size_t to_read;                  //定义每次循环读取的长度
        if (length > READ_SIZE) {
            to_read = READ_SIZE;
        }else {
            to_read = length;
        }

        pthread_mutex_lock(&lock);  //加锁
        size_t read_size = pread(fd, buffer, to_read + LOOKAHEAD_SIZE, offset);  //pread函数，从指定偏移量位置开始读数据
        pthread_mutex_unlock(&lock);  //解锁   目的：保证数据一致性，保护共享资源，实现线程同步

///////////TODO 考虑一下中断少读的情况/////////
        if (read_size < 0) {       
            perror("Error reading file");
            free(buffer);
            return NULL;
        }


        if (read_size < READ_SIZE + LOOKAHEAD_SIZE) { //如果读取到的字节数少于请求的字节数，说明已到文件末尾
            buffer[read_size] = '\0';
        }else {
            size_t lookahead = LOOKAHEAD_SIZE;
            while(lookahead > 0 && buffer[to_read + lookahead - 1] != ' ' && buffer[to_read + lookahead - 1] != '\n' &&
                   buffer[to_read + lookahead - 1] != '\t' && buffer[to_read + lookahead - 1] != '\0') {
                    lookahead--;
            }
            buffer[to_read + lookahead] = '\0';  //确保是在边界处结束
            read_size = to_read + lookahead;
        }

        //遍历文本。计算单词数量
        size_t i;
        for (i = 0; i < read_size; i++) {
            if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == '\0') {
                if (in_word) {
                    word_count++;    //如果之前为1（在单词中），且现在为空白符，那么就算是已经经过一个单词
                    in_word = 0;
                }
            }else {
                in_word = 1;       //表示在一个单词内
            }
        }

        offset += read_size;  // 更新偏移量
        length -= read_size;  // 更新剩余读取长度
    }
    if (in_word) {
        word_count++;  // 如果最后一个字符是非空白字符，单词计数加1
    }

    free(buffer);  // 释放内存
    return NULL;  // 函数自然退出
}



int main() {
    printf("Opening file:\n");
    int fd = open("english.txt",O_RDONLY);  //O_RDONLY可以保证不会对文件进行写入操作
    if (fd < 0) {
        perror("Error opening file");  //这个函数可以同时显示出是哪里出现了错误
        abort();   //如果文件打开失败，终止程序运行
    }

    printf("File opened successfully.\n");

    struct stat st;  //用来存储文件状态信息
    if (fstat(fd, &st) < 0) {  //fstat函数调用失败后会返回-1，成功会返回0
        perror("Error grtting file size.");
        close(fd);
        abort();
    }
    off_t file_size = st.st_size;  //获取文件大小

    printf("File size: %1ld bytes\n", file_size);

    pthread_t threads[NUM_THREADS];  //线程标识符数组:储存每个线程的标识符
    struct ThreadData thread_data[NUM_THREADS];  //线程数据结构数组

    pthread_mutex_init(&lock, NULL);

    size_t chunk_size = file_size / NUM_THREADS;  //计算每个线程处理的文本块大小

    for(int i = 0; i < NUM_THREADS; i++) {
        printf("Creating thread %d...\n", i);
        thread_data[i].fd =  fd;  //设置文件描述符
        thread_data[i].offset = i * chunk_size;
        thread_data[i].length = (i == NUM_THREADS - 1) ? file_size - i * chunk_size : chunk_size;  // 设置文本块长度，最后一个线程不是固定长度，所以要减去(即额外计算)
        thread_data[i].word_count = 0;  //初始化线程中单词数量
        int ret = pthread_create(&threads[i], NULL, count_words, &thread_data[i]);   //创建线程，其中&thread_data[i]是作为count_words函数中的参数
        if (ret != 0) {   //pthread_creat函数创建失败返回一个非0值，创建成功会返回一个0值
            perror("Error creating thread");
            close(fd);
            abort();
        }
        printf("Thread %d created successfully.\n", i);
    }
    int total_word_count = 0;  //初始化总单词数量
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Joining thread %d..\n", i);
        pthread_join(threads[i], NULL);   //等待线程运行，NULL为不需要获取返回值
        printf("Thread %d joined. word count:%d\n", i, thread_data[i].word_count);   //输出每个线程的单词数量
        total_word_count += thread_data[i].word_count;    //文件总的单词数量
    }
    pthread_mutex_destroy(&lock);  //销毁互斥锁
    close(fd);

    printf("Toyal word count: %d\n", total_word_count);

    return 0;

}
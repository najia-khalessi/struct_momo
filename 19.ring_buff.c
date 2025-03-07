#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>


#define BUFFER_RESERVE_LENGTH 10       //ringbuff的缓冲区预留长度
#define READ_INDEX_INVALID -2          //定义读取索引无效的错误码
#define DATA_UNIT_INDEX_INVALID -3     //定义数据单元索引无效的错误码
#define WRITE_INDEX_FULL -4            //定义环形队列已满时的错误码
#define WRITE_INDEX_INVALID -5         //定义写入索引无效时的错误码


// 定义节点数据头结构体
typedef struct NodeDataHead {
    unsigned int iSize;
    unsigned int iWrite;
    unsigned int iRead;
    unsigned int iOffset;
} NodeDataHead;


// 定义环形队列结构体
typedef struct ShmRingQueue {
    NodeDataHead *m_pDataHead;
    char *m_pBuff;
} ShmRingQueue;


// 初始化 ringqueue 的成员
void shmRingQueue_init(ShmRingQueue *queue, char *pShmBuff) {
    queue->m_pDataHead = (NodeDataHead *)pShmBuff;
    queue->m_pBuff = pShmBuff + sizeof(NodeDataHead);
}


// 销毁 ringqueue 的成员
void shmRingQueue_destroy(ShmRingQueue *queue) {
    queue->m_pBuff = NULL;
    queue->m_pDataHead = NULL;
}


// 打印环形队列的信息
void shmRingQueue_PrintInfo(ShmRingQueue *queue) {
    printf("DataHead:\n\tiSize:%u\n\tiBegin:%u\n\tiEnd:%u\n\tiOffset:%u\n", queue->m_pDataHead->iSize, queue->m_pDataHead->iWrite, queue->m_pDataHead->iRead, queue->m_pDataHead->iOffset);
}


// 获取当前 ringqueue 的剩余可用大小
int shmRingQueue_GetLeftSize(ShmRingQueue *queue) {
    int iRetSize = 0;
    int iWritePos = -1;
    int iReadPos = -1;

    iWritePos = queue->m_pDataHead->iWrite;
    iReadPos = queue->m_pDataHead->iRead;

    // 首尾相等，无数据
    if (iReadPos == iWritePos) {
        iRetSize = queue->m_pDataHead->iSize; 
    }
    // 首大于尾，一般情况，iWritePos 始终在"前"
    else if (iWritePos > iReadPos) {
        iRetSize = iWritePos - iReadPos;
    }
    // 首小于尾，分开计算
    else {
        iRetSize = queue->m_pDataHead->iSize - iWritePos + iReadPos;
    }

    // 注意：最大长度减去预留部分长度，保证首尾不会相接
    iRetSize -= BUFFER_RESERVE_LENGTH;

    return iRetSize;
}


// 判断当前 ringueue 是否已满（包含了减掉 BUFFER_RESERVE_LENGTH 的）
bool shmRingQueue_IsFull(ShmRingQueue *queue) {
    int iLeftSize = 0;
    iLeftSize = shmRingQueue_GetLeftSize(queue);
    if (iLeftSize > 0) {
        return false;
    } else {
        return false;
    }
}


// 是否需要加上 BUFFER_RESERVE_LENGTH
int shmRingQueue_GetUsedSize(ShmRingQueue *queue) {
    int iLeftSize = shmRingQueue_GetLeftSize(queue);
    if (iLeftSize > 0) {
        return queue->m_pDataHead->iSize - iLeftSize;
    } else {
        return queue->m_pDataHead->iSize;
    }
}


int shmRingQueue_GetDataUnit(ShmRingQueue *queue, char *pOut, int *pnOutLen) {
    int iLeftSize = 0;
    int iReadPos = -1;
    int iWritePos = -1;
    char *pbyCodeBuf = queue->m_pBuff;
    char *pTempSrc = NULL;
    char *pTempDst = NULL;

    // 参数判断
    if ((NULL == pOut) || (NULL == pnOutLen)) {
        return -1;
    }

    if (queue->m_pDataHead->iOffset <= 0 || queue->m_pDataHead->iSize <= 0) {
        return -1;
    }

    // 取读写指针
    iReadPos = queue->m_pDataHead->iRead; 
    iWritePos = queue->m_pDataHead->iWrite;

    // 无数据
    if (iReadPos == iWritePos) {
        *pnOutLen = 0;
        return 0;
    }

    // 剩余缓冲大小,小于包长度字节数,错误返回
    iLeftSize = shmRingQueue_GetLeftSize(queue);
    if (iLeftSize < sizeof(int)) {
        // 异常情况，重置首尾，返回错误
        *pnOutLen = 0;
        queue->m_pDataHead->iRead = 0;
        queue->m_pDataHead->iWrite = 0;
        return READ_INDEX_INVALID;
    }

    // copy data
    pTempDst = (char *)pnOutLen;
    pTempSrc = (char *)&pbyCodeBuf[0];

    // 包长度编码
    for (int i = 0; i < sizeof(int); i++) {
        pTempDst[i] = pTempSrc[iReadPos];
        iReadPos = (iReadPos + 1) % queue->m_pDataHead->iSize;
    }

    // 数据包长度非法
    if (((*pnOutLen) > shmRingQueue_GetUsedSize(queue)) || (*pnOutLen < 0)) {
        *pnOutLen = 0;
        queue->m_pDataHead->iRead = 0;
        queue->m_pDataHead->iWrite = 0;
        return DATA_UNIT_INDEX_INVALID;
    }

    pTempDst = pOut;

    // 首小于尾，未跨越终点
    if (iReadPos < iWritePos) {
        memcpy((void *)pTempDst, (const void *)&pTempSrc[iReadPos], (size_t)(*pnOutLen));
    } else {
        // 首大于尾且出现分段，则需要分段拷贝
        int iRightLeftSize = queue->m_pDataHead->iSize - iReadPos; // 查看当前要读取的数据是否被分段了
        if (iRightLeftSize < *pnOutLen) {
            // 分段拷贝
            memcpy((void *)pTempDst, (const void *)&pTempSrc[iReadPos], iRightLeftSize);
            pTempDst += iRightLeftSize;
            memcpy((void *)pTempDst, (const void *)&pTempSrc[0], (size_t)(*pnOutLen - iRightLeftSize));
        }
        // 否则，直接拷贝（临界情况），待拷贝的数据长度没有跨越分段
        else {
            memcpy((void *)pTempDst, (const void *)&pTempSrc[iReadPos], (size_t)(*pnOutLen));
        }
    }

    // 变更读指针
    iReadPos = (iReadPos + (*pnOutLen)) % queue->m_pDataHead->iSize;
    // 更新 iRead
    queue->m_pDataHead->iRead = iReadPos;

    return iReadPos;
}


// 写入数据
int shmRingQueue_PutDataUnit(ShmRingQueue *queue, const char *pIn, int nInLen) {
    int iLeftSize = 0;
    int iRead = -1;
    int iWrite = -1;

    // 参数判断
    if ((NULL == pIn) || (nInLen <= 0)) {
        return -1;
    }

    if (queue->m_pDataHead->iOffset <= 0 || queue->m_pDataHead->iSize <= 0) {
        return -1;
    }

    // 首先判断是已满
    if (shmRingQueue_IsFull(queue)) {
        return WRITE_INDEX_FULL;
    }

    // 取首、尾
    iRead = queue->m_pDataHead->iRead;
    iWrite = queue->m_pDataHead->iWrite;

    // 缓冲区异常判断处理
    if (iRead < 0 || iRead >= queue->m_pDataHead->iSize || iWrite < 0 || iWrite >= queue->m_pDataHead->iSize) {
        // 非法的 index，重置
        queue->m_pDataHead->iWrite = 0;
        queue->m_pDataHead->iRead = 0;
        return WRITE_INDEX_INVALID;
    }

    // 剩余缓冲大小小于新来的数据,溢出了,返回错误
    iLeftSize = shmRingQueue_GetLeftSize(queue);
    if ((int)(nInLen + sizeof(int)) > iLeftSize) {
        // 空闲不够，无法写入
        return WRITE_INDEX_FULL;
    }

    // 数据首指针
    char *pbyCodeBuf = queue->m_pBuff;

    char *pTempSrc = NULL;
    char *pTempDst = NULL;

    pTempDst = &pbyCodeBuf[0];
    pTempSrc = (char *)&nInLen;

    // 包的长度编码
    for (int i = 0; i < sizeof(nInLen); i++) {
        pTempDst[iWrite] = pTempSrc[i];
        iWrite = (iWrite + 1) % queue->m_pDataHead->iSize;
    }

    // 首大于尾，直接写入（说明 W-R 之间可写，且一定不会跨越分段，一旦跨越分段 iRead 必然小于 iWrite）
    if (iRead > iWrite) {
        memcpy((void *)&pbyCodeBuf[iWrite], (const void *)pIn, (size_t)nInLen);
    } else {
        // 首小于尾,本包长大于右边剩余空间,需要分两段循环放到 buff 存放
        if ((int)nInLen > (queue->m_pDataHead->iSize - iWrite)) {
            // 右边剩余 buff
            int iRightLeftSize = queue->m_pDataHead->iSize - iWrite;
            memcpy((void *)&pbyCodeBuf[iWrite], (const void *)&pIn[0], (size_t)iRightLeftSize);
            memcpy((void *)&pbyCodeBuf[0], (const void *)&pIn[iRightLeftSize], (size_t)(nInLen - iRightLeftSize));
        }
        // 右边剩余 buff 够了，直接写入即可
        else {
            memcpy((void *)&pbyCodeBuf[iWrite], (const void *)&pIn[0], (size_t)nInLen);
        }
    }

    // 更新尾偏移
    iWrite = (iWrite + nInLen) % queue->m_pDataHead->iSize;
    queue->m_pDataHead->iWrite = iWrite;

    return iWrite;
}


int main() {
    // 假设我们有一个共享内存缓冲区，大小为 1024 字节
    char shmBuffer[1024];
    ShmRingQueue queue;

    // 初始化环形队列
    shmRingQueue_init(&queue, shmBuffer);

    // 设置初始的环形队列信息
    queue.m_pDataHead->iSize = 1024 - sizeof(NodeDataHead);
    queue.m_pDataHead->iWrite = 0;
    queue.m_pDataHead->iRead = 0;
    queue.m_pDataHead->iOffset = 0;

    // 打印初始信息
    shmRingQueue_PrintInfo(&queue);

    // 测试写入数据
    char data1[] = "Hello, World!";
    int len1 = strlen(data1) + 1;  // 加上字符串结束符的长度
    int result = shmRingQueue_PutDataUnit(&queue, data1, len1);
    if (result == WRITE_INDEX_FULL) {
        printf("Failed to write data1, queue is full.\n");
    } else {
        printf("Successfully wrote data1 at index: %d\n", result);
    }

    // 打印当前信息
    shmRingQueue_PrintInfo(&queue);

    // 测试读取数据
    char out[100];
    int outLen = 0;
    result = shmRingQueue_GetDataUnit(&queue, out, &outLen);
    if (result == READ_INDEX_INVALID) {
        printf("Failed to read data, invalid index.\n");
    } else {
        out[outLen] = '\0';  // 确保字符串以 '\0' 结尾
        printf("Read data: %s\n", out);
    }

    // 打印最终信息
    shmRingQueue_PrintInfo(&queue);

    // 销毁环形队列
    shmRingQueue_destroy(&queue);

    return 0;
}



package main

import (
	"fmt"
	"sync"
	"time"
)

// 生产者函数
func producer(ch chan<- int, wg *sync.WaitGroup) {
	defer wg.Done()
	fmt.Println("Producer started.")
	for i := 1; i < 11; i++ { // 生产 10 个数据
		fmt.Printf("Producer produced: %d\n", i)
		ch <- i // 将数据发送到 channel
	}
	close(ch) // 生产者完成，关闭通道
	fmt.Println("Producer finished.")
	fmt.Println("")
}

// 消费者函数
func consumer(ch <-chan int, wg *sync.WaitGroup) {
	defer wg.Done()
	fmt.Println("Consumer started.")
	for data := range ch { // 从通道中读取数据，直到通道关闭
		time.Sleep(time.Millisecond * 200) // 模拟处理时间
		fmt.Printf("Consumer consumed: %d\n", data)
	}
	fmt.Println("Consumer finished.")
}

func main() {
	fmt.Println("Create a channel.")
	dataChannel := make(chan int, 100) // 创建一个有缓冲的 channel

	fmt.Println("Create a WaitGroup for synchronization.")
	var wg sync.WaitGroup

	// 启动生产者
	wg.Add(1)
	go producer(dataChannel, &wg)
	fmt.Println("Producer has been started.")

	// 启动消费者
	wg.Add(1)
	go consumer(dataChannel, &wg)
	fmt.Println("Consumer has been started.")

	// 等待所有 goroutine 完成任务
	wg.Wait()

	fmt.Println("All goroutines have finished.")
}
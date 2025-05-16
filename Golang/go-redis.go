// go操作redis 实现连接池和json序列化

package main

import (
	"context"
	"fmt"
	"github.com/redis/go-redis/v9"
)

var ctx = context.Background()

func main() {
	rdb := redis.NewClient(&redis.Options{
		Addr: "localhost:6379",     // Redis 服务器地址
		Password: "",               // 没有密码则留空
        DB:       0,                // 使用默认数据库
	})

	//测试连接
	pong, err := rdb.Ping(ctx).Result()
	if err != nil {
		fmt.Println("无法控制连接到redis:",err)
		return
	}
	fmt.Println("连接成功：",pong)
}
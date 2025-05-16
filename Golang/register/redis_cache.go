/*1.创建redis连接池 
2.从 Redis 连接池获取一个客户端
3.将 Redis 客户端放回连接池
4.从 Redis客户端 获取数据
5.将数据写入 Redis客户端
6.过期机制（可以先不写）

*/

package main

import (
    "fmt"
	"encoding/json"
	"context"
	"github.com/go-redis/redis/v8"
)


var ctx = context.Background()  //context是用于处理并发和超时控制的标准方法

//RedisCache结构体定义一个Redis缓存类
type RedisCache struct {
	client *redis.Client //存储Redis客户端的指针
}

//创建连接池
func newRedisClient() *redis.Client {
    return redis.NewClient(&redis.Options{
        Addr:     "localhost:6379", //redis地址
        Password: "",               //redis密码，无则留空
        DB:       0,                //默认数据库，默认是0
    })
}
	
//将Json化用户数据存到redis中
func (rc *RedisCache) SetToCache(key string, user User) error {
	// TODO 
	// json序列化调用struct.go中的ToJSON函数
	jsonData, err := json.Marshal(user)
	if err != nil {
		return fmt.Errorf("用户序列化失败: %v", err)
	}
	//将用户JSON数据存入redis
	err = rc.client.Set(ctx, key, jsonData, 0).Err()
	if err != nil {
		return fmt.Errorf("存储用户数据到Redis失败: %v", err)
	}
	return nil
}

//读取用户数据
func GetUserRedis(rc *RedisCache, key string) (*User, error) {  //弄清楚参数为什么是这几个？
	//先从redis中获取用户数据
	jsonData, err := rc.client.Get(ctx, key).Result()
	if err != nil {
		return nil, fmt.Errorf("从Redis获取用户数据失败: %v", err)
	}
	//反序列化用户数据
	var user User
	//jsonData是string类型，user是结构体类型，所以要反序列化

	//下面这行代码是什么意思？
	// TODO
	// 调用FromJSON函数
	err = json.Unmarshal([]byte(jsonData), &user)
	if err != nil {
		return nil, fmt.Errorf("反序列化用户数据失败: %v", err)
	}
	return &user, nil  //这里要用地址符号&吗？因为在文件redis_PoolJson.go中读取用户信息时用了&
}




	
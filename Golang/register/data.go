package main

import (
	"fmt"
	"log"
	"database/sql"
)

// 比如，现在写一个查询的数据逻辑
// 查询数据，先查redis，再查mysql，并将结果写入redis
// username 第二个参数应该改成redis的连接池，比如 rp *redis.Pool, 这个你后面自己改，你自己定义的redis连接池

func GetUserCache(db *sql.DB, rc *RedisCache, UserID int) (*User, error) {
	cachekey := fmt.Sprintf("user:%d", UserID) // 生成redis的key
	// 1. 先查redis,如果redis中有数据，直接返回
	cacheData, err := GetUserRedis(rc, cachekey) // 调用 redis_cache.go 中的函数
	if err == nil {  // 如果没有错误，说明redis中有数据
		// 从 redis 获取到缓存
		return FromJSON(cacheData) // 因为从缓存中读到的数据是json格式的，所以要反序列化
	}
	
	// 2. 如果redis中没有想要的数据，查mysql
	jsonData, err := GetUser(db, UserID) // 调用 mysql_db.go 中的函数
	if err != nil {
		return nil, fmt.Errorf("查询用户失败: %v", err)
	}

	// 3. 将从mysql中获取的数据写入redis缓存（因为是查询用户，如果2成功，那么3一定成立）
	jsonData, err := GetUser.ToJSON()
	if err != nil {
		return nil, fmt.Errorf("序列化用户数据失败: %v", err)
	}
	if err := SetToCache(rc, cachekey, jsonData); err != nil { // 调用 redis_cache.go 中的函数
		return nil, fmt.Errorf("存储用户数据到Redis失败: %v", err)
	}

	// 4. 返回结果
	return User, nil
}
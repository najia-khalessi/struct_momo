package main
import (
	"context"                           //管理请求上下文（比如超时控制）
	"encoding/json"                     //处理JSON格式的编码/解码
	"fmt"                               //格式化输出（如打印带变量的字符串）
	"log"		                        //打印错误日志	
	"github.com/go-redis/redis/v8"      //Redis客户端
)

//创建基础上下文。用于redis操作
var ctx = context.Background()  //context是用于处理并发和超时控制的标准方法

//redis客户端初始化
func newRedisClient() *redis.Client {
	return redis.NewClient(&redis.Options{  //这里是先确认返回一个redis的客户端，然后创建一个redis的client实例
		Addr:    "localhost:6379",
		Password:"",
		DB:      0,
		PoolSize: 10,
		MinIdleConns: 2,
	})
}

//定义JSON用户数据结构
type User struct {
	ID   int    `json:"id"`  //将ID设置为JSON时用“id”作为键，当序列化时会用到这些名称
	Name string `json:"name"`
	Age  int    `json:"age"`
}

//存储json化用户数据到Redis
func storeUser(rdb *redis.Client, key string, user User) error {   //rdb是指针表示reids客户端，key是redis中的键名，user用于存储用户数据
	jsonData, err := json.Marshal(user)   //将Go语言结构体转换成JSON字节切片
	if err!= nil {
		return err
	}
	err = rdb.Set(ctx, key, jsonData, 0).Err()  //ctx用于控制操作的超时和取消，0表示数据永不过期
			                                    //Set方法返回一个*redis.Cmd对象，通过调用其Err()方法可以获取操作过程中发生的错误
	return err
}

//读取用户数据，从Redis中获取存储的用户数据并且反序列化为User结构体
func getUser(rdb *redis.Client, key string) (*User, error) {
	jsonData, err := rdb.Get(ctx, key).Result()    //从redis中获取数据，Get方法返回一个*redis.Cmd对象，通过调用其Result()方法可以获取操作的结果（即键对应的值）
	if err != nil {
		return nil, err					//返回值类型是由于函数签名决定的
	}
	var user User
	err = json.Unmarshal([]byte(jsonData), &user)  //将JSON字节切片解析为Go语言结构体
	return &user, err
}

//主逻辑函数
func main() {
	rdb := newRedisClient()  //创建redis客户端
	user := User{            //创建User实例
		ID: 1,
		Name: "LT",
		Age: 20,
	}
	key := fmt.Sprintf("user:%d", user.ID)  //创建键名

	//存储
	err := storeUser(rdb, key, user)
	if err != nil {
		log.Fatal("存储失败：%d", err)
	}
	fmt.Printf("存储成功！\n")

	//读取
	retrievedUser, err := getUser(rdb, key)
	if err != nil {
		log.Fatal("读取失败：%d", err)
	}
	fmt.Printf("读取成功！读取结果为：%v \n", retrievedUser)  //%+v可以打印结构体的所有字段
}
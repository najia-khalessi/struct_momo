//注册系统 变量结构需要定义什么？
//ID，用户名，密码，生日，性别，邮箱(检测email是否合法)

//定义一个结构体，包含这些信息，对其进行序列化和反序列化


package main

import (
    "encoding/json"
	"fmt"
)

// 结构体用于映射数据库中的表
type User struct {
	UserID       int    `json:"userid"`
	UserName     string `json:"username"`
	UserPassWord string `json:"userpassword"`
	UserBirthday string `json:"userbirthday"`
	Gender       string `json:"gender"`			
	Email        string `json:"email"`
}

//序列化
func (u *User) ToJSON() (string,error) {  //接收者参数，函数名，接收参数，返回值
	jsonData, err := json.Marshal(u)  //将结构体转换为JSON格式
	if err != nil {
		return "", fmt.Errorf("序列化失败: %v", err)
	}
	return string(jsonData), nil	
}

//反序列化
func FromJSON(jsonData string) (*User, error) {
	var user User    //var声明的变量会自动初始化为该类型的零值
	err := json.Unmarshal([]byte(jsonData), &user)
	if err != nil {
		return nil, fmt.Errorf("反序列化失败: %v", err)
	}
	return &user, nil
}


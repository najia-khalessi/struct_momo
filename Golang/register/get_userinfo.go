package main

import (
	"os"
	"fmt"
	"strings"
	"bufio"

)

func getUserInfo() (*User, error){
	reader := bufio.NewReader(os.Stdin)

	fmt.Print("请输入用户名：")
	username, err := reader.ReadString('\n')
	if err != nil {
		return nil, fmt.Errorf("获取用户名失败: %v", err)
	}

	fmt.Print("请输入密码: ")
    password, err := reader.ReadString('\n')
    if err != nil {
        return nil, fmt.Errorf("获取密码失败: %v", err)
    }

    fmt.Print("请输入出生日期: ")
    birthday, err := reader.ReadString('\n')
    if err != nil {
        return nil, fmt.Errorf("获取出生日期失败: %v", err)
    }

    fmt.Print("请输入性别: ")
    gender, err := reader.ReadString('\n')
    if err != nil {
        return nil, fmt.Errorf("获取性别失败: %v", err)
    }

    fmt.Print("请输入邮箱: ")
    email, err := reader.ReadString('\n')
    if err != nil {
        return nil, fmt.Errorf("获取邮箱失败: %v", err)
    }



    //去除输入中的换行符
	username = strings.TrimSpace(username)
    password = strings.TrimSpace(password)
    birthday = strings.TrimSpace(birthday)
    gender = strings.TrimSpace(gender)
    email = strings.TrimSpace(email)

    user := &User{
        UserName:     username,
        UserPassWord: password,
        UserBirthday: birthday,
        Gender:       gender,
        Email:        email,
    }
	return user, nil
}
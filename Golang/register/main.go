
//负责处理用户请求

//与docker中的数据库相连，数据库名为go_test,表名为users

package main
import (
	"fmt"
	"log"
	_ "github.com/go-sql-driver/mysql" //导入MySQL驱动
)

func main() {
	// 初始化数据库
	db, err := InitDB()
	if err != nil {
		log.Fatalf("初始化数据库失败: %v", err)
	}

	//创建一个新用户
	newUser, err := getUserInfo()	
	if err != nil {
		log.Fatalf("获取用户信息失败：%v", err) 
	}

	//注册新用户
	if err := db.RegisterUser(*newUser); err != nil {    //为什么要加db?   
		log.Fatalf("注册用户失败: %v", err)
    }
    log.Printf("用户注册成功")
	

	//查询用户
	user, err := db.GetUserMysql(newUser.UserName, newUser.UserPassWord)
	if err != nil {
        log.Fatalf("查询用户失败: %v", err)
    }
    log.Printf("查询到用户: %+v", user)

	//更新用户
	if err := db.UpdataUser(newUser.UserName, newUser.UserPassWord, *newUser); err != nil {
        log.Fatalf("更新用户失败: %v", err)
    }
    log.Printf("更新用户成功")


	//删除用户
	username, password, err := getDeleteUserInfo()
    if err != nil {
        log.Fatalf("获取删除用户信息失败: %v", err)
    }
    
    if err := db.DeleteUser(username, password); err != nil {
        log.Fatalf("删除用户失败: %v", err)
    }
    log.Printf("用户删除成功")
	

}
//职责：封装与 MySQL 数据库的交互逻辑


//数据库操作是系统的核心功能之一，将其单独放在一个文件中，可以让其他模块（如主程序）通过简单的接口调用数据库操作，而无需关心具体的实现细节。
//如果未来需要更换数据库（如从 MySQL 切换到 PostgreSQL），只需修改这个文件即可，而无需修改其他模块。

package main

import (
	"database/sql"
	"log"
	_"github.com/go-sql-driver/mysql"
	"fmt"
	"os"
	"bufio"
	"strings"
)

func InitDB() (*sql.DB, error) {
	//连接数据库
	dsn := fmt.Sprintf("%s:%s@tcp(%s:%s)/%s?charset=utf8mb4&parseTime=True&loc=Local",
	"root, admin, localhost, 3307, register")

	db, err := sql.Open("mysql", dsn)
	if err != nil {
		return nil, fmt.Errorf("连接数据库失败: %v", err)
	}

	// 设置数据库连接池的参数
	db.SetMaxOpenConns(100) // 设置最大打开连接数
	db.SetMaxIdleConns(10)  // 设置最大空闲连接数
	db.SetConnMaxLifetime(0) // 设置连接的最大生命周期

	// 测试数据库连接
	if err := db.Ping(); err != nil {
		return nil, fmt.Errorf("数据库连接失败: %v", err)
	}
	return db, nil
}

/*1.函数的参数dsn从何而来？
2.第二个if的作用是什么？为什么要写这步？
3.return后面的返回值还是看不懂 请帮我再从为什么的角度解释一遍。

1.dsn表示数据库的连接字符串（Data Source Name）。它是由调用该函数的代码提供的。
比如： dsn := "user:password@tcp(127.0.0.1:3306)/dbname"
mySQLDB, err := NewMySQLDB(dsn) 
2.用于验证数据库连接是否可用。它会尝试与数据库服务器建立连接，并检查是否能够成功通信。如果连接失败，则返回错误信息。
3.返回两个值：一个指向 MySQLDB 结构体的指针（*MySQLDB），和error. 
  前面的db表示结构体中的db字段，后面的db表示函数中的局部变量db。
  结构体中的db字段是一个指向sql.DB的指针，表示数据库连接池。
  函数中的局部变量db也是一个指向sql.DB的指针，表示新创建的数据库连接池。
  通过返回这个结构体的指针，可以在其他地方使用这个数据库连接池进行操作。
*/

//在main文件中实现直接与docker中创建好的表相连，所以在mysql文件中不需要重新创建数据库表


//RegisterUser函数用于注册新用户
func (m *MySQLDB) RegisterUser(user User) error {
	query := `
	INSERT INTO users (user_name, user_password, user_birthday, gender, email)
	VALUES (?, ?, ?, ?, ?)
	`
	_, err := m.db.Exec(query, user.UserName,  user.UserPassWord, user.UserBirthday, user.Gender, user.Email)  //m：方法接收器，代表调用该方法的 MySQLDB 实例。m.db访问 MySQLDB 实例中的 sql.DB 字段。m.db.Exec：通过 MySQLDB 实例调用 sql.DB 的 Exec 方法，执行 SQL 语句。
	if err != nil {
		return err
	}
	return nil
}

//查询用户,并使用事务
func (m *MySQLDB) GetUserMysql(UserID int) (*User, error) {
	// 开始事务
	tx, err := db.Begin()
	if err != nil {
		return 0, fmt.Errorf("开始事务失败: %v", err)
	}

	// 查询数据
	query := `
	SELECT user_id, user_name, user_birthday, gender, email
	FROM users
	WHERE user_name= ? AND user_password = ?
	`
	//QueryRow：执行查询并返回单行结果---Scan：将查询结果扫描到 user 结构体中
	_, err := m.db.QueryRow(query, username, password).Scan(&user.UserID, &user.UserName, &user.UserBirthday, &user.Gender, &user.Email) 
	if err != nil {
		tx.Rollback() // 出现错误,回滚事务
		return 0, fmt.Errorf("查询用户失败: %v", err)
	}

	// 提交事务
	if err := tx.Commit(); err != nil {
		return 0, fmt.Errorf("提交事务失败: %v", err)
	}
	
	// 返回查询到的用户
	return &user, nil

}

//更新用户
func (m *MySQLDB) UpdataUser(oldUsername, oldPassword string, user User) error {
	//先验证用户身份
	existingUser, err := m.GetUser(oldUsername, oldPassword)
	if err != nil {
		log.Fatalf("验证用户身份失败： %v", err)
	}else {
		log.Printf("验证用户身份成功：%v", existingUser)
	}

	//构建更新语句
	query := `
	UPDATE users
	SET  user_name = ?, user_password = ?, user_birthday = ?, gender = ?, email = ?
	WHERE user_id = ?
	`
	//为什么下面等号不用:=？后面为什么要加上err()
    _, err = m.db.Exec(query, 
        user.UserName,
        user.UserPassWord,
        user.UserBirthday,
        user.Gender,
        user.Email,
        user.UserID)

	if err != nil {
		return err
	}
	return nil
}



//删除用户
func getDeleteUserInfo() (string, string, error) {
    reader := bufio.NewReader(os.Stdin)

    fmt.Print("请输入要删除的用户名：")
    username, err := reader.ReadString('\n')
    if err != nil {
        return "", "", fmt.Errorf("获取用户名失败: %v", err)
    }

    fmt.Print("请输入用户密码: ")
    password, err := reader.ReadString('\n')
    if err != nil {
        return "", "", fmt.Errorf("获取密码失败: %v", err)
    }

    // 去除输入中的换行符
    username = strings.TrimSpace(username)
    password = strings.TrimSpace(password)

    return username, password, nil
}


















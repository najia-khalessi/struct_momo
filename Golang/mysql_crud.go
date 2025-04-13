/*
// 创建mysql容器 -- run: docker run --name my-mysql -e MYSQL_ROOT_PASSWORD=my-secret-pw -d mysql   (my-secret-pw为密码)
// 进入mysql容器内部可交互式shell -- exec :docker exec -it 8db3594ad401 /bin/sh   (8db3594ad401为容器id)
                                          docker exec -it my-mysql mysql -u root -p
// 熟悉mysql的基本命令   :mysql -u root -p
查看数据库 SHOW DATABASES;
创建数据库 CREATE DATABASE TEST;
进入数据库 USE Test;
查看表 SHOW TABLES;
创建表 CREATE TABLE People (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(20), age INT);
重命名表 RENAME TABLE People TO users;
查看表结构 DESCRIBE users; 或者 SHOW COLUMNS FROM users;
查看表状态 SHOW TABLE STATUS FROM Test;   //什么时候要用到AUTO_INCREMENT？
编辑表内容 INSERT INTO users (id, name, age) VALUES (1, 'LT', 22);
查看表的具体内容 SELECT * FROM users;
更新表内容 UPDATE users SET age=98 WHERE name='LT';
创建索引  CREATE INDEX on_id ON users(id);

// 建立表并建立合适的主键和索引 (存在id,name,age)   主键索引id,普通索引on_id
// 使用mysql自带的主键自增	 :ALTER TABLE users MODIFY COLUMN id INT AUTO_INCREMENT;
Test: INSERT INTO users (name, age) VALUES ('TT', 18);
// 实现查找id同时显示出name  
1.复合主键:PRIMARY KEY (id, name)
2.视图:创建视图:CREATE VIEW user_id_name AS  SELECT id, name   FROM users;
       查询视图:SELECT * FROM user_id_name;
3.索引:创建索引:CREATE INDEX ID_NAME  ON users(id, name);
       查看索引:SELECT id, name   FROM users;
// 上述都是在mysql内部实现的
// 代码上：实现连接数据库后进行增删查改
*/

package main

import (
	"database/sql"
	"fmt"
	"log"
	_ "github.com/go-sql-driver/mysql"
)

// 数据库连接信息,定义const常量
const (
	dbUser     = "root"       // 数据库的用户名
	dbPassword = "admin" // 数据库的密码
	dbHost     = "localhost"
	dbPort     = "3307"       // 确保端口号是 3307
	dbName     = "go_test"  // 数据库名称，根据实际情况修改
       
)

type User struct {
	ID   int
	Name string
	Age  int
}

func main() {
	// 连接字符串，固定格式--用户名:密码@tcp(主机地址:端口号)/数据库名称?charset=utf8mb4&parseTime=True&loc=Local
	dsn := fmt.Sprintf("%s:%s@tcp(%s:%s)/%s?charset=utf8mb4&parseTime=True&loc=Local", dbUser, dbPassword, dbHost, dbPort, dbName)
	// 连接数据库
	db, err := sql.Open("mysql", dsn)
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	// 测试连接
	if err := db.Ping(); err != nil {
		log.Fatal(err)
	}
	fmt.Println("连接成功！")

	// 创建表
	createTable(db)


	// 插入数据
	insertTable(db)
	

	// 查询数据
	queryTable(db)

	// 更新数据
	updateTable(db)
	

	// 删除数据
	deleteTable(db)
	
}

// 创建表
func createTable(db *sql.DB) {
	query := `
	CREATE TABLE IF NOT EXISTS users (
		id INT AUTO_INCREMENT PRIMARY KEY,
		name VARCHAR(20),
		age INT
	);`
	_, err := db.Exec(query)  // 执行 SQL 语句
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("表创建成功！")
}

// 插入数据
func insertTable(db *sql.DB) {
	user := User{}

	fmt.Println("请输入id、name、age (用空格分隔)")
	_, err := fmt.Scanf("%d %s %d", &user.ID, &user.Name, &user.Age)
    if err != nil {
        log.Fatalf("读取输入失败: %v", err)
    }

	query := `
	INSERT INTO users (id, name, age)
	VALUES (?, ?, ?)`
	 _, err = db.Exec(query, user.ID, user.Name, user.Age)
    if err != nil {
		log.Fatalf("插入数据失败: %v", err)
	}
	fmt.Println("插入数据成功！")
}

// 查询数据
func queryTable(db *sql.DB) {
	query := `
	SELECT id, name, age
	FROM users`
	rows, err := db.Query(query)  // db.Query 用于执行返回结果集的 SQL 查询语句，例如 SELECT 语句。它返回一个 *sql.Rows 对象，表示查询结果集。
	if err != nil {
		log.Fatal(err)
	}
	
	for rows.Next() {   // 遍历结果集
		user := User{}
		if err := rows.Scan(&user.ID, &user.Name, &user.Age); err != nil {  // Scan 方法将查询结果的列值依次赋给传入的变量
			log.Fatal(err)
		}
		fmt.Printf("id: %d, name: %s, age: %d\n", user.ID, user.Name, user.Age)
	}
	defer rows.Close()  // 关闭结果集,避免资源泄漏，确保数据库连接池中的连接能够被正确释放并复用
}

// 更新数据
func updateTable(db *sql.DB) {
	user := User{}
	fmt.Println("请输入要更新的记录id、新name、新age (用空格分隔)")
    _, err := fmt.Scanf("%d %s %d", &user.ID, &user.Name, &user.Age)
    if err != nil {
        log.Fatalf("读取输入失败: %v", err)
    }
	query := `
	UPDATE users
	SET name = ?, age = ?
	WHERE id = ?`
	_, err = db.Exec(query, user.Name, user.Age, user.ID)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("更新数据成功！")
}

// 删除数据
func deleteTable(db *sql.DB) {
	user := User{}
	fmt.Println("请输入要删除的id")
	fmt.Scanf("%d", &user.ID) // 输入要删除的id

	query := `
	DELETE FROM users
	WHERE id = ?`
	_, err := db.Exec(query, user.ID)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("删除数据成功！")
}
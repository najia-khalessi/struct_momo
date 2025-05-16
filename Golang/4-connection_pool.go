package main
import (
	"database/sql"   //标准库，包含连接池
	"fmt"
	"log"
	_"github.com/go-sql-driver/mysql"
)

func main() {
    fmt.Println("连接数据库")
	dsn := "username:password@tcp(127.0.0.1:3306)/mydatabase"

	fmt.Println("打开连接池")
	db, err := sql.Open("mysql", dsn)  //*sql.DB返回数据库对象,err返回是否成功
	if err != nil {
	  log.Fatalf("无法打开数据库连接:%v, err")
	}
	defer db.Close()

	fmt.Println("测试连接")
	err = db.Ping()
	if err != nil {
		log.Fatalf("数据库连接错误：%v", err)
	}
	fmt.Println("数据库连接成功")

	fmt.Println("执行查询")
	rows, err := db.Query("SELECT id, name FROM users")
	if err != nil {
		log.Fatalf("查询失败: %v",err)
	}
	defer rows.Close() //确保释放资源

	fmt.Println("遍历查询")
	for rows.Next() {
		var id int
		var name string
		if err := rows.Scan(&id, &name); err != nil {
			log.Fatalf("扫描失败：%v", err)
		}
		fmt.Println("ID: %d, Name: %s\n", id, name)
	}

	fmt.Printfln("检查;遍历过程是否有错误")’
	if err := rows.Err(); err != nil {
		Log.Fatalf("遍历失败: %v",err)
	}
}

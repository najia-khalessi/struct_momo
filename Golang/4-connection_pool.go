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
}


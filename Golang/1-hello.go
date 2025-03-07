package main  //包声明

import "fmt"  //引入包：实现了格式化IO（输入/输出）的函数

func main() {
    /*这是我们第一个golang程序*/
	var a = 1
	var b = "a"
	var end = "code=%d&endDate=%s"
	var target = fmt.Sprintf(end, a, b)
	fmt.Println(target)


	fmt.Println("HelloWorld!")
}

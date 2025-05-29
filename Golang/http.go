package main
import(
	"net/http"
	"io"
	"fmt"
	"time"   //处理https请求超时问题
)

func main() {
     person := Person{
      UserId:   "120",
      Username: "jack",
      Age:      18,
      Address:  "usa",
   }

   json, _ := json.Marshal(person)
   reader := bytes.NewReader(json)

   resp, err := http.Post("https://golang.org", "application/json;charset=utf-8", reader)
   if err != nil {
      fmt.Println(err)
   }
   defer resp.Body.Close()
}
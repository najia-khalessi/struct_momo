/*国际摩尔斯密码定义一种标准编码方式，将每个字母对应于一个由一系列点和短线组成的字符串， 比如:

'a' 对应 ".-" ，
'b' 对应 "-..." ，
'c' 对应 "-.-." ，以此类推。
为了方便，所有 26 个英文字母的摩尔斯密码表如下：

[a".-", b"-...", c"-.-.", d"-..", e"."]
给你一个字符串数组 words ，每个单词可以写成每个字母对应摩尔斯密码的组合。

例如，"cab" 可以写成 "-.-..--..." ，(即 "-.-." + ".-" + "-..." 字符串的结合)。我们将这样一个连接过程称作 单词翻译 。
对 words 中所有单词进行单词翻译，返回不同 单词翻译 的数量。*/

abe  .- -... .
.--.... adee

#include <stdio.h>

struct TreeNode {
    int word_end;//字符结束标志
    struct TreeNode* dot;//左子节点，表示 .
    struct TreeNode* line;//右子节点，表示 -
};
 
      root
   dot    line
dot line        line

//创建一个新节点
TreeNode* createNode() {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));//malloc申请一块内存，并返回指向这块内存的指针
    if(newNode != NULL) {
        newNode->word_end = 0;//初始化标记该节点是否是一个单词结束的标志（word_end）为 0
        newNode->dot = NULL;
        newNode->line = NULL;
    }
    return newNode;
}

//构建摩尔斯码树
TreeNode* build_morse_tree(char* morse_codes[], int code_count) {
    TreeNode* root = createNode();

    for(int i = 0; i < code_count; i++) {
        char* code = morse_codes[i];
        TreeNode* current = root;

        for(int j = 0; j < strlen(code); j++) {//用于计算并返回一个以空字符 ('\0') 结尾的字符串的长度
            if(code[j] == '.') {
                if(current->dot ==NULL) {
                    current->dot =  createNode();//如果没有左子节点，创建一个
                }
                current = current->dot;//移动到左子节点
            } else if(code[j] == '-') {
                if(current->line == NULL) {
                    current->line = createNode();//如果没有右子节点，创建一个
                }
                current = current—>line;//移动到右子节点
            }
        }

        current->word_end = 1;//到摩尔斯码的末尾，标记这个单词结束
    }
    return root;
}





/*void split_word_to_char(char *word) {
    printf("将单词拆分成一个个字符");
    int i, j;
    for (i = 0; str[i] != '\0'; i++) {
        for (j = 0; str[i] != ' ' && str[i] != '\0'; j++, i++) {
            printf("%c ", str[i]);
        }
        printf("\n");
    }
}


void moss (char word) {
    printf("将字符转换成摩斯密码");
    split_word_to_char(word);

}

int main(){
    char word;
    printf("请输入需要翻译的单词：");
    scanf("%c", word);

    moss(word);
    printf("这个单词的摩斯密码是：%c", )

    return 0;
}*/
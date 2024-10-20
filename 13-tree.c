#include<stdio.h>
#include<stdlib.h>



//结点结构
typedef struct node {
    int date;
    struct node* left;
    struct node* right;
}Node;

//树根
typedef struct {
    Node* root;
}Tree;


//创建树
void insert (Tree* tree, int value) {
     //先创建树的节点
    Node* node = (Node*)malloc(sizeof(node));
    node->date = value;
    node->left = NULL;
    node->right = NULL;

    //判断树是否为空
    if (tree->root == NULL) {
        printf("树为空");
        tree->root = node;
    }else{  //树不为空
        Node* temp = tree->root;
        while (temp != NULL) {
            if (temp->date > value) {
                if (temp->left == NULL) {    //如果temp->left不为空，则要将left当作root继续比较           
                    temp->left = node;
                    return ;
                }else{
                    temp = temp->left;
                }
            }else{
                if(temp->right == NULL) {
                    temp->right = node;
                    return ;
                }else{
                    temp = temp->right;
                }
            }
        }
    }
}

//递归
void inorder(struct Node* root, int* res, int* resSize) {
    if (!root) {
        return;
    }
    inorder(root->left, res, resSize);
    res[(*resSize)++] = root->val;
    inorder(root->right, res, resSize);
}

int* inorderTraversal(struct Node* root, int* returnSize) {
    int* res = malloc(sizeof(int) * 501);
    *returnSize = 0;
    inorder(root, res, returnSize);
    return res;
}


//遍历树 （先序遍历：根左右）
void preorder (Node* node){
    if (node != NULL){
        printf("%d",node->date);
        inorder(node->left);
        inorder(node->right);  //这里不是很懂！！！

    }
}

//遍历树 （中序遍历：左根右）
void inorder(Node* node){
    if (node != NULL)
    {
        inorder(node->left);
        printf("%d ",node->date);
        inorder(node->right);
    }
}


//遍历树 （后序遍历：左右根）
void inorder(Node* node){
    if (node != NULL)
    {
        inorder(node->left);
        inorder(node->right);
        printf("%d ",node->date);
    }
}

int mian(){
    Tree tree;
    tree.root = NULL;
    int n;
    scanf("%d",&n);


    //输入n个数并创建这个树
    for(int i = 0; i < n; i++) {
        int temp;
        scanf("%d", &temp);
        insert(&tree, temp);
    }
    inorder(tree.root);
    return 0;
}
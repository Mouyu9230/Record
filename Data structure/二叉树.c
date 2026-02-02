#include <stdio.h>
#include <stdlib.h>

//结点定义
typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

//结点创建
TreeNode* createNode(int data) {
    TreeNode* node=(TreeNode*)malloc(sizeof(TreeNode));
    if (!node) return NULL;

    node->data=data;
    node->left=NULL;
    node->right=NULL;
    return node;
}

//判空 
int isEmpty(TreeNode* root) {
    return root==NULL;
}

//获取树高 
int treeHeight(TreeNode* root) {
    if (root==NULL)
        return 0;

    int lh=treeHeight(root->left);
    int rh=treeHeight(root->right);

    return (lh>rh?lh:rh)+1;
}

//前序遍历 
void preorder(TreeNode* root) {
    if (root==NULL)
        return;

    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

// 中序遍历
void inorder(TreeNode* root) {
    if (root == NULL)
        return;

    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

// 后序遍历
void postorder(TreeNode* root) {
    if (root == NULL)
        return;

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

//销毁
void destroyTree(TreeNode* root) {
    if (root == NULL)
        return;

    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
}

int main() {

    TreeNode* root=createNode(1);
    root->left=createNode(2);
    root->right=createNode(3);
    root->left->left=createNode(4);
    root->left->right=createNode(5);

    printf("preorder: ");
    preorder(root);
    printf("\n");

    printf("inorder: ");
    inorder(root);
    printf("\n");

    printf("postorder: ");
    postorder(root);
    printf("\n");

    destroyTree(root);
    return 0;
}

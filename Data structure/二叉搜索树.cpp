#include <iostream>
using namespace std;

struct Node{
    int data;
    Node* left;
    Node* right;
};

//创建
Node* createNode(int x){
    Node* p=new Node;
    p->data=x;
    p->left=NULL;
    p->right=NULL;
    return p;
}

//插入
Node* insert(Node* root,int x){
    if(root==NULL)
        return createNode(x);

    if(x<root->data)
        root->left=insert(root->left,x);
    else if(x>root->data)
        root->right=insert(root->right,x);

    return root;
}

// 查找
Node* search(Node* root,int x){
    if(root==NULL||root->data==x)
        return root;

    if(x<root->data)
        return search(root->left,x);
    else
        return search(root->right,x);
}

//中序遍历（有序输出）
void inorder(Node* root){
    if(root==NULL)
        return;

    inorder(root->left);
    cout<<root->data<<" ";
    inorder(root->right);
}

/* 销毁 */
void destroy(Node* root){
    if(root==NULL)
        return;

    destroy(root->left);
    destroy(root->right);
    delete root;
}

int main(){
    int a[]={5,3,7,2,4,6,8};
    Node* root=NULL;

    for(int x:a)
        root=insert(root,x);

    inorder(root);
    cout<<endl;

    if(search(root,6))
        cout<<"found"<<endl;
    else
        cout<<"not found"<<endl;

    destroy(root);
    return 0;
}

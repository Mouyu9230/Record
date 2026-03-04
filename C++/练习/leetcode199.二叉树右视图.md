####给定一个二叉树的 根节点 root，想象自己站在它的右侧，按照从顶部到底部的顺序，返回从右侧所能看到的节点值。

代码：
```
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {

        vector<int> result;
        if(root==nullptr){
            return result;
        }
        queue<TreeNode*> nodes;
        nodes.push(root);
        while(!nodes.empty()){
            int size=nodes.size();
        for(int i=0;i<size;i++){
            TreeNode* temp=nodes.front();
            nodes.pop();
            if(i==size-1){
                result.push_back(temp->val);
            }
            if(temp->left!=nullptr)nodes.push(temp->left);
            if(temp->right!=nullptr)nodes.push(temp->right);
        }
        }

        return result;
    }
};
```
---
用队列管理节点，实现按层遍历
队列相关：
```
q.push(x);//尾部插入
q.pop();//去除队头
```

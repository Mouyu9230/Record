## 给你两个大小为 n x n 的二进制矩阵 mat 和 target 。现 以 90 度顺时针轮转 矩阵 mat 中的元素 若干次 ，如果能够使 mat 与 target 一致，返回 true ；否则，返回 false 。

#### 记这题主要是为了记旋转矩阵的方法

```
class Solution {
public:
    bool findRotation(vector<vector<int>>& mat, vector<vector<int>>& target) {
        int time=0;
        while(time!=4){
            
            if(is_mat_same(mat,target)){
                return true;
            }
            rotate(mat);
            time++;

        }
        return false;
    }

    void rotate(vector<vector<int>>& mat){//旋转矩阵的函数
        int size=mat.size();
        for(int i=0;i<size;i++){//可视作将矩阵依过（0,0）的对称轴翻转一周
            for(int j=i+1;j<size;j++){
                swap(mat[i][j],mat[j][i]);
            }
        }

        for(auto &x:mat){//翻转完了把每行reverse一下就能得到原矩阵顺时针翻转90度的矩阵了
            reverse(x.begin(),x.end());
        }
    }//逆时针同理    吧

    bool is_mat_same(vector<vector<int>>& mat, vector<vector<int>>& target){
        int size=mat.size();
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                if(mat[i][j]!=target[i][j]){
                    return false;
                }
            }
        }
        return true;
    }
};
```


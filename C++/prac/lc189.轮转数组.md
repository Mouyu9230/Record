#### 给定一个整数数组 nums，将数组中的元素向右轮转 k 个位置，其中 k 是非负数。

---

超时做法：

```cpp
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        k%=nums.size();
        for(int i=0;i<k;i++){
            oneturn(nums);
        }
    }

    void oneturn(vector<int>& nums){
        int thetaken=*(nums.end()-1);
        for(int i=nums.size()-2;i>=0;i--){
            nums[i+1]=nums[i];
        }
        nums[0]=thetaken;
    }
};
````

用取余规避多余的轮转，但无法应对nums数据太多的情况。

---

聪明的方法：

```cpp
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = nums.size();
        if (n == 0) return;
        k %= n;
        reverse(nums.begin(), nums.end());
        reverse(nums.begin(), nums.begin() + k);
        reverse(nums.begin() + k, nums.end());
    }
};
```

用自带的操作函数reverse进行3次翻转就行：

```
1234567->7654321->567 4321->567 1234->完成
```




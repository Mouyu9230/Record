//KMP算法通过预处理模式串，找出其中的自相似结构（前后缀重复），在匹配失败时根据处理得到的数据智能跳转，避免暴力法对时间的浪费
//当模式串中存在前后缀重复且失配发生在重复段的后面时，重新开始比对时就可以通过移动模式串使其前缀与主串对应的后缀相合，由此避免回退主串指针
//在比对二进制字符串时效果尤其好


#include<stdio.h>
#include<string.h>

void get_next(char *pattern,int next[]){//构造 next 数组
    int i=0;//当前计算next的位置
    int j=-1;//当前最长前后缀匹配长度
    int len=strlen(pattern);
    next[0]=-1;//开头对不上就后跳
    while(i<len-1){
        if(j==-1||pattern[i]==pattern[j]){//从零开始推进/前后缀字符相等，推进匹配位数
            i++;
            j++;
            next[i]=j;
        }else{
            j=next[j];//不匹配时尝试更短的前后缀
        }
    }
}

int kmp(char *text,char *pattern){//KMP 匹配
    int i=0;
    int j=0;
    int n=strlen(text);
    int m=strlen(pattern);
    int next[m];

    get_next(pattern,next);

    while(i<n&&j<m){
        if(j==-1||text[i]==pattern[j]){
            i++;
            j++;
        }else{
            j=next[j];//根据next中的数据决定把模式串的哪一位匹配过来
        }
    }
    if(j==m){
        return i-j;
    }
    return -1;
}

int main(){
    char text[]="ababcabcacbab";
    char pattern[]="abcac";

    int pos=kmp(text,pattern);
    if(pos!=-1){
        printf("match at %d\n",pos);
    }else{
        printf("no match\n");
    }
    return 0;
}

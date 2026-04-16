#include<bits/stdc++.h>

int main(){

    std::random_device rd;              // 真随机种子
    std::mt19937 gen(rd());     

    std::uniform_int_distribution<int> dist(1,20);

    int rdnum[50];

    for(int i=0;i<50;i++){
        int x = dist(gen);
        rdnum[i]=x;
    }

    FILE *fp = fopen("test_data.txt", "w");
    if (!fp) { printf("write failed\n"); return -1; }
    for (int i = 0; i<50; i++)
        fprintf(fp,"%d\n",rdnum[i]);
    fclose(fp);

    return 0;



}
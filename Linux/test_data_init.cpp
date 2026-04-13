#include<bits/stdc++.h>

int main(){

    std::random_device rd;              // 真随机种子
    std::mt19937 gen(rd());    

    std::uniform_int_distribution<int> dist(1, 100);

    int rdnum[100];

    for(int i=0;i<100;i++){
        int x = dist(gen);
        rdnum[i]=x;
    }

    FILE *fp = fopen("test_data.txt", "w");
    if (!fp) { printf("write failed\n"); return -1; }
    for (int i = 0; i<100; i++)
        fwrite(&rdnum[i], sizeof(int), 1, fp);
    fclose(fp);

    return 0;



}
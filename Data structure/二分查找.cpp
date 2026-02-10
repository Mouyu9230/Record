#include <iostream>
#include <vector>
using namespace std;

//要求有序
int binary_search(const vector<int>& v,int key){
    int left=0;
    int right=v.size()-1;

    while(left<=right){
        int mid=left+(right-left)/2;

        if(v[mid]==key)
            return mid;   
        else if(v[mid]<key)
            left=mid+1; 
        else
            right=mid-1; 
    }
    return -1; 
}

int main(){
    vector<int> v={1,3,5,7,9,11,13};

    int key=7;
    int pos=binary_search(v,key);

    if(pos!=-1)
        cout<<"find "<<key<<" at "<<pos<<endl;
    else
        cout<<"not found"<<endl;

    return 0;
}

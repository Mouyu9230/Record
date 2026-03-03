#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

class user{
    private:
    string name;
    long long int account;
    double money;
    string password;

    public:
    user(const string& n,long long int a,double m,const string& p):name(n),account(a),money(m),password(p){
        if(money<=0){
            cout<<"wrong input"<<endl;
        }
    }
    user():name("unknown"),account(0),money(0.0),password("password"){}

    string get_name()const{
        return name;
    }

    long long int get_account()const{
        return account;
    }

    double get_money()const{
        return money;
    }

    string get_password()const{
        return password;
    }

    bool operator==(const user& other){//重载==，使user之间可以使用
        return account==other.account;//可以访问同类型对象的private成员！
    }

    void add_money(double x){
        money+=x;return;
    }
    void minu_money(double x){
        if(x<=money){
        money-=x;
    }else{
        cout<<"wrong input"<<endl;
    }
    return;
    }

};
vector<user> users;

void init(){
    string str;
    long long int ac;
    double mo;
    string pa;
    cout<<"please enter name account money password"<<endl;
    cin>>str>>ac>>mo>>pa;
    user s(str,ac,mo,pa);
    users.push_back(s);
    cout<<"succeed."<<endl;
}

void check(){
    long long int ac;
    string pa;
    cout<<"please enter account password"<<endl;
    cin>>ac>>pa;
    int findflag=0;
    for(const auto& u:users){
        if(u.get_account()==ac&&u.get_password()==pa){
            cout<<"name:"<<u.get_name()<<endl;
            cout<<"money:"<<u.get_money()<<endl;
            findflag=1;
            break;
        }
    }
    if(findflag==0){
        cout<<"nope"<<endl;
    }

}

void trans(){
    cout<<"enter your account,password,and the target account"<<endl;
    long long int ac1;
    string pa;
    long long int ac2;
    double mo;
    cin>>ac1>>pa>>ac2;
    cout<<"how many?"<<endl;
    cin>>mo;
    int findflag=0;
    for(auto u:users){
        if(u.get_account()==ac1&&u.get_password()==pa){
            for(auto us:users){
                if(us.get_account()==ac2){
                    us.add_money(mo);
                    u.minu_money(mo);
                    findflag=1;
                    cout<<"succeed, "<<ac1<<" now "<<u.get_money()<<" left"<<endl;
                    cout<<ac2<<" now have "<<us.get_money()<<endl;
                    break;
                }
            }
            if(!findflag){
                cout<<"target nonexist"<<endl;
                findflag=1;
            }
        }
        if(findflag){
            break;
        }
    }
    if(!findflag){
        cout<<"wrong account/password"<<endl;
    }
}

void out(){
    long long int ac;
    string pa;
    double mo;
    int findflag=0;
    cout<<"please enter account password"<<endl;
    cin>>ac>>pa;
    cout<<"how much?"<<endl;
    cin>>mo;
    for(auto u:users){
        if(u.get_account()==ac&&u.get_password()==pa){
            if(u.get_money()<mo){
                cout<<"you only have "<<u.get_money()<<" on your account!"<<endl;
            }else{
                u.minu_money(mo);
                cout<<"succeed,now theres "<<u.get_money()<<" left"<<endl;
            }
            findflag=1;
            break;

        }
 
    }
       if(!findflag){
            cout<<"wrong input"<<endl;
        }
}

void in(){
    long long int ac;
    string pa;
    double mo;
    int findflag=0;
    cout<<"please enter account password"<<endl;
    cin>>ac>>pa;
    cout<<"how much?"<<endl;
    cin>>mo;
    for(auto u:users){
        if(u.get_account()==ac&&u.get_password()==pa){
                u.minu_money(mo);
                cout<<"succeed,now theres "<<u.get_money()<<" left"<<endl;
            findflag=1;
            break;

        }
 
    }
       if(!findflag){
            cout<<"wrong input"<<endl;
        }
}

void del(){
    string na;
    long long int ac;
    double mo;
    string pa;

    cout<<"please enter account password"<<endl;
    cin>>ac>>pa;
    user target(na,ac,mo,pa);
    auto it=find(users.begin(),users.end(),target);
    if(it!=users.end()){
        users.erase(it);
        cout<<"succeed."<<endl;
    }else{
        cout<<"failed."<<endl;
    }
}

void showall(){
    for(const auto& u:users){
        cout<<"name: "<<u.get_name()<<endl;
        cout<<"account: "<<u.get_account()<<endl;
        cout<<"money: "<<u.get_money()<<endl;
        cout<<"password: "<<u.get_password()<<endl;
        
    }
}
    

int main(){
    int userchoice;
    cout<<"=============银行信息管理系统=================="<<endl;
    cout<<"银行信息管理系统"<<endl;
    cout<<"输入数字选择对应服务"<<endl;
    cout<<"1:存款\n2:取款\n3:转账\n4:查看账户信息\n5:创建账户\n6:注销账户\n7:展示所有用户\n0:退出"<<endl;
    cout<<"========================================================"<<endl;
    while(1){
    cin>>userchoice;
    switch(userchoice){
        case 1:in();break;
        case 2:out();break;
        case 3:trans();break;
        case 4:check();break;
        case 5:init();break;
        case 6:del();break;
        case 7:showall();break;
        case 0:cout<<"thank you for using"<<endl;return 0;
        default:cout<<"wrong input"<<endl;
    }
    }
    return 0;
}


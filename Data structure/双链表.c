#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int data;
    struct Node* prev;
    struct Node* next;
}Node;

Node* init_list(){//创建
    Node *head=(Node*)malloc(sizeof(Node));
    if(!head)return NULL;
    head->data=0;
    head->prev=NULL;
    head->next=NULL;
    return head;   
}

int list_insert(Node* head,int num){//尾部插入
    if(!head) return -1;
    Node *p=head;
    while(p->next!=NULL){
        p=p->next;
    }
    Node *new=(Node*)malloc(sizeof(Node));
    if(!new) return -1;
    new->data=num;
    new->next=NULL;
    new->prev=p;
    p->next=new;
    return 0;
}

int location_insert(Node* head,int num,int location){//按位置插入
    if(!head||location<0) return -1;
    Node *p=head;
    for(int i=0;i<location&&p->next!=NULL;i++){
        p=p->next;
    }
    Node *new=(Node*)malloc(sizeof(Node));
    if(!new) return -1;
    new->data=num;
    new->next=p->next;
    new->prev=p;
    if(p->next!=NULL){
        p->next->prev=new;
    }
    p->next=new;
    return 0;
}

int get_location(Node* head,int location){//获取位置元素值
    if(!head||location<0) return -1;
    Node *p=head->next;
    for(int i=0;i<location&&p!=NULL;i++){
        p=p->next;
    }
    if(!p) return -1;
    return p->data;   
}

Node* num_find(Node* head,int num){//查找结点
    if(!head) return NULL;
    Node *p=head->next;
    while(p!=NULL){
        if(p->data==num){
            return p;
        }
        p=p->next;
    }
    return NULL;
}

int located_deletion(Node* head,int location){//按位置删除
    if(!head||location<0) return -1;
    Node *p=head->next;
    for(int i=0;i<location;i++){
        if(p==NULL){
            printf("location nonexist");
            return -1;
        }
        p=p->next;
    }
    if(!p) return -1;
    if(p->prev!=NULL){
        p->prev->next=p->next;
    }
    if(p->next!=NULL){
        p->next->prev=p->prev;
    }
    free(p);
    return 0;
}

void list_print(Node *head){//打印
    if(!head) return ;
    Node *p=head->next;
    while(p!=NULL){
        printf("%d\n",p->data);
        p=p->next;
    }
}

void clear_list(Node *head){//清除
    if(!head) return ;
    Node *p=head;
    while(p!=NULL){
        Node* next=p->next;
        free(p);
        p=next;
    }
}

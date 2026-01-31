#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
}Node;

typedef struct Queue{
    Node* front;
    Node* rear;
}Queue;

Queue* init_queue(){//创建
    Queue *q=(Queue*)malloc(sizeof(Queue));
    if(!q) return NULL;
    q->front=NULL;
    q->rear=NULL;
    return q;
}

int queue_empty(Queue *q){//判空
    if(!q) return -1;
    return q->front==NULL;
}

int enqueue(Queue *q,int num){//入队
    if(!q) return -1;
    Node *new=(Node*)malloc(sizeof(Node));
    if(!new) return -1;
    new->data=num;
    new->next=NULL;
    if(q->rear==NULL){
        q->front=new;
        q->rear=new;
    }else{
        q->rear->next=new;
        q->rear=new;
    }
    return 0;
}

int dequeue(Queue *q,int *out){//出队
    if(!q||queue_empty(q)) return -1;
    Node *temp=q->front;
    *out=temp->data;
    q->front=temp->next;
    if(q->front==NULL){
        q->rear=NULL;
    }
    free(temp);
    return 0;
}

int get_front(Queue *q){//获取队头元素
    if(!q||queue_empty(q)) return -1;
    return q->front->data;
}

void queue_print(Queue *q){//打印
    if(!q) return ;
    Node *p=q->front;
    while(p!=NULL){
        printf("%d\n",p->data);
        p=p->next;
    }
}

void clear_queue(Queue *q){//清除
    if(!q) return ;
    Node *p=q->front;
    while(p!=NULL){
        Node *next=p->next;
        free(p);
        p=next;
    }
    free(q);
}

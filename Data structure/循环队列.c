//队列：先进先出
//只允许在一端进行插入操作，在另一端进行删除操作
//使用分别指向头尾的两个指针来解决只有一个元素时头尾重合带来的问题。当头指针等于尾指针时则可知队列为空
//使用数组实现时，会遇到添加元素过多产生越界的情况，但由于此时数组开头处可能仍有空间，故称这种现象为假溢出
//解决方法：将头尾链接，构建循环队列
//具体如下：

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100

typedef struct Queue{
    int data[MAXSIZE];
    int front;
    int rear;
}Queue;

Queue* init_queue(){//创建
    Queue *q=(Queue*)malloc(sizeof(Queue));
    if(!q) return NULL;
    q->front=0;
    q->rear=0;
    return q;
}

int queue_empty(Queue *q){//判空
    if(!q) return -1;
    return q->front==q->rear;
}


int get_in(Queue *q,int num){//入队
    if(!q||queue_full(q)) return -1;
    q->data[q->rear]=num;
    q->rear=(q->rear+1)%MAXSIZE;
    return 0;
}

int get_out(Queue *q,int *out){//出队
    if(!q||queue_empty(q)) return -1;
    *out=q->data[q->front];
    q->front=(q->front+1)%MAXSIZE;
    return 0;
}

int get_front(Queue *q){//获取队头元素
    if(!q||queue_empty(q)) return -1;
    return q->data[q->front];
}

void queue_print(Queue *q){//打印
    if(!q) return ;
    int i=q->front;
    while(i!=q->rear){
        printf("%d\n",q->data[i]);
        i=(i+1)%MAXSIZE;
    }
}

void clear_queue(Queue *q){//清除
    if(!q) return ;
    free(q);
}

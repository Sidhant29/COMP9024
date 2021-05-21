#include<stdio.h>
#include<stdlib.h>

typedef struct node {
    int data;
    struct node* next;
} nodeLL;

nodeLL* initLL(){
    nodeLL *LL = NULL;
    return LL;
}

nodeLL* appendLL(nodeLL* n,int val){
    if(n==NULL){
        n = malloc(sizeof(nodeLL));
        n->data = val;
        //printf("%d",n->data);
        n->next = NULL;
        return n;
    }
    n->next = appendLL(n->next,val);
    return n;
}

void printLL(nodeLL* n){
    if (n==NULL){
        return;
    }
    else if (n->next==NULL){
        printf("%d",n->data);
    }
    else{
        printf("%d->",n->data);
    }
    printLL(n->next);   
}

void freeLL(nodeLL* n){
    if(n==NULL){
        return;
    }
    if(n!=NULL){
        freeLL(n->next);
    }
    free(n);
}

int main(){
    nodeLL *all = initLL();
    int val;
    printf("Enter list:\n");
    while(scanf("%d",&val)==1){
       
        all = appendLL(all,val);
    }
    printLL(all);
    freeLL(all);
    return 0;
}
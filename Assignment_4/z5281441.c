#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<ctype.h>

typedef struct BusStopNode 
{ 
    int serial_no;
    char *bus_stop_name; 
    struct BusStopNode* next; 
} BusStopNode;

typedef struct BusStopGraph 
{ 
    int no_nodes; 
    struct BusStopNode *start; 
} BusStopGraph; 

BusStopNode *initBusStopNode(int serial_no,char* bus_stop_name){
  BusStopNode *n = malloc(sizeof(BusStopNode));
  assert(n!=NULL);
  n->bus_stop_name = strcpy(n->bus_stop_name,bus_stop_name);
  n->serial_no = serial_no;
  n->next = NULL;
  return n;
}

BusStopGraph *initBusStopGraph(int count){
  BusStopGraph *n = malloc(sizeof(BusStopGraph));
  assert(n!=NULL);
  n->start = malloc(count*sizeof(BusStopNode));
  n->no_nodes=0;
  n->start=NULL;
  return n;
}

BusStopGraph *createBusStopGraph(char filename[]){
FILE *f = fopen(filename,"r");
	if(f==NULL){
		printf("FILE could not be opened, Please check file name , location");
		exit(1);
	}
  int no,count;
  char name[100];
  char line[100];
  char *get_no,*get_name;
  while(!feof(f)){
    fgets(line,BUFSIZ,f);
    printf("%s",line);
    int i=0;
    while(isdigit(*(line+i))){
      i++;
    }
    strncpy(get_no,line,i);
    strcpy(get_name,line+i+1);
    printf("%s,%s",get_no,get_name);
  }
}

int main(){
  BusStopGraph *graph= createBusStopGraph("BusStops.txt");
  return 0;  
}
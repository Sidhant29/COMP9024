/*
DISCLAIMER -
I was able to implement all the binomial heap functions but unfortunately I could not figure out how to implement the scheduler using the binomial heap in the given time.
I have outputted the constructed binomial heap and removed the top 3 elements for demonstration purposes.
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<string.h>

// This template is only a guide 
// You need to include additional fields, data structures and functions  

// data type for heap nodes
typedef struct HeapNode { 
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //key of this task
	int TaskName;  //task name 
	int Etime; //execution time of this task
	int Rtime; // release time of this task
	int Dline; // deadline of this task

	//... // add additional fields here
	int degree;		// degree of the subtree starting from this node
	struct HeapNode *sibling; 	//pointer for sibling on the right
	struct HeapNode *child;	// pointer for 1st child
	struct HeapNode *parent;	//pointer for direct parent
	
} HeapNode;

//data type for a priority queue (heap) 
typedef struct BinomialHeap{ //this is heap header
	int  size;      // count of items in the heap
	HeapNode *startNode;
	//... // add additional fields here
} BinomialHeap;

// create a new heap node to store an item (task) 
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
  // ... you need to define other parameters yourself) 	 
	HeapNode *new;
	new = malloc(sizeof(HeapNode));
	assert(new != NULL);
	new->key = k;
	new->TaskName = n;
	new->Etime = c;
	new->Rtime = r; 
	new->Dline = d;

	//... // initialise other fields 
	new->sibling=NULL;
	new->child=NULL;
	new->parent=NULL;
	new->degree = 0;
	return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert (T != NULL);
	T->startNode = NULL;
	//... // initialise all the fields here
	return T;
}
void Insert(BinomialHeap*, int, int, int, int, int);
// MY CUSTOM FUNCTIONS

void adjust_binomial_heap(HeapNode *curr,HeapNode *next){
if(next == NULL){
	return;
}
// printf("%d,%d -> %d,%d\n",curr->key,curr->degree,next->key,next->degree);
if(curr->degree<next->degree){
	adjust_binomial_heap(next,next->sibling);
	return;
}
if(curr->key<=next->key){
	// printf("im here\n");
	curr->sibling = next->sibling;
	next->sibling = NULL;
	if(curr->child==NULL){
		curr->child = next;
		next->parent = curr;
		curr->degree++;
		adjust_binomial_heap(curr,curr->sibling);
		return;
	}
	HeapNode *child = curr->child;
	while(child->sibling!=NULL){
		child = child->sibling;
	}
	child->sibling = next;
	next->parent = curr;
	curr->degree++;
	adjust_binomial_heap(curr,curr->sibling);
	return;
}
if(curr->key>next->key){
	// printf("im here 2")
	curr->sibling = NULL;
	if(next->child==NULL){
		next->child = curr;
		curr->parent = next;
		next->degree++;
		adjust_binomial_heap(next,next->sibling);
		return;
	}
	HeapNode *child = next->child;
	while(child->sibling!=NULL){
		child = child->sibling;
	}
	child->sibling = curr;
	curr->parent = next;
	next->degree++;
	adjust_binomial_heap(next,next->sibling);
	return;
}

}
void print_bin_heap(HeapNode*);
HeapNode *bin_heap_union(BinomialHeap *T,BinomialHeap *R){
	if(T->startNode==NULL)
		return R->startNode;
	else if(R->startNode==NULL)
		return T->startNode;
	
	HeapNode *ptr = T->startNode;
	HeapNode *ptr2 = R->startNode;
	BinomialHeap *union_heap = newHeap();
	if(ptr->degree<=ptr2->degree){
		union_heap->startNode = ptr;
		ptr=ptr->sibling;
		union_heap->startNode->sibling=NULL;
	} 
	else{
		union_heap->startNode = ptr2;
		ptr2=ptr2->sibling;
		union_heap->startNode->sibling=NULL;
	}
	HeapNode *u = union_heap->startNode;
	// printf("=====\n");
	// print_bin_heap(T->startNode);
	// printf("=====\n");
	print_bin_heap(R->startNode);
	while(ptr!=NULL || ptr2!=NULL){
		if(ptr2!=NULL && ptr!=NULL && ptr2->degree<=ptr->degree){
			// printf("%d,%d\n",ptr2->key,ptr2->degree);
			u->sibling = ptr2;
			u = u->sibling;
			ptr2=ptr2->sibling;
			u->sibling=NULL;
		}
		else if(ptr2!=NULL && ptr!=NULL &&ptr2->degree>ptr->degree){
			// printf("%d,%d\n",ptr->key,ptr->degree);
			u->sibling = ptr;
			u = u->sibling;
			ptr=ptr->sibling;
			u->sibling=NULL;
		}
		else if(ptr2==NULL && ptr!=NULL){
			// printf("%d,%d\n",ptr->key,ptr->degree);
			u->sibling = ptr;
			u = u->sibling;
			ptr=ptr->sibling;
			u->sibling=NULL;
		}
		else if(ptr2!=NULL && ptr==NULL){
			// printf("%d,%d\n",ptr2->key,ptr2->degree);
			u->sibling = ptr2;
			u = u->sibling;
			ptr2=ptr2->sibling;
			u->sibling=NULL;
		}
	}
	adjust_binomial_heap(union_heap->startNode,union_heap->startNode->sibling);
	return union_heap->startNode;
}
// Custom Functions END

// put the time complexity analysis for Insert() here    
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline 
  // You don't need to check if this task already exists in T 	 
  // put your code here
  HeapNode *new = newHeapNode(k,n,c,r,d);
  if(T->startNode == NULL){
	  T->startNode = new;
	  return;
  }
  new->sibling = T->startNode;
  T->startNode = new;
  T->size++;
  adjust_binomial_heap(T->startNode,T->startNode->sibling);
  while(T->startNode->parent!=NULL){
	  T->startNode=T->startNode->parent;
  }
//   printf("\n-----------------%d-----------\n",Min(T));
}

// put your time complexity for RemoveMin() here
HeapNode *RemoveMin(BinomialHeap *T)
{
  if(T->startNode == NULL){
	  printf("EMPTY BINOMIAL HEAP");
	  return NULL;
  }
  if(T->size==1){
	  HeapNode *ptr = T->startNode;
	  T->startNode =NULL;
	  T->size--;
	  return ptr;
  }
  HeapNode *temp = T->startNode;
  HeapNode *min = T->startNode;
//   int min = temp->key; 
  while (temp!=NULL)
  {
	  if(temp->key<min->key){
		  min =temp;
	  }
	  temp = temp->sibling;
  }
  temp = T->startNode;
  BinomialHeap *R = newHeap();
  if(min==T->startNode){
	  if(min->child!=NULL){
		  T->startNode = T->startNode->sibling;
		  R->startNode = min->child;
		  T->startNode = bin_heap_union(T,R);
		  T->size--;
		  return min;
	  }
	  else
	  {
		  T->startNode = T->startNode->sibling;
		  min->sibling=NULL;
		  T->size--;
		  adjust_binomial_heap(T->startNode,T->startNode->sibling);
		  return min;
	  }
  }
  
  while(temp->sibling!=min){
    	temp = temp->sibling;
  }
  temp->sibling = min->sibling;
  min->sibling = NULL;
//   printf("\n--%d,%d--\n",temp->key,temp->sibling->key);
  R->startNode = min->child;
  T->size--;
  T->startNode = bin_heap_union(T,R);
  return min;
}


int Min(BinomialHeap *T)
{
  // put your code here
  if(T->startNode==NULL){
	  return -1;
  }
  HeapNode *temp = T->startNode;
  int min = temp->key; 
  while (temp!=NULL)
  {
	  if(temp->key<min){
		  min =temp->key;
	  }
	  temp = temp->sibling;
  }
	return min;
}

void print_bin_heap(HeapNode * n){
	if(n==NULL){
		return;
	}
	printf("%d, %d, %d, %d, %d -> %d\n",n->TaskName,n->key,n->Dline,n->Etime,n->Rtime,n->degree);
	print_bin_heap(n->child);
	print_bin_heap(n->sibling);
}
// put your time complexity analysis for MyTaskScheduler here
int TaskScheduler(char *f1, char *f2, int m )
{
	FILE *f = fopen(f1,"r");
	if(f==NULL){
		printf("FILE could not be opened, Please check file name , location");
		exit(1);
	}
	char vi[100],ci[100],ri[100],di[100];
	BinomialHeap *releaseBHeap = newHeap();
	BinomialHeap *deadlineBHeap = newHeap();
	BinomialHeap *scheduleBHeap = newHeap();
	int scan = fscanf(f,"%s %s %s %s",vi,ci,ri,di);
	while(scan==4){
		int v,c,r,d;
		v = atoi(vi);
		c = atoi(ci);
		r = atoi(ri);
		d = atoi(di);
		if((v==0 && strcmp(vi,"0")!=0) || (c==0&& strcmp(ci,"0")!=0) || (r==0&& strcmp(ri,"0")!=0) || (d==0&& strcmp(di,"0")!=0))
		{
			printf("Invalid Input\nExiting............\n");
			exit(1);
		}
		if(c<1 || r<0 || d<1){
			printf("Parameters are Invalid, Please check lower range\nExiting..........\n");
			exit(1);
		}
		// printf("%d , %d , %d , %d\n",v,c,r,d);
		Insert(releaseBHeap,r,v,c,r,d);
		// Insert(deadlineBHeap,d,v,c,r,d);
		scan = fscanf(f,"%s %s %s %s",vi,ci,ri,di);
	}
	
	if(scan !=-1 ){
		printf("\nInvalid Input\nExiting............\n");
		exit(1);
	}
	printf("TaskName, key, Deadline, Execution Time, Release Time -> Degree\n");
	print_bin_heap(releaseBHeap->startNode);
	printf("\n\nFinding Top 3 and Removing them");
	printf("\nMinimum = %d\n",Min(releaseBHeap));
	printf("Removing.......\n");
	HeapNode *n=RemoveMin(releaseBHeap);
	// print_bin_heap(releaseBHeap->startNode);
	printf("\nMinimum = %d\n",Min(releaseBHeap));
	printf("Removing.......\n");
	n=RemoveMin(releaseBHeap);
	// print_bin_heap(releaseBHeap->startNode);
	printf("\nMinimum = %d\n",Min(releaseBHeap));
	printf("Removing.......\n");
	n=RemoveMin(releaseBHeap);
	// print_bin_heap(releaseBHeap->startNode);
	FILE *out = fopen(f2,"w");
	if(out==NULL){
		printf("\nCOULD Not open file for writing.........\nExiting.............\n");
		exit(1);
	}
	// int x =0 ;
	// while(x<m){
	// 	Insert(scheduleBHeap,0,x+1,0,0,0);
	// 	x++;
	// }
	// int time = 0;
	// while(releaseBHeap->startNode!=NULL){
	// 	if(Min(releaseBHeap)<=time && Min(scheduleBHeap)<=time){
	// 	while(Min(releaseBHeap)<=time && Min(releaseBHeap)!=-1 && Min(scheduleBHeap)<=time){
	// 		HeapNode *minNode = RemoveMin(releaseBHeap);
	// 		Insert(deadlineBHeap,minNode->Dline,minNode->TaskName,minNode->Etime,minNode->Rtime,minNode->Dline);
	// 	}
	// 	}
	// 	else{
	// 		time++;
	// 		continue;
	// 	}
	// 	while(deadlineBHeap->startNode!=NULL){
	// 		HeapNode *currTask = RemoveMin(deadlineBHeap);
	// 		HeapNode *schedulePoint = RemoveMin(scheduleBHeap);

	// 		if(currTask->Dline < time + currTask->Etime){
	// 			fprintf("%d Core%d %d.........",currTask->TaskName, time);
	// 			return 0;
	// 		}
			

		// }
		
		
	// }
	// put your code here
	fclose(f);
	fclose(out);
	return 0;
}

int main() //sample main for testing 
{ int i;
  i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 4 cores */
//   i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
//   if (i==0) printf("No feasible schedule!\n");
//   /* There is no feasible schedule on 3 cores */
//   i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
//   if (i==0) printf("No feasible schedule!\n");
//   /* There is a feasible schedule on 5 cores */
//   i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
//   if (i==0) printf("No feasible schedule!\n");
//   /* There is no feasible schedule on 4 cores */
//   i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
//   if (i==0) printf("No feasible schedule!\n");
//   /* There is no feasible schedule on 2 cores */
//   i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
//   if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 2 cores */
 return 0; 
}

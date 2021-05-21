// Author: Sidhant Arora
// Student ID:	z5281441
// Platform: Mac
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data structures representing DLList

// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item 
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList()
{
	DLList *L;

	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

DLList *insertDLList(DLList *L, DLListNode *new){
	if(L->size==0){
		L->size=1;
		L->first=new;
		L->last=new;
		return L;
	}
	L->last->next = new;
	new->prev = L->last;
	L->last = new;
	L->size++;
	return L;
}

// create a DLList from a text file
// put your time complexity analysis for CreateDLListFromFileDlist() here : O(n) where n is the input size
DLList *CreateDLListFromFileDlist(const char *filename)
{
 // put your code here
	if(strcmp(filename,"stdin")==0){
		DLList *l = newDLList();
		int val;
		char str[BUFSIZ];
		while(scanf("%s",str)==1)
		{	
			if(strcmp(str,"end")==0){
				break;
			}
			val = atoi(str);
			//printf("%d",val);
			if(val==0 && strcmp(str,"0")!=0){
				exit(1);
			}
			DLListNode *newNode = newDLListNode(val);
			l = insertDLList(l,newNode);
		}
		return l;
	}
	FILE *fp = fopen(filename,"r");
	if (fp==NULL){
		printf("Invalid input!");
		exit(1);
	}
	DLList *l = newDLList();
	int val;
	char str[BUFSIZ];
	while(fscanf(fp,"%s",str)==1)
	{
		val = atoi(str);
		//printf("%d",val);
		if(val==0 && strcmp(str,"0")!=0){
			printf("Invalid input!");
			exit(1);
		}
		DLListNode *newNode = newDLListNode(val);
		l = insertDLList(l,newNode);
	}
	return l;
	//printf("\n");
}

// clone a DLList
// put your time complexity analysis for cloneList() here : O(n)
DLList *cloneList(DLList *u)
{
 if (u==NULL){
	 return newDLList();
 }
 DLList *l = newDLList();
 if(u->size==0){
	 return l;
 }
 
  DLListNode *ptr = u->first;
  while(ptr!=NULL){
	  DLListNode *n = newDLListNode(ptr->value); 
	  l=insertDLList(l,n);
	  ptr = ptr->next;
  }
 
 return l;
  
}

int isPresent(DLList *l,int val){
	DLListNode *ptr = l->first;
	while (ptr!=NULL){
		if(ptr->value == val){
			return 1;
		}
		ptr=ptr->next;
	}
	return 0;
}

// compute the union of two DLLists u and v
// put your time complexity analysis for Union() here :O(nm) : where n and m are length of the list
DLList *setUnion(DLList *u, DLList *v)
{
 
 DLList *l;
 if(u==NULL && v==NULL){
	 return newDLList();
 }
 l=cloneList(u);
 if(v==NULL){
	 return l;
 }
 DLListNode *ptr = v->first;
 while(ptr!=NULL){
	 if(!isPresent(u,ptr->value)){
		 DLListNode *node = newDLListNode(ptr->value);
	 	 l = insertDLList(l,node);
	 }
	 ptr=ptr->next;
 }
 return l;
}

// compute the insection of two DLLists u and v
// put your time complexity analysis for intersection() here :O(nm) : where n and m are length of the list
DLList *setIntersection(DLList *u, DLList *v)
{
  if(u==NULL || v==NULL){
	 return newDLList();
 }
  DLList *l = newDLList();
  DLListNode *ptr = u->first; 
  while(ptr!=NULL){
	 	if(isPresent(v,ptr->value)){
	 	 DLListNode *node = newDLListNode(ptr->value);
	 	 l = insertDLList(l,node);
  		}
		ptr=ptr->next;
  }
  
  return l;
}

// free up all space associated with list
// put your time complexity analysis for freeDLList() here: O(n)
void freeDLList(DLList *L)
{
// put your code here
if(L==NULL || L->size==0){
	return;
}
DLListNode *ptr;
ptr = L->first;
while(ptr!=NULL){
	L->first = L->first->next;
	free(ptr);
	ptr = L->first;
}	

}


// display items of a DLList
// put your time complexity analysis for printDDList() here: O(n)
void printDLList(DLList *u)
{
//  put your code here
 if (u==NULL || u->size==0){
	 return;
 }
 DLListNode *ptr = u->first;
 do{
	 printf("%d\n",ptr->value);
	 ptr=ptr->next;
 }while(ptr!=NULL);
}

int main()
{
 DLList *list1, *list2, *list3, *list4;

 list1=CreateDLListFromFileDlist("File1.txt");
 printDLList(list1);

 list2=CreateDLListFromFileDlist("File2.txt");
 printDLList(list2);
 list3=setUnion(list1, list2);
 printDLList(list3);

 list4=setIntersection(list1, list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 printf("please type all the integers of list1\n");
 list1=CreateDLListFromFileDlist("stdin");
 
 printf("please type all the integers of list2\n");
 list2=CreateDLListFromFileDlist("stdin");
 
 list3=cloneList(list1);
 printDLList(list3);
 list4=cloneList(list2);
 printDLList(list4);
 
 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 return 0; 
}

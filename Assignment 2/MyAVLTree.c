#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}
AVLTreeNode *Search(AVLTree*,int,int);
int InsertNode(AVLTree*, int, int);
void PrintAVLTree(AVLTree*);

// data type for array for UNION and INTERSECTION

typedef struct {
	int k,v,size;
} Arr;

// MY CUSTOM FUNCTIONS:

AVLTreeNode *checkBalance(AVLTreeNode*);
void restructure(AVLTreeNode*);
void LL(AVLTreeNode*);
void RR(AVLTreeNode*);
void LR(AVLTreeNode*);
void RL(AVLTreeNode*);

Arr *getElements(AVLTreeNode *n,Arr array[],int *i){
	if(n==NULL){
		return array;
	}
	array = getElements(n->left,array,i);
	// printf("%d\n",n->key);
	array[*i].k = n->key;
	array[*i].v = n->value; 
	*i=*i+1;
	array[*i].size=-1;
	return getElements(n->right,array,i);
}

Arr *treeToArr(AVLTree *T){
	
	Arr *array;
	array = malloc(sizeof(Arr)*T->size);
	int i=0;
	// printf("%p",&i);
	// printf("--------------------------------\n");
	array= getElements(T->root,array,&i);
	array[0].size = i;
	// printf("--------------------------------\n");
	return array;
}
AVLTreeNode *ArrtoTree(Arr *n,int start,int end){
	if(start>end || n==NULL){
		return NULL;
	}
	int mid = (start+end)/2;
	AVLTreeNode *t= newAVLTreeNode(n[mid].k,n[mid].v);
	// printf("%d\n",n[mid].v);
	t->left = ArrtoTree(n,start,mid-1); 
	t->right = ArrtoTree(n,mid+1,end); 
	start++;end--;
	return t;
}

//searching in the tree
AVLTreeNode *searchNode(AVLTreeNode *n,int k,int v){
	if(n==NULL){
		return NULL;
	}
	if(n->key==k && n->value==v){
		return n;
	}
	else if (n->key>k)
	{
		return searchNode(n->left,k,v);
	}
	else if(n->key<k){
		return searchNode(n->right,k,v);
	}
	else if (n->key==k && n->value!=v)
	{
		if(n->value>v)
			return searchNode(n->left,k,v);
		else if (n->value<v)
		    return searchNode(n->right,k,v);
	}
}

int maxHeight(AVLTreeNode* a,AVLTreeNode* b){
	if(a==NULL && b==NULL){
		return -1;
	}
	else if(a==NULL && b!=NULL){
		return b->height;
	}
	else if(b==NULL && a!=NULL){
		return a->height;
	}
	else if(a->height>b->height){
		return a->height;
	}
	return b->height;
}
//updates heights of nodes in the tree
void updateHeight(AVLTreeNode* n){
	// printf("\n%dcccc\n",n->key);
	// printf("%d,%d:%d\n",n->key,n->height,n->value);
	if(n->left==NULL && n->right==NULL){
		n->height=0;
		
		return;
	}
	if(n->left){
	updateHeight(n->left);
	}
	// printf("%p\n",n->right);
	if(n->right){
		// printf("right:%d\n",n->key);
	updateHeight(n->right);
	}
	n->height = maxHeight(n->left,n->right) + 1;
	// printf("%d,%d:%d\n",n->key,n->height,n->value);
	return;
	
}

// updates the root in case the root value for the tree is changed
AVLTreeNode *updateRoot(AVLTreeNode *root){
	if(root->parent!=NULL){
		root=root->parent;
	}
	return root;
}

void rotateRight(AVLTreeNode *n){
 AVLTreeNode *y;

 y = n->left;
 y->parent=n->parent;
 if(n->parent){
	 if(n->parent->left==n){
		 n->parent->left = y;
	 }
	 else{
		 n->parent->right=y;
	 }	 
 }
 n->parent = y; 
 n->left = y->right;
 y->right = n;
 n->height = maxHeight(n->left,n->right) + 1;
 y->height = maxHeight(y->right,y->left) + 1;
}

void rotateLeft(AVLTreeNode *n){
 AVLTreeNode *y;
//  printf("\n%d\n",n->key);
 y = n->right;
 y->parent=n->parent;
 if(n->parent){
	 if(n->parent->left==n){
		 n->parent->left = y;
	 }
	 else{
		 n->parent->right=y;
	 }	 
 }
 n->parent = y; 
 n->right = y->left;
 y->left = n;
 n->height = maxHeight(n->left,n->right) + 1;
 y->height = maxHeight(y->right,y->left) + 1;
}

void LR(AVLTreeNode *n){
	rotateLeft(n->left);
	rotateRight(n);
}
void RL(AVLTreeNode *n){
	rotateRight(n->right);
	rotateLeft(n);

}
// checks if balance for tree is off and returns the node if imbalanced
AVLTreeNode *checkBalance(AVLTreeNode *n){
	// printf("\n Checking :%d\n",n->Key);
	AVLTreeNode *child;
	if(n==NULL){
		return NULL;
	}
	// printf("Cheecking1: %d\n",n->key);
	if (n->left==NULL && n->right==NULL){
		// printf("NULL2");
		return NULL;
	}
	if(n->left!=NULL){
		child = checkBalance(n->left);
		if(child!=NULL){
			return child;
		}
	}
	if(n->right!=NULL){
		child = checkBalance(n->right);
		
		if(child!=NULL){
		// printf("%d",child->key);
			return child;
		}
	}
	if(n->left==NULL && n->right->height>0){
		return n;
	}
	if(n->right==NULL && n->left->height>0){
		// printf("Check : %d\n",n->key);
		return n;
	}
	if(n->right && n->left && abs(n->right->height - n->left->height) >1){
		return n;
	}
	return NULL;
}

// ADDs to the tree BST way
AVLTreeNode *addtoTree(AVLTreeNode *n,int k,int v){
	if(n==NULL){
		return newAVLTreeNode(k,v);
	}
	if (n->key>k)
	{
		n->left = addtoTree(n->left,k,v);
		if(n->left->height+1>n->height)
			n->height=n->left->height+1;  
		n->left->parent = n;
	}
	else if(n->key<k){
		n->right =  addtoTree(n->right,k,v);
		if(n->right->height+1 > n->height)
			n->height=n->right->height+1;
		n->right->parent = n;
	}
	else if (n->key==k && n->value!=v)
	{
		if(n->value>v){
			n->left = addtoTree(n->left,k,v);
			if(n->left->height+1>n->height)
				n->height=n->left->height+1;
			n->left->parent = n;
		}
		else if (n->value<v){
			n->right =  addtoTree(n->right,k,v);
			if(n->right->height+1 > n->height)
				n->height=n->right->height+1;
			n->right->parent = n;
		}
	}
	return n;
}

void restructure(AVLTreeNode *n){
	AVLTreeNode *ptr = n;
	if(((ptr->left == NULL) && (ptr->right)) || ((ptr->right) &&(ptr->right->height>ptr->left->height))){
		ptr = ptr->right;
	
		if(((ptr->left == NULL) && (ptr->right)) || ((ptr->right) && (ptr->right->height>ptr->left->height)))
			{rotateLeft(n);return;}
		if(((ptr->right == NULL) && (ptr->left)) || ((ptr->left) && (ptr->left->height>ptr->right->height)))
			{RL(n);return;}
		else {
			RL(n);
		}
	
	}
	else if(((ptr->right == NULL) && (ptr->left)) || ((ptr->left) && (ptr->left->height>ptr->right->height))){
		ptr = ptr->left;
		if(((ptr->right == NULL) && (ptr->left)) || ((ptr->left) && (ptr->left->height>ptr->right->height)))
			{rotateRight(n);return;}
		if(((ptr->left == NULL) && (ptr->right)) || ((ptr->right)&&(ptr->right->height>ptr->left->height)))
			{LR(n);return;}
		else{
			LR(n);return;
		}
	}
	
}

// Complexity : O(nlogn)
AVLTree *CreateAVLTree(const char *filename)
{

 int k,v,j;
 AVLTree *tree = newAVLTree();	
 if (strcmp(filename,"stdin")==0){
	 
	 if(!scanf("(%d, %d)",&k,&v)){
		 exit(1);
	 }
	 j=InsertNode(tree,k,v);
	 do{ 
		 char c;
		 scanf("%c",&c);
		 if(c=='('){
			 scanf("%d,%d)",&k,&v);
			 j=InsertNode(tree,k,v);
		 }
		 else if (c==' '|| c=='\n')
			 continue;
		 else
			 break;
	 }while(1);
	 }
 
 else{
	 FILE *fp = fopen(filename,"r");
	 if(fp == NULL){
		 exit(1);
	 }
	 if(!fscanf(fp,"(%d, %d)",&k,&v)){
		 exit(1);
	 }
	 j=InsertNode(tree,k,v);
	 do{
		 
		 char c;
		 fscanf(fp,"%c",&c);
		 if(c=='('){
			 fscanf(fp,"%d,%d)",&k,&v);
		 	 j=InsertNode(tree,k,v);
	 	 }
		 else if (c==' '||c=='\n'|| c=='\r')
			 continue;
		 else{
			 break;}
	 }while(!feof(fp));
	 fclose(fp);
 }
 return tree;
  
}
void cloneNodes(AVLTreeNode *n,AVLTree *clone){
	if(n==NULL){
		return;
	}
	clone->root=addtoTree(clone->root,n->key,n->value);
	cloneNodes(n->left,clone);
	cloneNodes(n->right,clone);
}
// // Complexity : O(n)
AVLTree *CloneAVLTree(AVLTree *T)
{
 if(T==NULL){
	 return NULL;
 }
 AVLTree *clone = newAVLTree();
 if(T->root==NULL){
	 return clone;
 }
 cloneNodes(T->root,clone); 
 return clone;
}
 
// // Complexity : O(m+n)
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
	
	Arr *a1 = treeToArr(T1);
	Arr *a2 = treeToArr(T2);
	
	Arr a3[T1->size+T2->size];
	
	int i=0,j=0,k=0;
	int counter;
	while(i<T1->size || j<T2->size){
		if(a1[i].k<a2[j].k){
			a3[k].k = a1[i].k;
			a3[k].v = a1[i].v;
			i++;k++;
		}
		if(a2[j].k<a1[i].k){
			a3[k].k = a2[j].k;
			a3[k].v = a2[j].v;
			j++;k++;
		}
		if(a2[j].k==a1[i].k){
			if(a1[i].v<a2[j].v){
				a3[k].k = a1[i].k;
				a3[k].v = a1[i].v;
				i++;k++;
			}
			if(a2[j].v<a1[i].v){
				a3[k].k = a2[j].k;
				a3[k].v = a2[j].v;
				j++;k++;
			}
			else if(a2[j].v==a1[j].v){
				a3[k].k = a2[j].k;
				a3[k].v = a2[j].v;
				j++;k++;i++;
			}
		}
	}
	// for(i=0;i<k;i++){
	// 	printf("%d,%d\t",a3[i].k,a3[i].v);
	// }
	// printf("\n");
	AVLTree *T3 = newAVLTree();
	T3->size = k;
	T3->root = ArrtoTree(a3,0,k-1);
	updateHeight(T3->root);
	// PrintAVLTree(T3);
	return T3;
}
 
// // Complexity : O(m+n)
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
	 //put your code here
	Arr *a1 = treeToArr(T1);
	Arr *a2 = treeToArr(T2);
	
	Arr a3[T1->size];
	
	int i=0,j=0,k=0;
	int counter;

	while(i<T1->size){
		if(a1[i].k<a2[j].k){
			i++;
		}
		if(a2[j].k<a1[i].k){
			j++;
		}
		if(a2[j].k==a1[i].k){
			if(a1[i].v<a2[j].v){
				i++;
			}
			if(a2[j].v<a1[i].v){
				j++;
			}
			else if(a2[j].v==a1[j].v){
				a3[k].k = a2[j].k;
				a3[k].v = a2[j].v;
				j++;k++;i++;
			}
		}
	}
	printf("\n");
	AVLTree *T3 = newAVLTree();
	T3->size = k;
	T3->root = ArrtoTree(a3,0,k-1);
	updateHeight(T3->root);
	// PrintAVLTree(T3);
	return T3;

}

// // Complexity : O(log(n))    
int InsertNode(AVLTree *T, int k, int v)
{
	if(Search(T,k,v)!=NULL){
		return 0;
	}
	T->root = addtoTree(T->root,k,v);
	T->size++;
	AVLTreeNode *n;
	
	n=checkBalance(T->root);
	
	while (n!=NULL)
	{
		restructure(n);
		T->root = updateRoot(T->root);
		updateHeight(T->root);
		n = checkBalance(T->root);
	}
	return 1;
}

AVLTreeNode *getMin(AVLTreeNode* n ){

	if(n->left==NULL){
		return n;
	}
	return getMin(n->left);
}
AVLTreeNode *getMax(AVLTreeNode* n ){

	if(n->right==NULL){
		return n;
	}
	return getMin(n->right);
}

void delete(AVLTreeNode *t,int k,int v){
AVLTreeNode* n = searchNode(t,k,v);
//  printf("del: %d",n->value);
 if(n->left==NULL && n->right==NULL){
	 	if (n->parent!=NULL){
		 if(n->parent->right == n){
			 n->parent->right =NULL;
			 free(n);
			 return ;
		 }
		if(n->parent->left == n){
			 n->parent->left =NULL;
			 free(n);
			 return;
		}
	 } 
 }
 if(!n->left){
	if (n->parent!=NULL){
		 if(n->parent->right == n){
			 n->parent->right =n->right;
		 }
		 if(n->parent->left == n){
			 n->parent->left =n->right;
		 }
		n->right->parent = n->parent;
		free(n); 
		return;
	 }
	 else{
		 n->value = n->right->value;
		 n->key = n->right->key;
		 delete(n->left,n->key,n->value);
	 }
	
	
 }
 if(!n->right){
	if (n->parent!=NULL){
		 if(n->parent->right == n){
			 n->parent->right =n->left;
		 }
		 if(n->parent->left == n){
			 n->parent->left =n->left;
		 }
	     n->left->parent = n->parent;
		free(n); 
		return; 
	 }
	 else{
		 n->value = n->left->value;
		 n->key = n->left->key;
		 delete(n->left,n->key,n->value);
	 }
	
 }
 else {
	 AVLTreeNode *ptr = getMin(n->right);
	 n->key=ptr->key;
	 n->value=ptr->value;
	 delete(n->right,n->key,n->value);
	 return;
}
}

// // Complexity : O(log(n))
int DeleteNode(AVLTree *T, int k, int v)
{
 if(searchNode(T->root,k,v)==NULL){
	 return 0;
 	}
 delete(T->root,k,v);
 T->root = updateRoot(T->root);
 updateHeight(T->root);
 T->size--;
AVLTreeNode *n;
n=checkBalance(T->root);

while (n!=NULL)
{
	// printf("Del3: %d\n",n->key);
	restructure(n);
	// PrintAVLTree(T);
	T->root = updateRoot(T->root);
	updateHeight(T->root);
	n = checkBalance(T->root);
}

 
return 1;
}



// // Complexity : O(logn)
AVLTreeNode *Search(AVLTree *T, int k, int v){
	if (T->root==NULL){
		return NULL;
	}
	return searchNode(T->root,k,v);
}
void freeAVLTreeNodes(AVLTreeNode *n){
	if(n==NULL){
		return;}
	
	if(n->left==NULL && n->right==NULL){
		free(n);
		return;
	}
	freeAVLTreeNodes(n->left);
	freeAVLTreeNodes(n->right);
	free(n);
	return;
}
// Complexity : O(n)
void FreeAVLTree(AVLTree *T){
if (T->root==NULL){
	return;
}
else{
	freeAVLTreeNodes(T->root);
}
}


void printTreeNodes(AVLTreeNode *n){
	if(n==NULL){
		return;
	}
	printTreeNodes(n->left);
	printf("(%d, %d), %d\n",n->key,n->value,n->height);
	printTreeNodes(n->right);
}
//  // Complexity : O(n)
void PrintAVLTree(AVLTree *T){
 printTreeNodes(T->root);
 printf("\n");
}


int main() //sample main for testing 
{ int i,j;
 AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
 AVLTreeNode *node1;
 
 tree1=CreateAVLTree("stdin");
 PrintAVLTree(tree1);
 FreeAVLTree(tree1);
 //you need to create the text file file1.txt
//  to store a set of items without duplicate items
 tree2=CreateAVLTree("file1.txt"); 
 PrintAVLTree(tree2);
//  printf("\n-----------------\n");
 tree3=CloneAVLTree(tree2);
 PrintAVLTree(tree3);
 FreeAVLTree(tree2);
 FreeAVLTree(tree3);
 //Create tree4 
 tree4=newAVLTree();
 j=InsertNode(tree4, 10, 10);
 for (i=0; i<15; i++)
  {
   j=InsertNode(tree4, i, i);
//    PrintAVLTree(tree4);
   if (j==0) printf("(%d, %d) already exists\n", i, i);
  }
  PrintAVLTree(tree4);
  node1=Search(tree4,20,20);
  if (node1!=NULL)
    printf("key= %d value= %d\n",node1->key,node1->value);
  else 
    printf("Key 20 does not exist\n");
  
  for (i=17; i>0; i--)
  {
    
	j=DeleteNode(tree4, i, i);
	if (j==0) 
	  printf("Key %d does not exist\n",i); 
    PrintAVLTree(tree4);
  }
//   PrintAVLTree(tree4);
 FreeAVLTree(tree4);
//  Create tree5
 tree5=newAVLTree();
 j=InsertNode(tree5, 6, 25);
 j=InsertNode(tree5, 6, 10);
 j=InsertNode(tree5, 6, 12);
 j=InsertNode(tree5, 6, 20);
 j=InsertNode(tree5, 9, 25);
 j=InsertNode(tree5, 10, 25);
 PrintAVLTree(tree5);
//  //Create tree6
 tree6=newAVLTree();
 j=InsertNode(tree6, 6, 25);
 j=InsertNode(tree6, 5, 10);
 j=InsertNode(tree6, 6, 12);
 j=InsertNode(tree6, 6, 20);
 j=InsertNode(tree6, 8, 35);
 j=InsertNode(tree6, 10, 25);
 PrintAVLTree(tree6);
 tree7=AVLTreesIntersection(tree5, tree6);
 tree8=AVLTreesUnion(tree5,tree6);
 PrintAVLTree(tree7);
 PrintAVLTree(tree8);
 return 0; 
}

#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#define ep '~'								//定义空符号为~
#define op set<int>::iterator				//定义set集指针
#define LINE 10
#define MAX 100
using namespace std;
struct List{
	int data[LINE];
	int length;
}L;
void InsertList(List & L,int x)
 {   
	int i;
	for(int t=0;t<L.length;t++){
		if(x<=L.data[0]) {
			i=1;
			break;
		}
		else if(x>=L.data[L.length-1]) {
			i=L.length+1;
			break;
		}
		else if(x>=L.data[t]&&x<L.data[t+1])  {
			i=t+2;
			break;
		}
	}
	if(i<1 || i>L.length+1){
       cout<<"错误"<<endl;
}
    else{
		for(int j=L.length-1;j>=i-1;j--)
               L.data[j+1]=L.data[j];
         L.data[i-1]=x;
          L.length++;   
}
}

struct LNode{
  char data;
   LNode *next;
};
LNode *create()
 {
    char  ch;
    LNode *p,*r,*head; 
    head=r=NULL;
      cin>>ch;
    while(ch!='$'){
        p=new LNode;
        p->data=ch;
        if(head==NULL)
              head=p;
else  r->next=p;
 r=p;
cin>>ch;
}
if(r!=NULL)
   r->next=NULL;
   return head;
}

void print(LNode * h)
{  
	LNode *p=h;
    while(p!=NULL)
     { 
		cout<<p->data<<' ';
        p=p->next; 
   }
         cout<<endl;
 }
LNode *insertnode(LNode * head,char x)
    {
       LNode  * q,* i;
	   i=head->next;
           q=new LNode;
           q->data=x;
		   if(x<=head->data){
		   q->next=head;
		   head=q;
		   }
	   else {
		   while(i->next&&i->next->data<x)
		   i=i->next;
		   if(i->next==NULL){
          i->next=q;
	  q->next=NULL;
		   }
		    else{
        q->next=i->next;
         i->next=q;
	  }
      }
	   return head;
}

struct RNode{
  char data;
   RNode *next;
};
RNode * create()
{ 
	RNode *r,*h;
   char ch;
   h=new RNode;
   r=h;
  cin>>ch;
while(ch!='$'){
    r->next=new RNode;
    r=r->next;
    r->data=ch;
   cin>>ch;
}
   r->next=NULL;
   return  h;
}


void print(RNode * h)
{  
	RNode *p=h;
    while(p!=NULL)
     { 
		cout<<p->data<<' ';
        p=p->next; 
   }
         cout<<endl;
 }

RNode *insertnode(RNode * head,char x)
    {
       RNode  * q,* i,*h;
	   h=head->next;
	   i=h->next;
           q=new RNode;
           q->data=x;
		   if(x<=h->data){
		   q->next=h;
		   head->next=q;
		   }
	   else {
		   while(i->next&&i->next->data<x)
		   i=i->next;
		   if(i->next==NULL){
          i->next=q;
	  q->next=NULL;
		   }
		    else{
        q->next=i->next;
         i->next=q;
	  }
      }
	   return head;
}
template <class T>
class Li{
    T *p;
    int len;
    int max;
    public:
    Li(){
	p=NULL;
	len=0;
	max=0;
    }
    ~Li(){
	if(p!=NULL) 
	    delete[] p;
    }
    void insert(int y,T x){
	if(y<0||y>len){
	    cout<<"error"<<endl;
		return;
	}
	if(len>=max){
	    max+=len;
	    T* q=new T(max);
	    if(p!=NULL){
		int i;
		for(i=0;i<len;i++)
		    q[i]=p[i];
		delete[] p;
	    }
	    p=q;
	}
	for(int i=len-1;i>=y;i--)
	    p[i+1]=p[i];
	p[y]=x;
	len++;
    }
    void erase(int y){
	if(y<0||y>=len){
	    cout<<"NOT ERASED"<<endl;
		return;
	}
	for(int i=y+1;i<=len-1;i++)
	    p[i-1]=p[i];
	len--;
    }

    void clear(){
	if(p!=NULL)
	    delete[] p;
	p=NULL;
	len=0;
	max=0;
    }
    void show(){
	if(p==NULL){
	    cout<<"is empty"<<endl;
	    return ;
	}

	for(int i=0;i<len;i++)
	    cout<<p[i]<<' ';
cout<<endl;
    }
};
struct Node{
	int data;
	Node * next;
};
Node * create(){
Node * rear;
rear=new Node;
 rear->next=rear;
 return rear;
}
Node * insert(Node * rear,int x){
Node * r;
r=new Node;
 r->data=x;
 r->next=rear->next;
 rear->next=r;
 rear=r;
return rear;
}
Node * exit(Node * rear){
	if(rear->next==rear){
  cout<<"队列为空"<<endl;
  return NULL;
	}
else{
	Node *p=rear->next->next;
	int y;
	y=p->data;
	rear->next->next=p->next;
	delete p;
	return rear;
}
}
void print(Node * rear)
{  
	Node *p=rear->next->next;
    while(p!=rear->next)
     { 
		cout<<p->data<<' ';
        p=p->next; 
   }
         cout<<endl;
 }
struct triple{
       int i,j;
       int v;
    };
struct tripletable{
      triple  data[MAX];
      int  m,n,t;
      }tr1,tr2;
int tr3[MAX][MAX]={0},p,q;
void create(tripletable & tr){
	cin>>tr.m>>tr.n>>tr.t;
	for(int k=1;k<=tr.t;k++)
		cin>>tr.data[k].i>>tr.data[k].j>>tr.data[k].v;
}
void add(tripletable tr1,tripletable tr2){
	for(p=1;p<=tr1.t;p++){
       tr3[tr1.data[p].i][tr1.data[p].j]+=tr1.data[p].v;
	   }
		for(q=1;q<=tr2.t;q++){
				tr3[tr2.data[q].i][tr2.data[q].j]+=tr2.data[q].v;
			}
}
struct tnode{
	 int data;
	 tnode * next;
};
struct node{
      int data;
	  tnode * first;
};

void  create(node a[],int n,int e){ 
	int u,v;
	for(int i=0;i<n;i++){
     a[i].data=i+1;  
     a[i].first=NULL;
	}
  for(int i=1;i<=e;i++){
	  cin>>u>>v;
    tnode * p =new tnode;
     p->data=v;
     p->next=a[u-1].first;
     a[u-1].first=p;
   }
}
void add(node a[],int u,int v){
    tnode * p =new tnode;
     p->data=v;
     p->next=a[u-1].first;
     a[u-1].first=p;
}
void del(node  a[],int u,int v){
	tnode * t=a[u-1].first;
	if(t->data==v){
		a[u-1].first=t->next;
		delete t;
	}
	else{
	while(t->next->data!=v)
		t=t->next;
	tnode * q=t->next;
	t->next=q->next;
	delete q;
	}
}
void show(node a[],int n){
	tnode * p;
	for(int i=0;i<n;i++){
		cout<<a[i].data<<' ';
		p=a[i].first;
		while(p!=NULL){
			cout<<p->data<<' ';
			p=p->next;
		}
		cout<<endl;
	}
}	
int a[10][10],b[10],set[10],dist[10];
void create(int a[][10],int n,int e){
	int u,v,t;
	for(int p=0;p<n;p++)
		for(int q=0;q<n;q++)
			a[p][q]=MAX;
	for(int i=0;i<e;i++){
		cin>>u>>v>>t;
	a[u-1][v-1]=t;
	}
}
struct node{
	int data;
	node * lchild;
	node * rchild;
	int flag;
};
int max(int x,int y){
	return x>y?x:y;
}
char getonech(char a[]){
	static i;
    return a[i++];
}
void CreateBiTree(node *&p,char a[])
{  char ch;
    ch=getonech(a);
   if(ch!= '*')
   {   p=new node;
        p->data=ch;
        CreateBiTree(p->lchild,a);
        CreateBiTree(p->rchild,a);
   }
   else p=NULL;
}
void preorder(node * p){  
	if(p!=NULL){ 
		cout<<(char)p->data;               
      preorder(p->lchild);      
      preorder(p->rchild); 
	}
}
void inorder(node * p){
  if (p) {
	  inorder (p->lchild);
     cout<<(char)p->data;
	 inorder (p->rchild);
  }
}
void postorder(node * p)
{
  if (p) {
	  postorder (p->lchild);
  postorder (p->rchild);
     cout<<(char)p->data;
  }
}

int count(node * p)
    {
	int m;
    if(p){
      m=count(p->lchild);
      m+=count(p->rchild);
	if(!m) return 1;
	else return m; 
	}
	else return 0;
    }
int high(node * p){
	int n;
	if(p){
		n=max(high(p->lchild),high(p->rchild))+1;
		if(!n) return 1;
		else return n;
	}
	else return 0;
}
void change(node * &p){
	node * temp=NULL;
	if(p->lchild==NULL&&p->rchild==NULL) return;
	temp=p->lchild;
	p->lchild=p->rchild;
	p->rchild=temp;
	if(p->lchild) change(p->lchild);
	if(p->rchild) change(p->rchild);
}
void  postorder1(node * t){                     //非递归后序遍历
     stack<node *> s;
	 node * p=t;
	 node * pre=NULL;
	 while(p||s.size()!=0){
		 while(p){
			 s.push(p);
			 p=p->lchild;
		 }
		 p=s.top();
		 if(p->rchild==NULL||p->rchild==pre){
			 cout<<(char)p->data;
			 s.pop();
			 pre=p;
			 p=NULL;
	 }
		 else{
			 p=p->rchild;
		 }
	 }
}

int main(){
}




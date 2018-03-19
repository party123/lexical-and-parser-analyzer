#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#define ep '~'								//定义空符号为~
#define op set<int>::iterator				//定义set集指针
const int mm = 30;
const int mlen = 100;
const int mlr = 50;
using namespace std;
map<char, int> sint;
map<int, char> ints;
set<char> first[mlen];			//FIRST集
char * l,* r;
char initial;					//初始符
int count;						//符号数
bool ter[mlen];					//是否为终结符
bool empty[mm];					//是否能推出空

void init() {					//初始化
    sint.clear();
	ints.clear();
	memset(ter,false,sizeof(ter));
	memset(empty,false,sizeof(empty));
	count = 0;
}

bool S(char a) {				//判断是否为非终结符
	if(a>='A'&&a<='Z') return true;
	else return false;
}
class tt {					//产生式类
	public:
		char l;				//左部非终结符
		char * r;			//产生式右部字符串
		tt(char l, char * r) {
			this->l=l;
			this->r=r;
		}
		void display() {
			cout<<l<<"->"<<r<<endl;
		}
		~tt() {
			if (r!=NULL)
				delete r;
		}
};
tt * list[50];

bool noempty(char * a) {							//右部不为空
	for (int i=0;i<strlen(a);i++) {
		if (S(a[i])==false&&a[i]!=ep) 
			return true;
	}
}

void tempty(int a) {								//左部能推出空
	for (int i=0;i<a;i++) {
		if (list[i]->r[0]==ep) {
			empty[sint[list[i]->l]]=true;			//empty表示表达式右部是否为空
		}
	}
	set<int> s;										//建立可推出空的非终结符
	for (int i=0;i<count;i++) {
		if (empty[i]==true) s.insert(i);
	}
	while (1) {
		int	ll=s.size();
		for (int i=1;i<a;i++) {
			if (empty[sint[list[i]->l]]==false) {		
				if (noempty(list[i]->r)==false) {		//右部不为空时判断右部是否能推出空
					bool t=true;
					for (int j=0;j<strlen(list[i]->r);j++) {
						if (empty[sint[list[i]->r[j]]]==false) {
							t=false;
							break;
						}
					}
					if (t==true) {
						empty[sint[list[i]->l]]=true;
						s.insert(list[i]->l);
					}
				}
			}
		}
		if (ll==s.size()) break;
	}
}

void firstset(int a) {										//构造first集
	int temp[mlen],f;
	for (int i=0;i<count;i++) 
		temp[i]=first[i].size();
	while(1){
		for (int i=0;i<count;i++) {							//遍历所有符号，构造每个符号的FIRST集					
			if (ter[i]==true) {								//终结符的FIRST集为自身
				first[i].insert(ints[i]);
				continue;
			}
			for (int j=1;j<a;j++) {							//非终结符的FIRST集求取过程
				if (list[j]->l==ints[i]) {					//所有产生式左部为当前扫描非终结时
					if (list[j]->r[0]==ep) continue;			//非终结符右部为空则continue
					for (int k=0;k<strlen(list[j]->r);k++) {	//右部不为空则找到第一个终结符
						char tmp=list[j]->r[k];
						if (ter[sint[tmp]]==true) {				//为终结符则加入
							first[i].insert(list[j]->r[k]);
							break;
						} 
						else {								//右部第一个为非终结符时将其FIRST集加入
							for (set<char>::iterator y=first[sint[tmp]].begin();y!=first[sint[tmp]].end();y++) {	
								first[i].insert(*y);
							}
							if (empty[sint[tmp]]==false){		//直到下一个不能推出空的非终结为止，否则继续循环
								break;
							}
						}
					}
				}
			}
		}
		f=0;
		for (int i=0;i<count;i++) {								//每个符号的FIRST集不再增加为止
			if (first[i].size()!=temp[i]) {
				f=1;
				break;
			}
		}
		if (f==0) break;
		for (int i=0;i<count;i++) temp[i]=first[i].size();
	}
}

void showfirstset() {								//FIRST集显示
	ofstream out;
	out.open("out.txt");
	cout<<"FIRST集:"<<endl;
//	out<<"FIRST集:"<<endl;
	for (int i=0;i<count;i++) {
		if(ter[i]==false){
			cout<<ints[i]<<": ";
//			out<<ints[i]<<": ";
		for (set<char>::iterator j=first[i].begin();j!=first[i].end();j++) {
			cout<<*j<<' ';
//			out<<*j<<' ';
		}
		if (empty[i]==true) {
			cout<<"~";
//			out<<"~";
		}
		cout<<endl;
//		out<<endl;
		}
	}
	out.close();
}
//字符串相等
bool strequal(char * s1, char * s2) {
	if (strlen(s1)!=strlen(s2)) return false;
	for (int i=0;i<strlen(s1);i++)
		if (s1[i]!=s2[i]) return false;
	return true;
}

//两个集合相等
bool setequal(set<char> s1, set<char> s2) {
	if (s1.size()!=s2.size()) return false;
	for (set<char>::iterator i=s1.begin();i!=s1.end();i++) {
		if (s2.find(*i)==s2.end()) 
			return false;
	}
	for (set<char>::iterator i=s2.begin();i!=s2.end();i++) {
		if (s1.find(*i)==s1.end()) 
			return false;
	}
	return true;
}

class lrt {						//项目集产生式
	public:
		char l;
		char * r;
		int dot;					//dot记录点的位置
		set<char> search;			//向前搜索符
		lrt() {}
		lrt(char l, char * r, int dot) {
			this->l=l;
			this->r=r;
			this->dot=dot;
			if (l=='$') search.insert('#');		//起始符的向前搜索符设为#
		}
		bool same(lrt * p) {				//相同核不同向前搜索符
			if (p->l!=l) return false;
			if (!strequal(p->r,r)) return false;		
			if (p->dot!= dot) return false;
			return true;
		}
		bool equal(lrt * p) {			//相等核相同向前搜索符
			if (p->l!=l) return false;
			if (strequal(p->r,r)==false) return false;
			if (p->dot!=dot) return false;
			if (setequal(p->search,search)==false) return false;
			return true;
		}	
};

class lrset {								//构造LR1项目集
	public:
		lrt * lr[mm];
		int num;							//项目集大小
		lrset() {
			num=0;
			for (int i=0;i<mm;i++) lr[i]=NULL;
		}
		lrset(lrt * a) {
			if (a==NULL||a->dot!=0) {
				cout<<"构建LR项目集错误!"<<endl;
			} 
			else {
				for (int i=0;i<mm;i++) lr[i]=NULL;
				lr[num++]=a;
			}
		}
		~lrset() {
			for (int i=0;i<mm;i++) {
				if (lr[i] != NULL) delete lr[i];
			}
		}

        void closure(int t_top) {			//求项目集闭包
			int presearch[mlen];
			int	presize =num;				
			for (int i=0;i<num;i++) 
				presearch[i]=lr[i]->search.size();
			while (1) {
				for (int i=0;i<num;i++) {
					if (lr[i]->dot==strlen(lr[i]->r)) continue;			//可规约状态
					int dot=lr[i]->dot;
					char tmp=lr[i]->r[dot];								//.的下一个字符
					if (ter[tmp]==false) {									//.后为非终结符时则将非终结符的第一个产生式加入
						for (int j=0;j<t_top;j++) {
							if (list[j]->l==tmp) {
								lrt * newlr = new lrt();
								newlr->l=tmp;
								newlr->dot=0;
								if (list[j]->r[0]==ep) newlr->r="";			//新产生式右部为空时
								else newlr->r=list[j]->r;

								set<char> tmpsearch;
								int in;
								for (in=lr[i]->dot+1;in<strlen(lr[i]->r);in++) {		//将非终结符后符号串的first集内容添加进搜索符集中
									char wait=lr[i]->r[in];
									for (set<char>::iterator it=first[sint[wait]].begin();it!=first[sint[wait]].end();it++) {
										tmpsearch.insert(*it);
									}
									if (empty[sint[wait]]==false)	break;
								}
								if (in==strlen(lr[i]->r)) {						//非终结符后的符号串全为空时将原来产生式的搜索符集加入
									for (set<char>::iterator it=lr[i]->search.begin();it!=lr[i]->search.end();it++) {
										tmpsearch.insert(*it);
									}
								}
								for (set<char>::iterator it=tmpsearch.begin();it!=tmpsearch.end();it++) {
									newlr->search.insert(*it);
								}

								bool t=false;
								for (int k=0;k<num;k++) {								//项目集的去重与合并
									if (newlr->same(lr[k])&&!newlr->equal(lr[k])) {		//产生式相同项目集不同时合并	
										set<char>::iterator it;
										for (it=newlr->search.begin();it!=newlr->search.end();it++) {
											lr[k]->search.insert(*it);
										}
										t=true;
										break;
									}
									if (newlr->equal(lr[k])) {							//项目集相同
										t=true;
										break;
									}
								}
								if (t==false) {
									lr[num++] = newlr;
								}
							}
						}
					}
				}								//num 扫描结束
				bool c=false;					//项目集大小是否改变过，直到所有项目集大小不再变化为止
				if (presize!=num) c=true;
				for (int i=0;i<num;i++) {
					if (presearch[i]!=lr[i]->search.size()) {
						c=true;
						break;
					}
				}
				if (c==false) break;
				presize=num;
				for (int i=0;i<num;i++) {
					presearch[i]=lr[i]->search.size();
				}
			}
		}

		bool equal(lrset * a) {					//相同LR1项目集
			bool eq=false;
			if (a->num!=num) return false;
			for (int i=0;i<num;i++) {			//遍历查找相同LR产生式
				int temp=0;
				for (int j=0;j<a->num;j++) {	//a遍历b
					if (lr[i]->equal(a->lr[j])==true) {
					    temp=1;
						break;
					}
				}
				if (temp==0) return false;
			}
				for (int i=0;i<a->num;i++) {	//b遍历a，只有当双方遍历都为true时才返回true
				int temp=0;
				for (int j=0;j<num;j++) {
					if (a->lr[i]->equal(lr[j])==true) {
						temp=1;
						break;
					}
				}
				if (temp==0) return false;
			}
			
			return true;
		}

		void insert(lrt * a) {						//产生式合并与添加
			int ismerge=0;
			set<char>::iterator j;
			for (int k = 0; k <num;k++) {
					if (a->equal(lr[k])==true) {	//产生式相同
					ismerge=1;
					break;
				}
				if (a->same(lr[k])==true &&a->equal(lr[k])==false) {	//核相同向前搜索符不同时合并	
					for (j= a->search.begin();j!=a->search.end();j++) {
						lr[k]->search.insert(*j);
					}
					ismerge=1;
					break;
				}
			
			}
			if (ismerge==0) {									//不能合并时将产生式添加到当前项目集
				lr[num++]=a;
			}
		}

		void dot_display(char * a, int in) {					//显示带.的产生式右部，in为.的位置
		ofstream out;
		out.open("out.txt");
			for (int i=0;i<in;i++){ 
				cout<<a[i];
		//		out<<a[i];
			}
			cout<<"·";
		//	out<<"·";
			for (int i=in;i<strlen(a);i++){
			cout<<a[i];
		//	out<<a[i];
			}
			out.close();
		}

		void display(int a) {									//显示LR项目集
			ofstream out;
			out.open("out.txt");
			cout<<"I"<<a<<":"<<endl;
		//	out<<"I"<<a<<":"<<endl;
			for (int i=0;i<num;i++) {
				cout<<lr[i]->l<<"->";
			//	out<<lr[i]->l<<"->";
				dot_display(lr[i]->r,lr[i]->dot);
				cout<<",{";
			//	out<<",{";
				set<char>::iterator j;
				int q=1;
				for (j=lr[i]->search.begin();j!=lr[i]->search.end();j++) {  
					if (q==1){
						cout<<*j;
					//	out<<*j;
					   q=0;
					}	   
					else {
						cout<<" "<<*j;
					//	out<<" "<<*j;
					}
				}
				cout<<"}"<<endl;
			//	out<<"}"<<endl;
			}
			cout<<endl;
			out.close();
		}
};

lrset * lrsets[mlr];
int top;									//项目集编号
int table[mlen][mlen];						//存储下一状态编号
int kind[mlen][mlen];						//存储类型编号，acc=100 r=1(终结符) S=2(非终结符) gui=3 others=-1

void transform(int in, int t_top) {										//构造LR1分析表，in为项目集编号，t_top为产生式数量
	for (int i = 0; i < lrsets[in]->num; i++) {
		if (strlen(lrsets[in]->lr[i]->r) == lrsets[in]->lr[i]->dot) {		//右部表达式长度==.的位置，即可规约
			int dot = lrsets[in]->lr[i]->dot;
			if (dot > 0) {														//右部为非空时
				if (lrsets[in]->lr[i]->l=='$'&&lrsets[in]->lr[i]->r[dot-1]==initial) {		//.前为起始符时为acc状态
					table[in][count] = 100; 					
					kind[in][count] = 100;
				} 
				else {															//普通规约
					int  j,z=0;
					char l = lrsets[in]->lr[i]->l;
					char * r = lrsets[in]->lr[i]->r;
					for (j = 0; j < t_top;j++) {
						if (list[j]->l==l && strequal(r,list[j]->r)==true) {	//找到待规约项对应产生式的编号，list存储所有初始产生式
						    z=1;
							break;
						}
					}
					if (z==1) {
						set<char>::iterator it;
						for (it=lrsets[in]->lr[i]->search.begin();it!=lrsets[in]->lr[i]->search.end();it++) {
							if (*it=='#') {									//向前搜索符为#时，添加到最后一项
								table[in][count] = j;				
								kind[in][count] = 3;
							} 
							else {											//向前搜索符为非#时
								table[in][sint[*it]] = j;
								kind[in][sint[*it]] = 3;					
							}
						}
					} 
					else {
						cout<<"运行错误"<<endl;
					   	return;
					}
				}
			} 
			else {															//右部为空时
				char l = lrsets[in]->lr[i]->l;
				int j,z=0;
				for (j = 0; j < t_top;j++) {								//找到左部为该非终结符且右部为空的产生式编号
					if (list[j]->l==l && list[j]->r[0]==ep) {			
						z=1;
						break;
					}
				}
				if (z==1) {
					set<char>::iterator it;
					for (it=lrsets[in]->lr[i]->search.begin();it!=lrsets[in]->lr[i]->search.end();it++) {
						if (*it=='#') {
							table[in][count] = j;
							kind[in][count] = 3;
						} else {
							table[in][sint[*it]] = j;
							kind[in][sint[*it]] = 3;		
						}
					}
				} else {
					cout<<"运行错误"<<endl;
				   	return;
				}
			}
		}
	}
			
	for (int a = 0;a<count;a++) {				//项目集移动及创建，count为符号数量		
		char c = ints[a];
		lrset * p = new lrset();
		for (int i = 0; i<lrsets[in]->num; i++) {
			int dot = lrsets[in]->lr[i]->dot;
			int len = strlen(lrsets[in]->lr[i]->r);
			if (dot < len && c == lrsets[in]->lr[i]->r[dot]) {		//遍历所有符号，.后符号为c时移动
				lrt * q = new lrt();
				q->l = lrsets[in]->lr[i]->l;
				q->r = new char[mlen];
				if (q->r != NULL) strcpy(q->r, lrsets[in]->lr[i]->r);
				else {
					cout<<"运行错误"<<endl;
				   	return;
				}
				q->dot = lrsets[in]->lr[i]->dot + 1;				//.位置+1
				q->search = lrsets[in]->lr[i]->search;				//搜索符不变
				p->insert(q);										//添加到.移动后的新项目集
			}
		}
		p->closure(t_top);										//求闭包
		if (p->num== 0) continue;
		int dst,z=0;
		for (int i = 0; i < top; ++i) {					//产生的项目集存在时，将存在的状态编号存入table
			if (lrsets[i]->equal(p)==true) {
			    z=1;
				dst = i;
				break;
			}
		}
		if (z==0) dst = top++;						//项目集不存在时编号+1，状态编号存入table
		lrsets[dst] = p;
		table[in][a] = dst;
		if (ter[a]==true) {							//a为终结符时，为GOTO
			kind[in][a] = 1;
		} 
		else kind[in][a] = 2;						//a为非终结符时，为ACTION
	}
} 

void fenxitable() {				//分析表显示
	ofstream out;
	out.open("out.txt");
	cout<<"状态";
//	out<<"状态";
	for(int i=0;i<count;i++) {
		cout<<'\t'<<ints[i];
	//	out<<'\t'<<ints[i];
	}
    cout<<'\t'<<'#'<<endl;
//	out<<'\t'<<'#'<<endl;
	for (int i = 0; i < top;i++) {
		cout<<i;
		for (int j = 0; j <= count;j++) {
			if (j == count) {
				if (kind[i][j] == 100) cout<<'\t'<<"acc";
				else if (kind[i][j] == 3) {
					cout<<"\tr"<<table[i][j];
				//	out<<"\tr"<<table[i][j];
				}
				else {
					cout<<"\t ";
				//	out<<"\t ";
				}
			} 
			else {
				if (kind[i][j] == 3) {
					cout<<"\tr"<<table[i][j];
				//	out<<"\tr"<<table[i][j];
				}
				else if (kind[i][j] == 1) {
					cout<<"\tS"<<table[i][j];
                //    out<<"\tS"<<table[i][j];
				}
				else if (kind[i][j] == 2) {
					cout<<"\t"<<table[i][j];
				//	out<<"\t"<<table[i][j];
				}
				else {
					cout<<"\t ";
				//	out<<"\t ";
				}
			}
		}
		cout<<endl;
	//	out<<endl;
	}
	out.close();
}

void fenxi(char * ll) {				//分析过程
	ofstream out;
	out.open("out.txt");
	int len = strlen(ll);
	if (ll[len-1] != '#') {
		ll[len] = '#';
		ll[len+1] = 0;
		len = strlen(ll);
	}
	cout<<"待分析串："<<ll<<endl;
//	out<<"待分析串："<<ll<<endl;
	for (int i = 0; i < len-1;i++) {
		if (sint.find(ll[i]) == sint.end()) {
			cout<<"无待分析串"<<endl;
			return ;
		}
	}
	cout<<"步骤\t状态栈\t\t符号栈\t\t输入串\t\tACTION\tGOTO"<<endl;
//	out<<"步骤\t状态栈\t\t符号栈\t\t输入串\t\tACTION\tGOTO"<<endl;	
	int states[100], stop = 0;				//stop为状态栈指针
	states[stop++] = 0;						//初始状态为0
	char cstack[100], ctop = 0;				//ctop为符号栈指针	
	cstack[ctop++] = '#';					//初始符号栈为#
	int cc = 0;								//待分析符号串指针
	int round = 0;							//步骤数
	int ACTION;
	int GOTO;
	while (1) {
		int z=stop;
		if (kind[states[stop-1]][sint[ll[cc]]] == 1) {				//ACTION移进
			cout<<"("<<++round<<")\t";
		//	out<<"("<<round<<")\t";
			for (int i = 0; i < stop; i++) {
				if (states[i] < 10) {
					cout<<states[i];
			//		out<<states[i];
				}
				else {
					cout<<"("<<states[i]<<")";
				//	out<<"("<<states[i]<<")";
					z+=3;
				}
			}
			if(z>=8) {
				cout<<"\t";
			//	out<<"\t";
			}
		    else {
				cout<<"\t\t";
			//	out<<"\t\t";
			}
			for (int i = 0; i < ctop; i++) {
				cout<<cstack[i];
			//	out<<cstack[i];
			}
			if(ctop>=8) {
				cout<<'\t';
			//	out<<'\t';
			}
			else {
				cout<<"\t\t";
			//	out<<"\t\t";
			}
			cout<<ll+cc<<"\t";
		//	out<<ll+cc<<"\t";
			if(strlen(ll+cc)<8) {
				cout<<"\t";
			//	out<<"\t";
			}
			ACTION = table[states[stop-1]][sint[ll[cc]]];			//读取新状态
			cout<<"S"<<ACTION<<endl;
		//	out<<"S"<<ACTION<<endl;
			states[stop++] = ACTION;
			cstack[ctop++] = ll[cc++];
			continue;
		}

		//规约
		if (kind[states[stop-1]][sint[ll[cc]]] == 3){
			cout<<"("<<++round<<")\t";
		//	out<<"("<<++round<<")\t";
			for (int i = 0; i < stop; i++) {
			 if(states[i] < 10) {
				 cout<<states[i];
			//	 out<<states[i];
			 }
				else {
					cout<<"("<<states[i]<<")";
			//		out<<"("<<states[i]<<")";
					z+=3;
				}
			}
			if(z>=8) {
				cout<<"\t";
			//	out<<"\t";
			}
			else {
				cout<<"\t\t";
				//out<<"\t\t";
			}
			for (int i = 0; i < ctop; i++) {
				cout<<cstack[i];
			//	out<<cstack[i];
			}
			if(ctop>=8) {
			//	out<<"\t";
				cout<<"\t";
				}
		    else {
				cout<<"\t\t";
			//	out<<"\t\t";
			}
			cout<<ll+cc<<"\t";
		//	out<<ll+cc<<"\t";
			if(strlen(ll+cc)<8) {
				cout<<"\t";
			//	out<<"\t";
			}
			ACTION = table[states[stop-1]][sint[ll[cc]]];				//读取规约的产生式编号
			cout<<"r"<<ACTION<<"\t";
		//	out<<"r"<<ACTION<<"\t";
			
			if (list[ACTION]->r[0] != ep) {								//规约动作，右部不为空时，状态栈和符号栈弹栈
				int len = strlen(list[ACTION]->r);
				stop -= len;										
				ctop -= len;
				cstack[ctop++] = list[ACTION]->l;
				GOTO = table[states[stop-1]][sint[list[ACTION]->l]];		//goto到新状态，即栈顶状态遇到规约的非终结符到达的状态
				cout<<GOTO<<endl;
		//		out<<GOTO<<endl;
				states[stop++] = GOTO;
			} 
			else {														//右部为空时不弹栈
				cstack[ctop++] = list[ACTION]->l;
				GOTO = table[states[stop-1]][sint[list[ACTION]->l]];
				cout<<GOTO<<endl;
		//		out<<GOTO<<endl;
				states[stop++] = GOTO;
			}
			continue;
		}
//接受acc
		if (kind[states[stop-1]][sint[ll[cc]]] == 100) {
			cout<<"("<<++round<<")\t";
			for (int i = 0; i < stop; i++) {
				cout<<states[i];
		//		out<<states[i];
			}
			if(stop>=8) {
		//		out<<"\t";
				cout<<"\t";
			}
			else {
				cout<<"\t\t";
		//		out<<"\t\t";
			}
			for (int i = 0; i < ctop; i++) {
				cout<<cstack[i];
			//	out<<cstack[i];
			}
			if(ctop>=8) {
				cout<<"\t";
			//	out<<"\t";
			}
			else {
				cout<<"\t\t";
			//	out<<"\t\t";
			}
			cout<<ll+cc<<"\t";
		//	out<<ll+cc<<"\t";
			if(strlen(ll+cc)<8) {
				cout<<"\t";
		//		out<<"\t";
			}
			cout<<"acc"<<endl<<endl;
		//	out<<"acc"<<endl<<endl;
			break;
		}
		cout<<"栈顶状态："<<states[stop-1]<<" 当前字符"<<ll[cc]<<",错误"<<endl<<endl;		//情况都不满足时出错
	//	out<<"栈顶状态："<<states[stop-1]<<"当前字符"<<ll[cc]<<"，错误"<<endl<<endl;
		break;
	}
	out.close();
}

int main() {
	init();
	FILE * in;
	in = fopen("test2.txt", "r");
	int t_top = 1;
    ofstream out;
	out.open("out.txt");
	l = (char *)malloc(30*sizeof(char));
	r = (char *)malloc(30*sizeof(char));

	while (fscanf(in, "%s %s", l, r) != EOF) {
		if (t_top == 1) {								//添加$->S
			char s[10];
			s[0] = l[0], s[1] = 0;
			list[0] = new tt('$', s);
			initial = l[0];
		}
		list[t_top++] = new tt(l[0], (char *)r);
		l = (char *)malloc(30*sizeof(char));
		r = (char *)malloc(30*sizeof(char));
	}
	for (int i = 0; i < t_top; i++) list[i]->display();

	if (l != NULL) free(l);
	if (r != NULL) free(r);
	fclose(in);

	for (int i = 1; i < t_top; i++) {						//添加非终结符的映射
		char tmp;
		if (sint.find(list[i]->l) == sint.end()) {
			tmp = list[i]->l;
			sint[tmp] = count;
			ints[count] = tmp;
			if (S(tmp)==true) {
				ter[count] = false;
			} 
			else {
				ter[count] = true;
			}
			++count;
		}
		//空字符ep没有被加入到字符表中
		for (int j = 0; j < strlen(list[i]->r); j++) {		//添加终结符的映射
			tmp = list[i]->r[j];
			if (tmp == ep) {
				continue;
			}
			if (sint.find(tmp) == sint.end()) {
				sint[tmp] = count;
				ints[count] = tmp;
				if (S(tmp)==true) {
					ter[count] = false;
				} else {
					ter[count] = true;
				}
				++count;
			}
		}
	}
	tempty(t_top);
	firstset(t_top);
	showfirstset();
	top = 0;
	lrt * p = new lrt(list[0]->l, list[0]->r, 0);
	lrsets[top] = new lrset();
	lrsets[top]->insert(p);
	lrsets[top]->closure(t_top);
	//lrsets[top]->display(0);
	top++;
	memset(table, -1, sizeof(table));
	for (int i = 0; i < top; i++) {
	   transform(i,t_top);	
	}
	for (int i = 0; i < top; i++) {
		lrsets[i]->display(i);
	}
	fenxitable();
	
	in = fopen("yu2.txt", "r");
	if (in == NULL) {
	    cout<<"找不到指定文件"<<endl; 
		return 0;
	}
	char ll[100];
	sint['#'] = count;
	ints[count++] = '#';
	while (fscanf(in, "%s", ll) != EOF) {
		fenxi(ll);
	}
    out.close();
    cout<<endl;
	cout<<"分析完毕~~~~"<<endl;
	
	return 0;
}



















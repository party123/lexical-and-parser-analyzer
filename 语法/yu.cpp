#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#define ep '~'								//����շ���Ϊ~
#define op set<int>::iterator				//����set��ָ��
const int mm = 30;
const int mlen = 100;
const int mlr = 50;
using namespace std;
map<char, int> sint;
map<int, char> ints;
set<char> first[mlen];			//FIRST��
char * l,* r;
char initial;					//��ʼ��
int count;						//������
bool ter[mlen];					//�Ƿ�Ϊ�ս��
bool empty[mm];					//�Ƿ����Ƴ���

void init() {					//��ʼ��
    sint.clear();
	ints.clear();
	memset(ter,false,sizeof(ter));
	memset(empty,false,sizeof(empty));
	count = 0;
}

bool S(char a) {				//�ж��Ƿ�Ϊ���ս��
	if(a>='A'&&a<='Z') return true;
	else return false;
}
class tt {					//����ʽ��
	public:
		char l;				//�󲿷��ս��
		char * r;			//����ʽ�Ҳ��ַ���
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

bool noempty(char * a) {							//�Ҳ���Ϊ��
	for (int i=0;i<strlen(a);i++) {
		if (S(a[i])==false&&a[i]!=ep) 
			return true;
	}
}

void tempty(int a) {								//�����Ƴ���
	for (int i=0;i<a;i++) {
		if (list[i]->r[0]==ep) {
			empty[sint[list[i]->l]]=true;			//empty��ʾ���ʽ�Ҳ��Ƿ�Ϊ��
		}
	}
	set<int> s;										//�������Ƴ��յķ��ս��
	for (int i=0;i<count;i++) {
		if (empty[i]==true) s.insert(i);
	}
	while (1) {
		int	ll=s.size();
		for (int i=1;i<a;i++) {
			if (empty[sint[list[i]->l]]==false) {		
				if (noempty(list[i]->r)==false) {		//�Ҳ���Ϊ��ʱ�ж��Ҳ��Ƿ����Ƴ���
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

void firstset(int a) {										//����first��
	int temp[mlen],f;
	for (int i=0;i<count;i++) 
		temp[i]=first[i].size();
	while(1){
		for (int i=0;i<count;i++) {							//�������з��ţ�����ÿ�����ŵ�FIRST��					
			if (ter[i]==true) {								//�ս����FIRST��Ϊ����
				first[i].insert(ints[i]);
				continue;
			}
			for (int j=1;j<a;j++) {							//���ս����FIRST����ȡ����
				if (list[j]->l==ints[i]) {					//���в���ʽ��Ϊ��ǰɨ����ս�ʱ
					if (list[j]->r[0]==ep) continue;			//���ս���Ҳ�Ϊ����continue
					for (int k=0;k<strlen(list[j]->r);k++) {	//�Ҳ���Ϊ�����ҵ���һ���ս��
						char tmp=list[j]->r[k];
						if (ter[sint[tmp]]==true) {				//Ϊ�ս�������
							first[i].insert(list[j]->r[k]);
							break;
						} 
						else {								//�Ҳ���һ��Ϊ���ս��ʱ����FIRST������
							for (set<char>::iterator y=first[sint[tmp]].begin();y!=first[sint[tmp]].end();y++) {	
								first[i].insert(*y);
							}
							if (empty[sint[tmp]]==false){		//ֱ����һ�������Ƴ��յķ��ս�Ϊֹ���������ѭ��
								break;
							}
						}
					}
				}
			}
		}
		f=0;
		for (int i=0;i<count;i++) {								//ÿ�����ŵ�FIRST����������Ϊֹ
			if (first[i].size()!=temp[i]) {
				f=1;
				break;
			}
		}
		if (f==0) break;
		for (int i=0;i<count;i++) temp[i]=first[i].size();
	}
}

void showfirstset() {								//FIRST����ʾ
	ofstream out;
	out.open("out.txt");
	cout<<"FIRST��:"<<endl;
//	out<<"FIRST��:"<<endl;
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
//�ַ������
bool strequal(char * s1, char * s2) {
	if (strlen(s1)!=strlen(s2)) return false;
	for (int i=0;i<strlen(s1);i++)
		if (s1[i]!=s2[i]) return false;
	return true;
}

//�����������
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

class lrt {						//��Ŀ������ʽ
	public:
		char l;
		char * r;
		int dot;					//dot��¼���λ��
		set<char> search;			//��ǰ������
		lrt() {}
		lrt(char l, char * r, int dot) {
			this->l=l;
			this->r=r;
			this->dot=dot;
			if (l=='$') search.insert('#');		//��ʼ������ǰ��������Ϊ#
		}
		bool same(lrt * p) {				//��ͬ�˲�ͬ��ǰ������
			if (p->l!=l) return false;
			if (!strequal(p->r,r)) return false;		
			if (p->dot!= dot) return false;
			return true;
		}
		bool equal(lrt * p) {			//��Ⱥ���ͬ��ǰ������
			if (p->l!=l) return false;
			if (strequal(p->r,r)==false) return false;
			if (p->dot!=dot) return false;
			if (setequal(p->search,search)==false) return false;
			return true;
		}	
};

class lrset {								//����LR1��Ŀ��
	public:
		lrt * lr[mm];
		int num;							//��Ŀ����С
		lrset() {
			num=0;
			for (int i=0;i<mm;i++) lr[i]=NULL;
		}
		lrset(lrt * a) {
			if (a==NULL||a->dot!=0) {
				cout<<"����LR��Ŀ������!"<<endl;
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

        void closure(int t_top) {			//����Ŀ���հ�
			int presearch[mlen];
			int	presize =num;				
			for (int i=0;i<num;i++) 
				presearch[i]=lr[i]->search.size();
			while (1) {
				for (int i=0;i<num;i++) {
					if (lr[i]->dot==strlen(lr[i]->r)) continue;			//�ɹ�Լ״̬
					int dot=lr[i]->dot;
					char tmp=lr[i]->r[dot];								//.����һ���ַ�
					if (ter[tmp]==false) {									//.��Ϊ���ս��ʱ�򽫷��ս���ĵ�һ������ʽ����
						for (int j=0;j<t_top;j++) {
							if (list[j]->l==tmp) {
								lrt * newlr = new lrt();
								newlr->l=tmp;
								newlr->dot=0;
								if (list[j]->r[0]==ep) newlr->r="";			//�²���ʽ�Ҳ�Ϊ��ʱ
								else newlr->r=list[j]->r;

								set<char> tmpsearch;
								int in;
								for (in=lr[i]->dot+1;in<strlen(lr[i]->r);in++) {		//�����ս������Ŵ���first��������ӽ�����������
									char wait=lr[i]->r[in];
									for (set<char>::iterator it=first[sint[wait]].begin();it!=first[sint[wait]].end();it++) {
										tmpsearch.insert(*it);
									}
									if (empty[sint[wait]]==false)	break;
								}
								if (in==strlen(lr[i]->r)) {						//���ս����ķ��Ŵ�ȫΪ��ʱ��ԭ������ʽ��������������
									for (set<char>::iterator it=lr[i]->search.begin();it!=lr[i]->search.end();it++) {
										tmpsearch.insert(*it);
									}
								}
								for (set<char>::iterator it=tmpsearch.begin();it!=tmpsearch.end();it++) {
									newlr->search.insert(*it);
								}

								bool t=false;
								for (int k=0;k<num;k++) {								//��Ŀ����ȥ����ϲ�
									if (newlr->same(lr[k])&&!newlr->equal(lr[k])) {		//����ʽ��ͬ��Ŀ����ͬʱ�ϲ�	
										set<char>::iterator it;
										for (it=newlr->search.begin();it!=newlr->search.end();it++) {
											lr[k]->search.insert(*it);
										}
										t=true;
										break;
									}
									if (newlr->equal(lr[k])) {							//��Ŀ����ͬ
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
				}								//num ɨ�����
				bool c=false;					//��Ŀ����С�Ƿ�ı����ֱ��������Ŀ����С���ٱ仯Ϊֹ
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

		bool equal(lrset * a) {					//��ͬLR1��Ŀ��
			bool eq=false;
			if (a->num!=num) return false;
			for (int i=0;i<num;i++) {			//����������ͬLR����ʽ
				int temp=0;
				for (int j=0;j<a->num;j++) {	//a����b
					if (lr[i]->equal(a->lr[j])==true) {
					    temp=1;
						break;
					}
				}
				if (temp==0) return false;
			}
				for (int i=0;i<a->num;i++) {	//b����a��ֻ�е�˫��������Ϊtrueʱ�ŷ���true
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

		void insert(lrt * a) {						//����ʽ�ϲ������
			int ismerge=0;
			set<char>::iterator j;
			for (int k = 0; k <num;k++) {
					if (a->equal(lr[k])==true) {	//����ʽ��ͬ
					ismerge=1;
					break;
				}
				if (a->same(lr[k])==true &&a->equal(lr[k])==false) {	//����ͬ��ǰ��������ͬʱ�ϲ�	
					for (j= a->search.begin();j!=a->search.end();j++) {
						lr[k]->search.insert(*j);
					}
					ismerge=1;
					break;
				}
			
			}
			if (ismerge==0) {									//���ܺϲ�ʱ������ʽ��ӵ���ǰ��Ŀ��
				lr[num++]=a;
			}
		}

		void dot_display(char * a, int in) {					//��ʾ��.�Ĳ���ʽ�Ҳ���inΪ.��λ��
		ofstream out;
		out.open("out.txt");
			for (int i=0;i<in;i++){ 
				cout<<a[i];
		//		out<<a[i];
			}
			cout<<"��";
		//	out<<"��";
			for (int i=in;i<strlen(a);i++){
			cout<<a[i];
		//	out<<a[i];
			}
			out.close();
		}

		void display(int a) {									//��ʾLR��Ŀ��
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
int top;									//��Ŀ�����
int table[mlen][mlen];						//�洢��һ״̬���
int kind[mlen][mlen];						//�洢���ͱ�ţ�acc=100 r=1(�ս��) S=2(���ս��) gui=3 others=-1

void transform(int in, int t_top) {										//����LR1������inΪ��Ŀ����ţ�t_topΪ����ʽ����
	for (int i = 0; i < lrsets[in]->num; i++) {
		if (strlen(lrsets[in]->lr[i]->r) == lrsets[in]->lr[i]->dot) {		//�Ҳ����ʽ����==.��λ�ã����ɹ�Լ
			int dot = lrsets[in]->lr[i]->dot;
			if (dot > 0) {														//�Ҳ�Ϊ�ǿ�ʱ
				if (lrsets[in]->lr[i]->l=='$'&&lrsets[in]->lr[i]->r[dot-1]==initial) {		//.ǰΪ��ʼ��ʱΪacc״̬
					table[in][count] = 100; 					
					kind[in][count] = 100;
				} 
				else {															//��ͨ��Լ
					int  j,z=0;
					char l = lrsets[in]->lr[i]->l;
					char * r = lrsets[in]->lr[i]->r;
					for (j = 0; j < t_top;j++) {
						if (list[j]->l==l && strequal(r,list[j]->r)==true) {	//�ҵ�����Լ���Ӧ����ʽ�ı�ţ�list�洢���г�ʼ����ʽ
						    z=1;
							break;
						}
					}
					if (z==1) {
						set<char>::iterator it;
						for (it=lrsets[in]->lr[i]->search.begin();it!=lrsets[in]->lr[i]->search.end();it++) {
							if (*it=='#') {									//��ǰ������Ϊ#ʱ����ӵ����һ��
								table[in][count] = j;				
								kind[in][count] = 3;
							} 
							else {											//��ǰ������Ϊ��#ʱ
								table[in][sint[*it]] = j;
								kind[in][sint[*it]] = 3;					
							}
						}
					} 
					else {
						cout<<"���д���"<<endl;
					   	return;
					}
				}
			} 
			else {															//�Ҳ�Ϊ��ʱ
				char l = lrsets[in]->lr[i]->l;
				int j,z=0;
				for (j = 0; j < t_top;j++) {								//�ҵ���Ϊ�÷��ս�����Ҳ�Ϊ�յĲ���ʽ���
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
					cout<<"���д���"<<endl;
				   	return;
				}
			}
		}
	}
			
	for (int a = 0;a<count;a++) {				//��Ŀ���ƶ���������countΪ��������		
		char c = ints[a];
		lrset * p = new lrset();
		for (int i = 0; i<lrsets[in]->num; i++) {
			int dot = lrsets[in]->lr[i]->dot;
			int len = strlen(lrsets[in]->lr[i]->r);
			if (dot < len && c == lrsets[in]->lr[i]->r[dot]) {		//�������з��ţ�.�����Ϊcʱ�ƶ�
				lrt * q = new lrt();
				q->l = lrsets[in]->lr[i]->l;
				q->r = new char[mlen];
				if (q->r != NULL) strcpy(q->r, lrsets[in]->lr[i]->r);
				else {
					cout<<"���д���"<<endl;
				   	return;
				}
				q->dot = lrsets[in]->lr[i]->dot + 1;				//.λ��+1
				q->search = lrsets[in]->lr[i]->search;				//����������
				p->insert(q);										//��ӵ�.�ƶ��������Ŀ��
			}
		}
		p->closure(t_top);										//��հ�
		if (p->num== 0) continue;
		int dst,z=0;
		for (int i = 0; i < top; ++i) {					//��������Ŀ������ʱ�������ڵ�״̬��Ŵ���table
			if (lrsets[i]->equal(p)==true) {
			    z=1;
				dst = i;
				break;
			}
		}
		if (z==0) dst = top++;						//��Ŀ��������ʱ���+1��״̬��Ŵ���table
		lrsets[dst] = p;
		table[in][a] = dst;
		if (ter[a]==true) {							//aΪ�ս��ʱ��ΪGOTO
			kind[in][a] = 1;
		} 
		else kind[in][a] = 2;						//aΪ���ս��ʱ��ΪACTION
	}
} 

void fenxitable() {				//��������ʾ
	ofstream out;
	out.open("out.txt");
	cout<<"״̬";
//	out<<"״̬";
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

void fenxi(char * ll) {				//��������
	ofstream out;
	out.open("out.txt");
	int len = strlen(ll);
	if (ll[len-1] != '#') {
		ll[len] = '#';
		ll[len+1] = 0;
		len = strlen(ll);
	}
	cout<<"����������"<<ll<<endl;
//	out<<"����������"<<ll<<endl;
	for (int i = 0; i < len-1;i++) {
		if (sint.find(ll[i]) == sint.end()) {
			cout<<"�޴�������"<<endl;
			return ;
		}
	}
	cout<<"����\t״̬ջ\t\t����ջ\t\t���봮\t\tACTION\tGOTO"<<endl;
//	out<<"����\t״̬ջ\t\t����ջ\t\t���봮\t\tACTION\tGOTO"<<endl;	
	int states[100], stop = 0;				//stopΪ״̬ջָ��
	states[stop++] = 0;						//��ʼ״̬Ϊ0
	char cstack[100], ctop = 0;				//ctopΪ����ջָ��	
	cstack[ctop++] = '#';					//��ʼ����ջΪ#
	int cc = 0;								//���������Ŵ�ָ��
	int round = 0;							//������
	int ACTION;
	int GOTO;
	while (1) {
		int z=stop;
		if (kind[states[stop-1]][sint[ll[cc]]] == 1) {				//ACTION�ƽ�
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
			ACTION = table[states[stop-1]][sint[ll[cc]]];			//��ȡ��״̬
			cout<<"S"<<ACTION<<endl;
		//	out<<"S"<<ACTION<<endl;
			states[stop++] = ACTION;
			cstack[ctop++] = ll[cc++];
			continue;
		}

		//��Լ
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
			ACTION = table[states[stop-1]][sint[ll[cc]]];				//��ȡ��Լ�Ĳ���ʽ���
			cout<<"r"<<ACTION<<"\t";
		//	out<<"r"<<ACTION<<"\t";
			
			if (list[ACTION]->r[0] != ep) {								//��Լ�������Ҳ���Ϊ��ʱ��״̬ջ�ͷ���ջ��ջ
				int len = strlen(list[ACTION]->r);
				stop -= len;										
				ctop -= len;
				cstack[ctop++] = list[ACTION]->l;
				GOTO = table[states[stop-1]][sint[list[ACTION]->l]];		//goto����״̬����ջ��״̬������Լ�ķ��ս�������״̬
				cout<<GOTO<<endl;
		//		out<<GOTO<<endl;
				states[stop++] = GOTO;
			} 
			else {														//�Ҳ�Ϊ��ʱ����ջ
				cstack[ctop++] = list[ACTION]->l;
				GOTO = table[states[stop-1]][sint[list[ACTION]->l]];
				cout<<GOTO<<endl;
		//		out<<GOTO<<endl;
				states[stop++] = GOTO;
			}
			continue;
		}
//����acc
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
		cout<<"ջ��״̬��"<<states[stop-1]<<" ��ǰ�ַ�"<<ll[cc]<<",����"<<endl<<endl;		//�����������ʱ����
	//	out<<"ջ��״̬��"<<states[stop-1]<<"��ǰ�ַ�"<<ll[cc]<<"������"<<endl<<endl;
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
		if (t_top == 1) {								//���$->S
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

	for (int i = 1; i < t_top; i++) {						//��ӷ��ս����ӳ��
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
		//���ַ�epû�б����뵽�ַ�����
		for (int j = 0; j < strlen(list[i]->r); j++) {		//����ս����ӳ��
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
	    cout<<"�Ҳ���ָ���ļ�"<<endl; 
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
	cout<<"�������~~~~"<<endl;
	
	return 0;
}



















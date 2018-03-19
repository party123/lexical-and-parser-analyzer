#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <algorithm>
#define ed '#'			 //��̬
using namespace std;
bool S(char a) {
	if (a>='A' && a<='Z') return true;
	else return false;
}
class NFA {					//nfa��
	public:
	vector<int> nfa[50][50];
    int Snum;				//���ս����
	int snum;				//�ս����
	map<char,int> Sint;		//�����������ս������ŵ�ӳ��
	map<int,char> intS;		//����������ŵ����ս����ӳ��
	map<char,int> sint;		//���������ս������ŵ�ӳ��
	map<int,char> ints;		//����������ŵ��ս����ӳ��

	NFA(int a) {			//���캯��
	    Snum=0;				//���ս����Ϊ0
		snum=0;				//�ս����Ϊ0
		Sint[ed]=Snum;		//#��״̬��Ϊ0
		intS[Snum]=ed;
		Snum++;				//״̬��+1
		char temp[500];
		FILE * read;		//��ȡ�ļ�
		if (a==0)  
			read=fopen("������.txt","r");			//����������ķ�
		else if (a==1) 
			read=fopen("��ʶ��.txt","r");			//�����ʶ���ķ�
		else 
			read=fopen("����.txt","r");				//���������ķ�
       
		while (fgets(temp,500-1,read)!=NULL) { //��read�ļ��ж���maxlen-1���ȵ��ַ�����temp�ַ�������
			int ll=strlen(temp);				//���鳤��
			int i;								//�����±�
			char start;                         //�ҵ���һ�����ս��startΪ��ʼ��
			for(i=0;i<ll;i++){
				if(S(temp[i])==true){
						start=temp[i];
						i++;
						if(Sint.find(start)==Sint.end()){			//start״̬δ����ʱ����״̬����
						Sint[start]=Snum;
						intS[Snum]=start;
						Snum++;
						}
						break;
						}
				}
			while (i<=ll-1){
				if (temp[i]=='\n'||temp[i]==' '||temp[i]=='|'){
					i++;
					continue;
				}
				else if (temp[i]=='-'&&temp[i+1]=='>'){
					i+=2;
					continue;
				}
				else if (i+1<ll&&S(temp[i+1])==true) {									//S->aA��ʽ
                    if (sint.find(temp[i])==sint.end()){								//��ǰ����Ϊ�ս��
                        sint[temp[i]]=snum;
                        ints[snum]=temp[i]; 
				        snum++;	
                    }
                    if (Sint.find(temp[i+1])==Sint.end()) {								//��һ����Ϊ���ս��
                        Sint[temp[i+1]]=Snum;
                        intS[Snum]=temp[i+1];
						Snum++;
                    }
					nfa[Sint[start]][sint[temp[i]]].push_back(Sint[temp[i+1]]);			//if S->+A: [S][+]=A
					i+=2;
				} 
				else {																		//S->a��ʽ
                    if (sint.find(temp[i])==sint.end()){
                        sint[temp[i]]=snum;
                        ints[snum]=temp[i];
						snum++;
                    }
					nfa[Sint[start]][sint[temp[i]]].push_back(Sint[ed]);				//if S->a�� [S][+]=#,��ӵ��ս�״̬
					i++;
				}
			}    //while����
		}
		fclose(read);
		//cout<<snum<<' '<<Snum<<endl;
	}

};



//DFA��
class DFA {
	public:
	vector<int> dfa_tmp[50];
	int dfa[50][50];
	bool ised[50];					//�ս����־
	int count;					//״̬��

	DFA(NFA a) {				//���캯��
		count=0;
		memset(dfa,-1,sizeof(dfa));
		memset(ised,false,sizeof(ised));
		bool same;
		int p,q,z,l,w,t;
		p=q=0;
	    dfa_tmp[q++].push_back(a.Sint['S']);							//dfa_tmpΪ״̬����
		while (p != q) {          
			for (int i=0;i<a.sint.size();i++) {     //�����ս��
				vector<int> temp;					//��ʱ����
				for (int j=0;j<dfa_tmp[p].size();j++) {  //������״̬�����з��ս��       
					int s= dfa_tmp[p][j];                           
					for (int index=0;index<a.nfa[s][i].size();index++) { //������ͬ�ս���ļ���
						z=0;
						t=a.nfa[s][i][index];//ͬһ���ս��������ͬ�ս���ɵ����״̬
						for (int k=0;k<temp.size();k++) {
							if (temp[k]==t) {     //�������Ƿ���ڵ�ǰ״̬
								z=1;
								break;
							}
						}
						if (z==0) temp.push_back(t);//�����в����ڵ�ǰ״̬����뼯��
					}
				}
				if (temp.size()==0) continue;
				
				sort(temp.begin(),temp.end());    //temp��ʱΪ����ĳһ�ս���ɴﵽ��״̬����
				l=w=0;
				for (w=0;w<q;w++) {    
					if (dfa_tmp[w].size()==temp.size()) {//Ԫ�ظ�����ͬʱ�Ƚ�temp�е�״̬���Ƿ��Ѵ���
						int u;
						for (u=0;u<temp.size();u++) {   
							if (temp[u]!=dfa_tmp[w][u]) break;
						}
						if (u>=temp.size()) {
							l=1;
							break;
						}
					}
					if (l==1) break;
				}
				if (l==0) {
					//����ͬʱ����µ�״̬��
					for (int x=0;x<temp.size();x++) 
						dfa_tmp[q].push_back(temp[x]);				//���¼��ϼ���
					    dfa[p][i]=q;									 //p��ʼ״̬�������ս���ɵ����״̬����
					for (int y=0;y<dfa_tmp[q].size();y++) {
						if (dfa_tmp[q][y]==a.Sint[ed])	{		//�ж��Ƿ�Ϊ��̬
							ised[q] = true;
							break;
						}
					}
					q++;
				} 
				else {
				dfa[p][i]=w;//��ͬʱΪ��ǰ״̬��
				}
			}
			p++;//�ı���ʼ״̬
		}
		count=q;//dfa״̬��
	//	cout<<q<<endl;
	}

	void display(NFA a) {
		for (int i = 0; i < count; ++i) {
			for (int j = 0; j < a.sint.size();j++) {
				if (dfa[i][j] != -1) {
					printf("%d->%c %d\n", i,a.ints[j],dfa[i][j]);
				}
			}
		}
	}


	int fenxi(char * s, int ss,NFA a) {					//�������̣�ssΪ��ʼλ��
		int l=strlen(s);
		int an;											//token����ֹλ��
		for (int i=ss;i<l;i++) {
			vector<int> t;
		    t.clear();
			if (s[i]==' '||s[i]=='\n') {
				an=i-1;
				break;
			}
			int num=0,z=0;
			for (int j=ss;j<=i;j++) {
				if (a.sint.find(s[j])==a.sint.end()) {	//����û����ķ����˳�
					z=1;
					an=i-1;
					break;
				}
				t.push_back(a.sint[s[j]]);				//�������t��
				num++;
			}
			if (z==1) break;
			int c=a.Sint[ed];
			for (int k=0;k<num;k++) {
				if (dfa[c][t[k]]==-1) {					//û��״̬ת��ʱֹͣ
					an=i-1;
					z=1;
					break;
				}
				c=dfa[c][t[k]];
			}
			if (z==1) break;
		}
		return an;
	}
};

bool letter(char s) {
	if((s>='a'&&s<='z')||(s>='A'&&s<='Z')) 
		return true;
	else return false;
}

bool blank(char s){
	if(s==' '||s=='\t')
		return true;
	else return false;
}

bool bundary(char s){
	if(s=='{'||s=='}'||s=='['||s==']'||s=='('||s==')'||s=='#'||s==','||s==';'||s==':'||s=='\''||s=='\"')
		return true;
	else return true;
}

bool key(char * s){
	if(strcmp(s,"include")==0||strcmp(s,"define")==0||strcmp(s,"iostream")==0||strcmp(s,"int")==0||strcmp(s,"float")==0||strcmp(s,"double")==0||strcmp(s,"main")==0||strcmp(s,"if")==0||strcmp(s,"else")==0||strcmp(s,"return")==0||strcmp(s,"for")==0||strcmp(s,"while")==0||strcmp(s,"do")==0||strcmp(s,"switch")==0||strcmp(s,"case")==0||strcmp(s,"cin")==0||strcmp(s,"cout")==0||strcmp(s,"continue")==0)
		return true;
	else return false;
}

void strcopy(char * d, char * s, int a, int b) {
	int k = 0;
	for (int i=a;i<=b;i++) d[k++]=s[i];
	d[k++] = 0;
}

int main()
{   
	char temp[500];
	int line=1;
	NFA a(0);
	NFA b(1);
	NFA c(2);
	DFA aa(a);
	DFA bb(b);
	DFA cc(c);
	cout<<"DFA:������"<<endl;
	aa.display(a);
	cout<<"DFA:��ʶ��"<<endl;
	bb.display(b);
	cout<<"DFA:����"<<endl;
	cc.display(c);
	FILE * read=fopen("y.cpp","r");
    ofstream out;
	out.open("out.txt");
	while (fgets(temp,500-1,read)!=NULL){
		int len=strlen(temp);
		int i=0;
		while (1) {
			char t[50];
			while (temp[i]==' '||temp[i]=='\t') i++;
			if (i==len||temp[i]=='\n')  break;
			int aaa=aa.fenxi(temp,i,a);			//�������ķ�ƥ�������
			int bbb=bb.fenxi(temp,i,b);			//��־���ķ�ƥ�������
			int ccc=cc.fenxi(temp,i,c);			//�����ķ�ƥ�������
			if (aaa==i-1&&bbb==i-1&&ccc==i-1) {
				if (bundary(temp[i])==true) {
					out<<"("<<line<<"): "<<temp[i++]<<' '<<"���"<<endl;
					continue;
				}
				out<<"����:("<<line<<") "<<temp[i]<<endl;
				return 0;
			}

			if (aaa>bbb&&aaa>ccc) {			//�����ƥ��ķ��������Ϊ�����
				strcopy(t,temp,i,aaa);		//��temp��i��aaa copy��t
				out<<"("<<line<<"): "<<t<<' '<<"�����"<<endl;
				i=aaa+1;
				continue;
			}

			if (bbb>aaa&&bbb>ccc) {
				strcopy(t,temp,i,bbb);
				if (key(t)==true) {
					out<<"("<<line<<"): "<<t<<' '<<"�ؼ���"<<endl;
				} 
				else {
					out<<"("<<line<<"): "<<t<<' '<<"��ʶ��"<<endl;
				}
				i=bbb+1;
				continue;
			}
			if (ccc>aaa&&ccc>bbb) {
				strcopy(t,temp,i,ccc);
				out<<"("<<line<<"): "<<t<<' '<<"����"<<endl;
				i=ccc+1;
				continue;
			}
		}
		line++;
	}
	out.close();
	cout<<"�������~"<<endl;
	return 0;
}

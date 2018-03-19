#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <algorithm>
#define ed '#'			 //终态
using namespace std;
bool S(char a) {
	if (a>='A' && a<='Z') return true;
	else return false;
}
class NFA {					//nfa类
	public:
	vector<int> nfa[50][50];
    int Snum;				//非终结符数
	int snum;				//终结符数
	map<char,int> Sint;		//用来建立非终结符到编号的映射
	map<int,char> intS;		//用来建立编号到非终结符的映射
	map<char,int> sint;		//用来建立终结符到编号的映射
	map<int,char> ints;		//用来建立编号到终结符的映射

	NFA(int a) {			//构造函数
	    Snum=0;				//非终结符数为0
		snum=0;				//终结符数为0
		Sint[ed]=Snum;		//#的状态设为0
		intS[Snum]=ed;
		Snum++;				//状态号+1
		char temp[500];
		FILE * read;		//读取文件
		if (a==0)  
			read=fopen("操作符.txt","r");			//读入操作符文法
		else if (a==1) 
			read=fopen("标识符.txt","r");			//读入标识符文法
		else 
			read=fopen("数字.txt","r");				//读入数字文法
       
		while (fgets(temp,500-1,read)!=NULL) { //从read文件中读出maxlen-1长度的字符串到temp字符数组中
			int ll=strlen(temp);				//数组长度
			int i;								//数组下标
			char start;                         //找到第一个非终结符start为起始符
			for(i=0;i<ll;i++){
				if(S(temp[i])==true){
						start=temp[i];
						i++;
						if(Sint.find(start)==Sint.end()){			//start状态未定义时加入状态表中
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
				else if (i+1<ll&&S(temp[i+1])==true) {									//S->aA形式
                    if (sint.find(temp[i])==sint.end()){								//当前坐标为终结符
                        sint[temp[i]]=snum;
                        ints[snum]=temp[i]; 
				        snum++;	
                    }
                    if (Sint.find(temp[i+1])==Sint.end()) {								//下一坐标为非终结符
                        Sint[temp[i+1]]=Snum;
                        intS[Snum]=temp[i+1];
						Snum++;
                    }
					nfa[Sint[start]][sint[temp[i]]].push_back(Sint[temp[i+1]]);			//if S->+A: [S][+]=A
					i+=2;
				} 
				else {																		//S->a形式
                    if (sint.find(temp[i])==sint.end()){
                        sint[temp[i]]=snum;
                        ints[snum]=temp[i];
						snum++;
                    }
					nfa[Sint[start]][sint[temp[i]]].push_back(Sint[ed]);				//if S->a则 [S][+]=#,添加到终结状态
					i++;
				}
			}    //while结束
		}
		fclose(read);
		//cout<<snum<<' '<<Snum<<endl;
	}

};



//DFA类
class DFA {
	public:
	vector<int> dfa_tmp[50];
	int dfa[50][50];
	bool ised[50];					//终结符标志
	int count;					//状态数

	DFA(NFA a) {				//构造函数
		count=0;
		memset(dfa,-1,sizeof(dfa));
		memset(ised,false,sizeof(ised));
		bool same;
		int p,q,z,l,w,t;
		p=q=0;
	    dfa_tmp[q++].push_back(a.Sint['S']);							//dfa_tmp为状态集合
		while (p != q) {          
			for (int i=0;i<a.sint.size();i++) {     //遍历终结符
				vector<int> temp;					//临时集合
				for (int j=0;j<dfa_tmp[p].size();j++) {  //遍历该状态的所有非终结符       
					int s= dfa_tmp[p][j];                           
					for (int index=0;index<a.nfa[s][i].size();index++) { //经过相同终结符的集合
						z=0;
						t=a.nfa[s][i][index];//同一非终结符经过相同终结符可到达的状态
						for (int k=0;k<temp.size();k++) {
							if (temp[k]==t) {     //集合中是否存在当前状态
								z=1;
								break;
							}
						}
						if (z==0) temp.push_back(t);//集合中不存在当前状态则加入集合
					}
				}
				if (temp.size()==0) continue;
				
				sort(temp.begin(),temp.end());    //temp此时为经过某一终结符可达到的状态集合
				l=w=0;
				for (w=0;w<q;w++) {    
					if (dfa_tmp[w].size()==temp.size()) {//元素个数相同时比较temp中的状态集是否已存在
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
					//不相同时添加新的状态集
					for (int x=0;x<temp.size();x++) 
						dfa_tmp[q].push_back(temp[x]);				//将新集合加入
					    dfa[p][i]=q;									 //p起始状态集经过终结符可到达的状态集合
					for (int y=0;y<dfa_tmp[q].size();y++) {
						if (dfa_tmp[q][y]==a.Sint[ed])	{		//判断是否为终态
							ised[q] = true;
							break;
						}
					}
					q++;
				} 
				else {
				dfa[p][i]=w;//相同时为当前状态集
				}
			}
			p++;//改变起始状态
		}
		count=q;//dfa状态数
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


	int fenxi(char * s, int ss,NFA a) {					//分析过程，ss为起始位置
		int l=strlen(s);
		int an;											//token串截止位置
		for (int i=ss;i<l;i++) {
			vector<int> t;
		    t.clear();
			if (s[i]==' '||s[i]=='\n') {
				an=i-1;
				break;
			}
			int num=0,z=0;
			for (int j=ss;j<=i;j++) {
				if (a.sint.find(s[j])==a.sint.end()) {	//遇到没定义的符号退出
					z=1;
					an=i-1;
					break;
				}
				t.push_back(a.sint[s[j]]);				//否则加入t中
				num++;
			}
			if (z==1) break;
			int c=a.Sint[ed];
			for (int k=0;k<num;k++) {
				if (dfa[c][t[k]]==-1) {					//没有状态转化时停止
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
	cout<<"DFA:操作符"<<endl;
	aa.display(a);
	cout<<"DFA:标识符"<<endl;
	bb.display(b);
	cout<<"DFA:数字"<<endl;
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
			int aaa=aa.fenxi(temp,i,a);			//操作符文法匹配符号数
			int bbb=bb.fenxi(temp,i,b);			//标志符文法匹配符号数
			int ccc=cc.fenxi(temp,i,c);			//数字文法匹配符号数
			if (aaa==i-1&&bbb==i-1&&ccc==i-1) {
				if (bundary(temp[i])==true) {
					out<<"("<<line<<"): "<<temp[i++]<<' '<<"界符"<<endl;
					continue;
				}
				out<<"错误:("<<line<<") "<<temp[i]<<endl;
				return 0;
			}

			if (aaa>bbb&&aaa>ccc) {			//运算符匹配的符号最多则为运算符
				strcopy(t,temp,i,aaa);		//将temp的i到aaa copy到t
				out<<"("<<line<<"): "<<t<<' '<<"运算符"<<endl;
				i=aaa+1;
				continue;
			}

			if (bbb>aaa&&bbb>ccc) {
				strcopy(t,temp,i,bbb);
				if (key(t)==true) {
					out<<"("<<line<<"): "<<t<<' '<<"关键字"<<endl;
				} 
				else {
					out<<"("<<line<<"): "<<t<<' '<<"标识符"<<endl;
				}
				i=bbb+1;
				continue;
			}
			if (ccc>aaa&&ccc>bbb) {
				strcopy(t,temp,i,ccc);
				out<<"("<<line<<"): "<<t<<' '<<"数字"<<endl;
				i=ccc+1;
				continue;
			}
		}
		line++;
	}
	out.close();
	cout<<"解析完毕~"<<endl;
	return 0;
}

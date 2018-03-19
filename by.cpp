#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include <fstream>
#include<string>
#include<algorithm>
#define MAX_LENTH 256
#define MAX_NUM 1000
using namespace std;

void JUDGE_ELSE(char ss[]);
int LINE,WORD_CNT,MA,NUM[10]={0,1,2,3,4,5,6,7,8,9},IF_ERROR,WRONG_CNT;

struct WORD_ANALYSIS
{
	char s[MAX_LENTH];
	int sg;
}word_analysis[MAX_NUM];			//存放已经分析好的词素及其种码

char temp[MAX_LENTH],
	 wrong_mes[MAX_NUM][MAX_LENTH],
key_word[14][20]={"void","char","int","float","double","short","return","continue","break","if","else","for","do","while"},
sys_sg[6][20]={"main","printf","scanf","define","include","EOF"},
cal_sg[22][20]={"+","-","*","/","%","!","&","|","=","^",">","<",">=","<=","==","!=","&&","||","+=","-=","*=","/="},
head_sg[5][20]={"#include<>"},
head_na[12][20]={"stdio.h","stdlib.h","string.h","math.h","time.h","windows.h","string","map","algorithm","stack","queue","fstream"},
border_sg[9][20]={",",";","(",")","\"","'","{","}","//"};//数字码为52，用户定义标识符为53

void init()
{
	IF_ERROR=0;		//是否分析出错误
	MA=1;			//所给的标志码
	LINE=0;				//所在行数
	WORD_CNT=0;			//已分析的词素个数
	WRONG_CNT=0;		//错误信息个数
	memset(word_analysis,0,sizeof(word_analysis));
	memset(wrong_mes,0,sizeof(wrong_mes));
}

int JUDGE_KEY(char ss[])
{
	int i;
	for(i=0;i<14;i++)
	{
		if(strcmp(key_word[i],ss) == 0)
			return 1;
		else MA++;
	}
	return 0;
}

int JUDGE_HEAD(char ss[])
{
	char st[MAX_LENTH];
	int i,l=strlen(ss),nn=0;
	if( l < 10 ) return 0;
	for(i=0;i<9;i++)
		if(ss[i] != head_sg[0][i] ) return 0;
	if(ss[l-1] != head_sg[0][9]) return 0;
	for(i=9;i<l-1;i++)
		st[nn++]=ss[i];
	st[nn] = '\0';
	for(i=0;i<12;i++)
		if(strcmp(st,head_na[i]) == 0)
			return 1;
	return 0;
}

int JUDGE_SYSSG(char ss[])
{
	int i;
	for(i=0;i<6;i++)
	{
		if(strcmp(sys_sg[i],ss) == 0)
			return 1;
		else MA++;
	}
	return 0;
}

int JUDGE_CALSG(char ss[])
{
	int i;
	for(i=0;i<22;i++)
	{
		if(strcmp(cal_sg[i],ss) == 0)
			return 1;
		else MA++;
	}
	return 0;
}

int isd(char hh)
{
	int i;
	for(i=0;i<10;i++)
		if( (hh-'0') == NUM[i] )
			return 1;
	return 0;
}

int isz(char hh)
{
	if( ( hh >='a' && hh <='z' ) || ( hh >='A' && hh <='Z' ) )
		return 1;
	return 0;
}

int JUDGE_NUM(char ss[])									//判断数字
{
	int sta=1,i,len=strlen(ss);
	for(i=0;i<len;i++)
	{
		switch(sta)
		{
			case 1:
				{
					if( ss[i] == '+' || ss[i] == '-' || isd(ss[i]) ) sta = 2 ;
					else if( ss[i] == '.' ) sta = 4 ;
					else if( ss[i] == 'e' || ss[i] == 'E' ) sta = 3 ;
					else sta = -1 ;
					break;
				}
			case 2:
				{
					if( isd(ss[i]) ) sta = 2 ;
					else if( ss[i] == '.' ) sta = 4 ;
					else if( ss[i] == 'e' || ss[i] == 'E' ) sta = 3 ;
					else sta = -1 ;
					break;
				}
			case 3:
				{
					if( isd(ss[i]) ) sta = 6 ;
					else if( ss[i] == '+' || ss[i] == '-' ) sta = 5 ;
					else sta = -1 ;
					break;
				}
			case 4:
				{
					if( isd(ss[i]) ) sta = 7 ;
					else sta = -1 ;
					break;
				}
			case 5:
				{
					if( isd(ss[i]) ) sta = 6 ;
					else sta = -1 ;
					break;
				}
			case 6:
				{
					if( isd(ss[i]) ) sta = 6 ;
					else sta = -1 ;
					break;
				}
			case 7:
				{
					if( isd(ss[i]) ) sta = 7 ;
					else if( ss[i] == 'e' || ss[i] == 'E' ) sta = 3 ;
					else sta = -1 ;
					break;
				}
			default : break;
		}
		if( sta == -1 ) break;
	}
	if(i < len )
		return 0;
	if(sta != 2 && sta != 6 && sta != 7)
		return 0;
	MA=52;
	return 1;
}

int JUDGE_USER(char ss[])							//判断用户自定义标识符
{
	int sta=1,i,len=strlen(ss);
	for(i=0;i<len;i++)
	{
		switch(sta)
		{
			case 1:
				{
					if( ss[i] == '_' || isz(ss[i]) ) sta = 2 ;
					else sta = -1 ;
					break;
				}
			case 2:
				{
					if( ss[i] == '_' || isz(ss[i]) || isd(ss[i]) ) sta = 2 ;
					else sta = -1 ;
					break;
				}
			default : break;
		}
		if( sta  == -1 ) break;
	}
	if( i < len || sta != 2 )
		return 0;
	MA=53;
	return 1;
}

int HAVE_BDSG(char tmp[])													 //判断有无界符或运算符
{
	int i,j,l=strlen(tmp);
	for(i=0;i<l-1;i++)
	{
		for(j=12;j<22;j++)
			if(tmp[i] == cal_sg[j][0] && tmp[i+1] == cal_sg[j][1])
				return i+(j+21)*1000;
		if(tmp[i] == tmp[i+1] && tmp[i] == '/')
			return i+51000;	
		for(j=0;j<12;j++)
			if(tmp[i] == cal_sg[j][0])
				return i+(21+j)*1000;
		for(j=0;j<8;j++)
			if(tmp[i] == border_sg[j][0])
				return i+(43+j)*1000;
	}	
	for(j=0;j<12;j++)
			if(tmp[l-1] == cal_sg[j][0])
				return l-1+(21+j)*1000;
		for(j=0;j<8;j++)
			if(tmp[l-1] == border_sg[j][0])
				return l-1+(43+j)*1000;
																			//i为运算符或界符的初始位置
	return -1;
}

void WRITE(char ss[])
{
	strcpy(word_analysis[WORD_CNT].s,ss);
	word_analysis[WORD_CNT].sg=MA;
	WORD_CNT++;
}

void JUDGE_BDSG(int tt,char ss[])
{
	char st[MAX_LENTH];
	int i,l=strlen(ss),tmp=tt%1000,start,ttt;
	if(tmp)
	{
		for(i=0;i<tmp;i++)
			st[i]=ss[i];
		st[tmp]='\0';
		MA=1;
		JUDGE_ELSE(st);
	}
	MA=tt/1000;
	if(MA >=21 && MA <= 42)
	{
		for(i=0;i<22;i++)
			if( i+21 == MA )
				break;
		strcpy(word_analysis[WORD_CNT].s,cal_sg[i]);
		word_analysis[WORD_CNT].sg=MA;
		WORD_CNT++;
	}
	else if(MA >= 43 && MA <= 51)
	{
		for(i=0;i<9;i++)
			if( i+43 == MA )
				break;
		strcpy(word_analysis[WORD_CNT].s,border_sg[i]);
		word_analysis[WORD_CNT].sg=MA;
		WORD_CNT++;
	}
	else printf("所得标志码错误!\n");
	i=tmp+1;
	if( ( MA >= 33 && MA <=42 ) || MA == 51)
		i++;
	start=i;
	if(i >= l) return ;
	for(; i < l ; i++ )
		st[i-start]=ss[i];
	st[i-start]='\0';
	MA=1;
	if(( ttt = HAVE_BDSG(st)) > 0 )
		JUDGE_BDSG(ttt,st);
	else JUDGE_ELSE(st);
}

void err()
{
	IF_ERROR=1;
	sprintf(wrong_mes[WRONG_CNT++],"在第%d行有词法错误!\n",LINE);
}

void JUDGE_ELSE(char ss[])
{
	if(JUDGE_KEY(ss))
		WRITE(ss);
	else if(JUDGE_SYSSG(ss))
		WRITE(ss);
	else if(JUDGE_NUM(ss))
		WRITE(ss);
	else if(JUDGE_USER(ss))
		WRITE(ss);
	else err();
}

void BODY()
{
	int len;
	char ch;
	while( ( ch = getchar() ) != EOF )
	{
		MA=1;
		len=0;
		while( ch == 32 || ch == '\t' || ch == '\n' )
			ch = getchar();
		while(ch != 32 && ch != '\t' && ch != '\n' )
		{
			temp[len++]=ch;
			ch=getchar();
		}
		if(ch == '\n')
			LINE++;
		int tt;
		temp[len]='\0';
		if( JUDGE_HEAD(temp) )
		{
			MA=0;
			WRITE(temp);
		}
		else if( ( tt= HAVE_BDSG(temp) ) != -1)
			JUDGE_BDSG(tt,temp);
		else
			JUDGE_ELSE(temp); 
	}
}

int main()
{
	freopen("C:\\Users\\user\\Desktop\\in.cpp","r",stdin);
	//freopen("C:\\Users\\user\\Desktop\\out.cpp","w",stdout);
	int i,f=0;
	FILE * tp, *tpp;
	tp = fopen("C:\\Users\\user\\Desktop\\out.cpp","w");
	tpp = fopen("E:\\by\\link.txt","w+");
	fprintf(tp,"--------------------词法分析器--------------------\n");
	init(); 
	BODY();  
	if(!IF_ERROR)
		fprintf(tp,"所给程序没有词法错误\n");
	else
		fprintf(tp,"所给程序存在词法错误\n\n");
	for(i=0;i<WRONG_CNT;i++)
		fprintf(tp," %s\n",wrong_mes[i]);
	fprintf(tp,"  词素种码  词素值  \n");
	for(i=0;i<WORD_CNT;i++)
	{
		fprintf(tp," (    %d  ,  %s   )\n",word_analysis[i].sg,word_analysis[i].s);
		if(!f && word_analysis[i].sg >= 21 && word_analysis[i].sg <= 42)
		{
			fprintf(tpp,"%d",30);
			f=1;
		}
		else if(word_analysis[i].sg >= 21 && word_analysis[i].sg <= 42)
			fprintf(tpp," %d",30);
		else if(f) fprintf(tpp," %d",word_analysis[i].sg);
		else { fprintf(tpp,"%d",word_analysis[i].sg); f=1; }
	}
	fprintf(tpp,"\n");
	fprintf(tp,"\n词法分析完成!\n");
	fclose(tp);
	fclose(tpp);
	return 0;
}

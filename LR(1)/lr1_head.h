#include<bits/stdc++.h>
using namespace std;
//���������ṹ��ֱ��ʾ�ս�������ս��������ʽ
//�ս��
typedef struct FINAL{
    char final[10]="";
    int num;
} FINAL;

//���ս��
typedef struct UNFINAL{
    char unfinal[10]="";
    int num;
} UNFINAL;

//����ʽ
typedef struct EXP{
    int num;
    char expression[20]="";
} EXP;

char start; //��ʼ����
char dot = '.';
map<char,vector<string>> rule;
//map<char,vector<string>> rule2;
map<char,set<char>> first;
map<char,set<char>> follow;
set<char> Vt;
set<char> Vn;
//unordered_set<char> V;
vector<char> V;
vector<char> Vnn;
vector<char> Vtt;
string input_string;
int at[20][20]; //action��
string act[20][20]; //action�������ʽ
int gt[20][20]; //goto��
//bool FlagofAction = false;

struct rule2{  //����closure�Ĺ���
    char left;
    string right;
    char future;
}rul[100];

struct rule3{  //���ʽ���������ķ�������
	char left;
	string right;
}ru[100];
int NumofRu = 0;

vector<rule2> minii_closure;  //����action��Ĺ���
vector<vector<rule2>> closure(100); //�洢�Զ���closure
int closure_num = 0;  //���ж��ٸ�closure
int closure_num_move = 0; //���ڽ��е����ĸ�closure
int f=0;
int ff=0;
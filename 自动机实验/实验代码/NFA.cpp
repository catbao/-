#include<bits/stdc++.h>
using namespace std;
const int Max = 50;

//����NFA��DFA�Ľṹ��
typedef struct NFA{
    int NumOfState;
    int NumOfTransfer;
    int matrix[Max][Max][Max];
    int begin[Max];
    int LengthOfBegin=0;
    int end[Max];
    int LengthOfEnd=0;
}NFA;
typedef struct DFA{
    int NumOfState;
    int NumOfTransfer;
    int matrix[Max][Max][Max];
    int begin;  //DFA�ĳ�ֻ̬��һ��
    int end[Max];
    int LengthOfEnd=0;
}DFA;

void NFA_input(NFA &nfa){
    scanf("%d",&nfa.NumOfState);
	scanf("%d",&nfa.NumOfTransfer);
	int src;
	int target;
	while(scanf("%d",&src)&&src!=-1){
		int edge;
		scanf("%d",&edge);
		while(scanf("%d",&target)&&target!=-1){
			nfa.matrix[src][edge][target]=1;
		}
	}
	while(scanf("%d",&src)&&src!=-1){
		nfa.begin[nfa.LengthOfBegin++]=src;
	}
	while(scanf("%d",&target)&&target!=-1){
		nfa.end[nfa.LengthOfEnd++]=target;
	}
}
int check(string a,int b){ //�ж��Ƿ���ֹ�
    for(int i=0;i<a.length();++i){
        if(a[i] == b + '0') return 0;
    }
    return 1;
}
int ExistEnd(NFA nfa,int a){ //�ж��Ƿ�����̬
	for(int i=0;i<nfa.LengthOfEnd;i++){
		if(a==nfa.end[i]) return 1;
	}
	return 0;
}
void e_closure(NFA nfa,int a,string &temp){
    stack<int> s;
    for(int i=0;i<temp.size();i++){
		s.push(temp[i]-'0');
	}
	while(!s.empty()){
		int tem=s.top();
		s.pop();
		for(int i=0;i<nfa.NumOfState;i++){
			if(nfa.matrix[tem][0][i]==1){
				if(check(temp,i)){
					temp+=i+'0'; 
					s.push(i);
				}
			}
		}
	}
}
string move(NFA nfa,int a,string &temp,bool &endflag){
    string res="";
	for(int i=0;i<temp.size();i++){
		int tem=temp[i]-'0';
		for(int i=0;i<nfa.NumOfState;i++){
			if(nfa.matrix[tem][a][i]==1){
				if(check(res,i)){
					res+=i+'0'; 
				}
				if(ExistEnd(nfa,i)){
					endflag=true;
				}
			}
		}
	}
	return res;
}
void NFA2DFA(NFA nfa,DFA &dfa){
    dfa.NumOfTransfer = nfa.NumOfTransfer;
    string temp = "";
    for(int i=0;i<nfa.LengthOfBegin;++i){
        temp += nfa.begin[i] + '0';
    }
    e_closure(nfa,0,temp);
    map<string,int> bai; // ʹ��map�鿴�²����Ĵ��Ƿ���ֹ������
    bai[temp] = 0; 
    dfa.begin = 0;  //����̬���Ϊ0
    dfa.NumOfState = 1;  //��ʱdfa��ֻ��һ��״̬
    queue<string> q;
    q.push(temp);
    while(!q.empty()){
        temp = q.front();
        q.pop();
        bool endflag = false; //�Ƿ񵽴���̬
        int start = bai[temp];
        for(int i=1;i<=nfa.NumOfTransfer;++i){
            string temp2 = ""; 
            temp2 = move(nfa,i,temp,endflag);
            e_closure(nfa,0,temp2);
            if(bai[temp2] == 0 && temp2 != temp){
                q.push(temp2);
                bai[temp2] = dfa.NumOfState++;
                dfa.matrix[start][i][bai[temp2]] = 1;
                if(endflag){
					dfa.end[dfa.LengthOfEnd++]=bai[temp2];
				}
            }
            //�õ�֮ǰ�й���״̬
            else if(bai[temp2]!=0){
                dfa.matrix[start][i][bai[temp2]] = 1;
            }
        }
    }
}
void DFA_output(DFA dfa){
    cout<<"DFA"<<endl;
	cout<<"״̬����:"<<dfa.NumOfState<<endl;
	cout<<"�ַ������:"<<dfa.NumOfTransfer<<endl;
	cout<<"״̬ת��:"<<endl;
	for(int i=0;i<dfa.NumOfState;i++){
		for(int j=1;j<=dfa.NumOfTransfer;j++){
			for(int k=0;k<dfa.NumOfState;k++){
				if(dfa.matrix[i][j][k]==1){
					printf("(%3d,%3d)->%3d\n",i,j,k);
				}
			}
		}
	}
	cout<<"��ʼ״̬:"<<0<<endl;
	cout<<"����״̬��:{";
	for(int i=0;i<dfa.LengthOfEnd;i++){
		cout<<dfa.end[i];
		if(i!=dfa.LengthOfEnd-1){
			cout<<',';
		}
	}
	cout<<"}"<<endl;
}

int main(){
    NFA nfa;
    NFA_input(nfa);
    DFA dfa;
    NFA2DFA(nfa,dfa);
    DFA_output(dfa);
    system("pause");
    return 0;
}
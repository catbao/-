#include<stdio.h>
#include<stdlib.h>
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

string str;

char start; //��ʼ����
map<char,vector<string>> rule;
map<char,set<char>> first;
map<char,set<char>> follow;
set<char> Vt;
set<char> Vn;

void trim(string &s){
int index = 0;
if( !s.empty()){
    while( (index = s.find(' ',index)) != string::npos)
    s.erase(index,1);
    }
}

void GetRule(EXP *exp){
    for(int i=1;i<=exp[0].num;++i){
        string right = "";
        int j=5;
        while(exp[i].expression[j] != '\0'){
            right += exp[i].expression[j];
            j++;
        }
        trim(right);
        rule[exp[i].expression[0]].push_back(right);
    }
    // for(map<char,vector<string>>::iterator
    // it=rule.begin();it!=rule.end();it++){
    //     cout<<it->first<<"->"<<it->second.size()<<endl;
    // }
}

void make_first(){
    //��ʼ��first��
    for(set<char>::iterator it = Vn.begin();it!=Vn.end();it++){
        set<char> t;
        first.insert(make_pair(*it,t));
    }
    //��������rule
    //2.��X��Vn,��X��a��,(a��Vt)��/��X���� ��a��/�ͦš�FIRST(x)
    //�� �� @ ������
    for(map<char,vector<string>>::iterator it = rule.begin();it!=rule
    .end();it++){
        //��tempp��¼it->first��Ӧ��first��
        set<char> tempp;
        vector<string> right = it->second;
        char left = it->first;
        for(int i=0;i<right.size();i++){
            if(Vt.count(right[i][0])!=0){
                tempp.insert(right[i][0]);
            }
            if(right[i] == "@"){
                tempp.insert('@');
            }
        }
        if(first.at(left).size() == 0){
            first.at(left) = tempp;
        }
        else{
            first.at(left).insert(tempp.begin(),tempp.end());
        }
    }
     //ѭ������������ ֱ��ÿһ��FIRST(X)��������Ϊֹ
    //3.��X��Y0Y1����.Yk(��Y0 ��Vn), ����ʹ������ֱ��ÿһ��FIRST(X)��������Ϊֹ
    set<char> temp[9];
    int k=0,first_size=0;
    for(map<char,vector<string>>::iterator it = rule.begin();;it++,k++){
         //��temp[k]��¼it->first��Ӧ��first��
        vector<string> right = it->second;
        char left = it->first;
         //3.��X��Y0Y1����.Yk(��Y0 ��Vn), ����ʹ������ֱ��ÿһ��FIRST(X)��������Ϊֹ
        bool flag = true; //���������{��}��Yj
        for(int i=0;i<right.size();++i){
            for(int j=0;i<right[i].size()&&(Vn.count(right[i][j])>0)&&flag;j++){
                set<char> temp1 = first.at(right[i][j]);
                //i ��Y0��Vn  ���FIRST(Y0)\{��}Ԫ�ؼ���FIRST(X)��
                if(j == 0){
                     //\{��}
                    if(temp1.count('@')>0){
                        temp1.erase('@');
                    }
                    //Y0����{��}
                    else{
                        flag = false;
                    }
                     //FIRST(Y0)\{��}
                    temp[k].insert(temp1.begin(),temp1.end());
                }
                /*
                ii ��Y1��Y2������Y i-1 ��Vn(2��i ��k) �Ҧ� ��FIRST(Y j) (1��j ��i-1)
                    ���FIRST(Yi )\{��}Ԫ�ؼ���FIRST(x)��
                 */
                else if(j<right[i].size()-1){
                     //Y0...Yj-1��{��}
                    if(temp1.count('@')>0){
                        temp1.erase('@');
                    }
                    //Yj����{��}
                    else{
                        flag = false;
                    }
                      //FIRST(Yj)\{��}
                    temp[k].insert(temp1.begin(),temp1.end());
                }
                /*
                iii ��Y0��Y1������Yk ��Vn�Ҧ� ��FIRST(Yj)(0��j ��k)
                    ��Ѧ�Ԫ�ؼ���FIRST(x)��
                 */
                else{
                    temp[k].insert('@');
                }
            }
        }
        if(first.at(left).size() == 0){
            first.at(left) = temp[k];
        }
        else{
            first.at(left).insert(temp[k].begin(),temp[k].end());
        }

        if(k == first.size()-1){
            int cnt = 0;
            for(int kk=0;kk<=k;kk++){
                cnt += temp[kk].size();
            }
             //����һ�δ�Сһ����ѭ����ֹ
            if(cnt == first_size) break;
            else{
                k=0;
                it = rule.begin();
                first_size = cnt;
            }
        }
    }

    // cout<<endl;
    // cout<<endl;
    // cout<<"First Set:"<<endl;
    // for(auto it = Vn.begin();it!=Vn.end();++it){
    //     //cout<<first[*it].size()<<" ";
    //     cout<<"First("<<*it<<"):" ;
    //     for(auto itt = first[*it].begin();itt != first[*it].end();itt++){
    //         cout<<*itt<<" ";
    //     }
    //     cout<<endl;
    // }
}

void make_follow(){
    for(set<char>::iterator it = Vn.begin();it != Vn.end();++it){
        set<char> t;
         //��#��FOLLOW(S) SΪ�ķ���ʼ����
        if(*it==start){
            t.insert('#');
        }
        follow.insert(make_pair(*it,t));
    }

     //��������rule
    //2.��A�� ��B��,�Ҧ� �٦�, �� FIRST(��)\{��}����FOLLOW(B)��
    for(map<char,vector<string>>::iterator it = rule.begin();it!=rule.end();++it){
        vector<string> right = it->second;
        char left = it->first;
        for(int i=0;i<right.size();++i){
            for(int j=0;j<right[i].size();j++){
                if(Vn.count(right[i][j]) != 0){
                    set<char> tt;
                      //������Vt
                    if(Vt.count(right[i][j+1]) != 0){
                        tt.insert(right[i][j+1]);
                        follow.at(right[i][j]).insert(tt.begin(),tt.end());
                    }
                    //������Vn,FIRST(��)\{��}����FOLLOW(B)��
                    if(Vn.count(right[i][j+1]) != 0){
                        tt = first.at(right[i][j+1]);
                        tt.erase('@');
                        follow.at(right[i][j]).insert(tt.begin(),tt.end());
                    }
                }
            }
        }
    }

        // cout<<endl;
        // cout<<endl;
        // cout<<"Follow Set"<<endl;
        // for(auto it = Vn.begin();it!=Vn.end();++it){
        //     //cout<<follow[*it].size()<<" ";
        //     cout<<"Follow("<<*it<<"):" ;
        //     for(auto itt = follow[*it].begin();itt != follow[*it].end();itt++){
        //         cout<<*itt<<" ";
        //     }
        //     cout<<endl;
        // }

     // 3.����, ֱ��ÿһ��FOLLOW(A)��������
    //   ��A�� ��B��A�� ��B��(�Ҧ� �� FIRST(��))
    //   ��FOLLOW(A)�е�ȫ��Ԫ�ؼ���FOLLOW(B)
        int k=0;
        int follow_size = 0;
        for(map<char,vector<string>>::iterator it = rule.begin();;it++,k++){
            vector<string> right = it->second;
            char left = it->first;
            // for(int i=0;i<right.size();++i){
            //     for(int j=0;j<right[i].size();++j){
            //         cout<<right[i][j]<<" ";
            //     }
            //     cout<<endl;
            // }
            for(int i=0;i<right.size();++i){
                for(int j=0;j<right[i].size();j++){
                    //cout<<endl<<right[i].size();
                    if(Vn.count(right[i][j]) != 0){
                        //cout<<"  yes"<<endl;
                        set<char> tt;
                         //����������һ���ַ�
                        
                        if(j == right[i].size() - 1){
                            //���һ���ַ���Vn����A����B����FOLLOW(A)�е�ȫ��Ԫ�ؼ���FOLLOW(B)
                            tt = follow.at(left);
                            follow.at(right[i][j]).insert(tt.begin(),tt.end());
                        }
                          //û�����һ��,��A�� ��B��(�Ҧš�FIRST(��)) �������
                        else {
                            bool flag = true;
                            //�����š�FIRST(��)�Ƿ����
                            for(int jj=j+1;jj<right[i].size();jj++){
                                 //right[i][jj]��Vn�Ұ�����
                                if(Vn.count(right[i][jj])!=0 && first.at(right[i][jj]).count('@')!=0){
                                    continue;
                                }
                                  //right[i][jj]����Vn�򲻰�����
                                else{
                                    flag = false;
                                }
                            }
                              //A�� ��B��(�Ҧš�FIRST(��))�������
                            if(flag){
                                tt = follow.at(left);
                                follow.at(right[i][j]).insert(tt.begin(),tt.end());
                            }
                        }
                    }
                }
            }
            if(k == follow.size() - 1){
                int cnt = 0;
                for(map<char,set<char>>::iterator it1 = follow.begin();it1!=follow.end();it1++){
                    cnt += it1->second.size();
                }
                 //����һ�δ�Сһ����ѭ����ֹ
                if(cnt == follow_size) break;
                else{
                    k = 0;
                    it = rule.begin();
                    follow_size = cnt;
                }
            }
        }
    
        // cout<<endl;
        // cout<<endl;
        // cout<<"Follow Set"<<endl;
        // for(auto it = Vn.begin();it!=Vn.end();++it){
        //     //cout<<follow[*it].size()<<" ";
        //     cout<<"Follow("<<*it<<"):" ;
        //     for(auto itt = follow[*it].begin();itt != follow[*it].end();itt++){
        //         cout<<*itt<<" ";
        //     }
        //     cout<<endl;
        // }
}

string cal_select(string temp){
    string a = "";
    if(Vt.count(temp[3])!=0){  //ֱ��
        a += temp[3];
        return a;
    }
    else if(temp[3] == '@'){
        set<char> t = follow[temp[0]];
        for(auto it = t.begin();it!=t.end();it++){
            a += " ";
            a += (*it);
        }
    }
    else{
        set<char> t = first[temp[3]];
        if(auto itt = t.find('@') != t.end()){
            t.erase('@');
            for(auto it = t.begin();it!=t.end();it++){
                a += " ";
                a += (*it);
            }
            set<char> tt = first[temp[0]];
            for(auto ittt = t.begin();ittt!=t.end();ittt++){
                a += " ";
                a += (*ittt);
            }
        }
        else{
            for(auto it = t.begin();it!=t.end();it++){
                a += " ";
                a += (*it);
            }
        }
    }
    return a;
}

void make_select(){
    cout<<endl;
    cout<<"Select Set:"<<endl;
    for(auto it = rule.begin();it!=rule.end();it++){
        vector<string> temp = it->second;
        char c = it->first;
        string str;
        stringstream stream;
        stream << c;
        str = stream.str();  //���ַ�תΪ�ַ�����ʽ
        string tem = str + "->";
        for(int i=0;i<temp.size();++i){
            tem += temp[i];
            cout<<"SELECT("<<tem<<")"<<": "<<cal_select(tem)<<endl;
            tem = str + "->";
        }
        temp.clear();
    }
}

void make_table(){
    cout<<endl;
    cout<<"Ԥ�������"<<endl<<endl;
    cout<<"\t ";
    for(auto it=Vt.begin();it!=Vt.end();it++){
        cout<<*it<<"         ";
    }
    cout<<endl;
    cout<<"-------------------------------------------------------------"<<endl;
    map<pair<char,char>,string> bai;
    for(auto it1=rule.begin();it1!=rule.end();it1++){
        vector<string> right = it1->second;
        for(int i=0;i<right.size();++i){
            if(Vt.find(right[i][0])!=Vt.end()){//���ֱ�����ս��
                bai[pair<char,char>(it1->first,right[i][0])] = right[i];
            }
            else if(right[i][0] == '@'){//����ǿ���
                set<char> temp = follow[it1->first];
                for(auto it2 = temp.begin(); it2!=temp.end(); it2++){
                    bai[pair<char,char>(it1->first,*it2)] = "@";
                }
            }
            else{ //�Ƿ��ս�������
                set<char> temp = first[right[i][0]];
                for(auto it3 = temp.begin();it3!=temp.end();it3++){
                    bai[pair<char,char>(it1->first,*it3)] = right[i];
                }
            }
        }
    }
    // for(auto it4 = bai.begin(); it4!=bai.end();it4++){
    //         cout<<it4->second<<endl;
    // }
    for(auto it4 = Vn.begin();it4!=Vn.end();it4++){
        cout<<*it4<<"\t";
        for(auto it5=Vt.begin();it5!=Vt.end();it5++){
            if(bai.find(pair<char,char>(*it4,*it5)) != bai.end()){
                cout<<*it4<<"->"<<bai[pair<char,char>(*it4,*it5)]<<"       ";
            }
            else cout<<"        ";
        }
        cout<<endl;
    }
    cout<<"-------------------------------------------------------------"<<endl;
    cout<<endl;


    int flag = 0;
    if(flag == 1) return;
    //str�ķ�������
    cout<<str<<"��������:"<<endl<<endl;
    cout<<"��ʼ����#��ջ,"<<start<<"��ջ"<<endl;
    stack<char> s;
    s.push('#');
    s.push(start);
    int j = 0,index = 0;
    while(!s.empty()){
        char ch;
        ch = s.top();
        s.pop();
        if(ch == str[j]){
            if(ch == '#'){
                cout<<++index<<":��ջX=#�� ����c=#��ƥ�䣬�ɹ���"<<endl;
                continue;
            } 
            cout<<++index<<":��ջX="<<ch<<", "<<"����c="<<str[j]<<", ƥ�䣬����ָ����ƣ�"<<endl;
            j++;
        }
        else if(bai.find(pair<char,char>(ch,str[j]))==bai.end()){
            cout<<"ƥ��ʧ��"<<endl;
            flag=1;
            break;
        }
        else{
            string temp = bai[pair<char,char>(ch,str[j])];
            if(temp == "@"){//������ֿ���
                cout<<++index<<":��ջX="<<ch<<", "<<"����c="<<str[j]<<", ���M[X,c]="
                <<ch<<"->"<<temp<<";"<<endl;
                continue;
            }
            string temp2 = temp;
            //cout<<str[j]<<endl;
            //cout<<temp.length();
            reverse(temp.begin(),temp.end());
            for(char c : temp){
                s.push(c);
            }
            cout<<++index<<":��ջX="<<ch<<", "<<"����c="<<str[j]<<", ���M[X,c]="
            <<ch<<"->"<<temp2<<", ����ʽ�Ҳ�������ջ��"<<endl;
        }
    }
}

void output(UNFINAL nf,FINAL yf,EXP *exp,char start){

	//��ӡ���ս��
	printf("���ս����\n");
    printf("%s\n", nf.unfinal);

    //��ӡ�ս��
    printf("�ս����\n");
    printf("%s\n", yf.final);

    //��ӡ����ʽ����
    printf("����ʽ:\n");
    int k = 0;
    int expnum = exp[k].num;
    for (k = 1; k <= expnum; k++){
        printf("%d��%s\n", k-1,exp[k].expression);
    }

    //��ӡ��ʼ����
    printf("��ʼ����:\n");
    printf("%c", start);
}

void input(){
    int i;
    FINAL yf;
    UNFINAL nf;
    EXP exp[20];
    //������ս��
    scanf("%d", &nf.num);
    for (i = 0; i < nf.num * 2; i++){
        scanf("%s", &nf.unfinal[i]);
        Vn.insert(nf.unfinal[i]);
        i++;
        nf.unfinal[i] = ' ';
    }

    //�����ս��
    scanf("%d", &yf.num);
    for (i = 0; i < yf.num * 2; i++){
        scanf("%s", &yf.final[i]);
        Vt.insert(yf.final[i]);
        i++;
        yf.final[i] = ' ';
    }

    //�������ʽ����
    scanf("%d", &exp[0].num);
    getchar();
    for (i = 1; i <= exp[0].num; i++){
        char ch[20];
        gets(ch);
        int temp = 0, j = 0;
        while (ch[j] != '\0'){
            if (ch[j] >= '0' && ch[j] <= '9'){
                exp[i].expression[temp] = '-';
                temp++;
                exp[i].expression[temp] = '>';
                temp++;
                j++;
            }
            else{
                exp[i].expression[temp] = ch[j];
                temp++;
                j++;
            }
        }
        exp[i].expression[temp] = '\0';
    }

    GetRule(exp);

    //���뿪ʼ����
    scanf("%s", &start);

    cin>>str;
    str += "#";

    //����ķ�
    //output(nf,yf,exp,start);

    //OutPutFirstSet(nf,yf,exp);
}

int main(){
    //������@���ű�ʾ
    input();
    make_first();
    make_follow();
    make_table();
    cout<<endl;
    system("pause");
    return 0;
}


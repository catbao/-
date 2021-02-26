#include "lr1_head.h"
#include<bits/stdc++.h>
using namespace std;

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
        ru[NumofRu].left = exp[i].expression[0];
        ru[NumofRu].right = right+".";
        NumofRu++;
    }
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
}

bool equal(){  //�ж�closure�Ƿ���֮ǰ����ͬ
	int end = closure_num;
	if(closure_num == 0) 
		return false; 
	bool flag;
	for(int i=0;i<end;++i){
		flag = true;
		if(closure[end].size() != closure[i].size()){
			flag = false;
			continue;
		}
		else{
			for(int j=0;j<closure[i].size();j++){
				if(closure[i][j].left != closure[end][j].left||
					closure[i][j].right != closure[end][j].right||
					closure[i][j].future != closure[end][j].future
				){
					flag = false;
					break;
				}
			}
		}
		if(flag == true) break;
	}
	return flag;
}

void Closure(int indexx){  //����closure
    int count = indexx;
    for(int i=0;i<count;++i){  
        string right = rul[i].right;     
        char fut = rul[i].future; 
        int point = right.find(".");
        if (point == -1) cout<<"LR1��Ŀ����";
        else{
            if(point != right.length()-1){
                char next = right[point+1];
                if(Vn.find(next) != Vn.end()){
                    for(auto it = rule.begin();it!=rule.end();it++){
                        if(it->first == next){
                            vector<string>temp = it->second;
                            if(Vn.find(right[point+2])!=Vn.end()){  //���point+2λ���Ƿ��ս��
                                set<char> t = first[right[point+2]]; //����first����
                                for(int j=0;j<temp.size();++j){
                                    for(auto it = t.begin();it!=t.end();it++){
                                        rul[count].left = next;
                                        rul[count].right = "."+temp[j];
                                        rul[count].future = *it;
                                        count++;
                                        for(int k=0;k<count-1;++k){
                                            if(rul[count-1].left == rul[k].left &&
                                            rul[count-1].right == rul[k].right &&
                                            rul[count-1].future == rul[k].future){
                                                count--;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }

                            else if(Vt.find(right[point+2])!=Vt.end()){  //���point+2λ�����ս��
                                for(int j=0;j<temp.size();++j){
                                    rul[count].left = next;
                                    rul[count].right = "."+temp[j];
                                    rul[count].future = right[point+2];
                                    count++;
                                    for(int k=0;k<count-1;++k){
                                        if(rul[count-1].left == rul[k].left &&
                                        rul[count-1].right == rul[k].right &&
                                        rul[count-1].future == rul[k].future){
                                            count--;
                                            break;
                                        }
                                    }
                                }
                            }

                            else{
                                for(int j=0;j<temp.size();++j){
                                    rul[count].left = next;
                                    rul[count].right = "."+temp[j];
                                    rul[count].future = fut;
                                    count++;
                                    for(int k=0;k<count-1;++k){
                                        if(rul[count-1].left == rul[k].left &&
                                        rul[count-1].right == rul[k].right &&
                                        rul[count-1].future == rul[k].future){
                                            count--;
                                            break;
                                        }
                                    }
                                }
                            }
                            
                        }
                    }
                }
            }
        }
    }
    
    f++;
	    if(f==1){  //����洢��closure��
	    	for(int i=0;i<count;++i){
	        	closure[closure_num].push_back(rul[i]);
		    }
		    //closure_num++;
		    //count = 0;
		    f=0;
		}
		
    if(!equal()){  //���֮ǰû�г��ֹ�
    	cout<<endl;
    	printf("I%d:\n",closure_num);
	    for(int i=0;i<count;++i){
	        cout<<"    "<<rul[i].left<<"->"<<rul[i].right<<","<<rul[i].future<<endl;
	    }  
	    //FlagofAction = true;
	}
	else{  //���֮ǰ�й��ظ���
		for(int i=0;i<count;++i){
			closure[closure_num].pop_back();
		}
		closure_num--;
	}
    
    //equal();
    //cout<<(closure[0] == closure[0])<<endl;
    //cout<<(closure[0] != closure[0])<<endl;
    //return count;
}

bool closure_equal(vector<rule2> a, vector<rule2> b){ //���ڼ���actionʱʹ��
	if(a.size() != b.size()) return false;
	for(int i=0;i<a.size();++i){
		if(a[i].left != b[i].left || a[i].right != b[i].right || a[i].future != b[i].future)
			return false;
	}
	return true;
}

bool closure_equal2(vector<rule2> a, vector<rule2> b){
	if(a.size() != b.size()) return false;
	bool flag;
	for(int i=0;i<a.size();++i){
		flag = false;
		for(int j=0;j<a.size();++j){
			if(a[i].left == b[j].left && a[i].right == b[j].right && a[i].future == b[j].future){
				flag = true;
				break;
			}
		}
		if(flag == false) return false;
	}
	return flag;
}

int Closure2(int indexx){  //����actionʱʹ��
    int count = indexx;
    for(int i=0;i<count;++i){  
        string right = rul[i].right;     
        char fut = rul[i].future; 
        int point = right.find(".");
        if (point == -1) cout<<"LR0��Ŀ����";
        else{
            if(point != right.length()-1){
                char next = right[point+1];
                if(Vn.find(next) != Vn.end()){
                    for(auto it = rule.begin();it!=rule.end();it++){
                        if(it->first == next){
                            vector<string>temp = it->second;
                            if(Vn.find(right[point+2])!=Vn.end()){
                                set<char> t = first[right[point+2]];
                                for(int j=0;j<temp.size();++j){
                                    for(auto it = t.begin();it!=t.end();it++){
                                        rul[count].left = next;
                                        rul[count].right = "."+temp[j];
                                        rul[count].future = *it;
                                        count++;
                                        for(int k=0;k<count-1;++k){
                                            if(rul[count-1].left == rul[k].left &&
                                            rul[count-1].right == rul[k].right &&
                                            rul[count-1].future == rul[k].future){
                                                count--;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }

                            else if(Vt.find(right[point+2])!=Vt.end()){
                                for(int j=0;j<temp.size();++j){
                                    rul[count].left = next;
                                    rul[count].right = "."+temp[j];
                                    rul[count].future = right[point+2];
                                    count++;
                                    for(int k=0;k<count-1;++k){
                                        if(rul[count-1].left == rul[k].left &&
                                        rul[count-1].right == rul[k].right &&
                                        rul[count-1].future == rul[k].future){
                                            count--;
                                            break;
                                        }
                                    }
                                }
                            }

                            else{
                                for(int j=0;j<temp.size();++j){
                                    rul[count].left = next;
                                    rul[count].right = "."+temp[j];
                                    rul[count].future = fut;
                                    count++;
                                    for(int k=0;k<count-1;++k){
                                        if(rul[count-1].left == rul[k].left &&
                                        rul[count-1].right == rul[k].right &&
                                        rul[count-1].future == rul[k].future){
                                            count--;
                                            break;
                                        }
                                    }
                                }
                            }
                            
                        }
                    }
                }
            }
        }
    }
    ff++;
	if(ff==1){  
	    for(int i=0;i<count;++i){
	        minii_closure.push_back(rul[i]); //����minii_closure
		}
		ff=0;
	}
    for(int i=0;i<closure_num;++i){
    	if(closure_equal2(minii_closure,closure[i])){  //�ж��ܹ��ִ��ĸ�״̬�����ж��Ƿ��ĳһ��״̬���
    		return i;
		}
	}
	return -1;
}

int go_to(char x,bool &flag){  //����һ��״̬�������������x���������������ʽ
    int index=0;
    vector<rule2> tem = closure[closure_num_move];
    for(int i=0;i<tem.size();++i){
        string str = tem[i].right;
        //cout<<str[0]<<" "<<str[1]<<endl;
        int point = 0;
        for(int j=0;j<1000;++j){
            if(str[j] == '.'){
            	point = j; 
                break;
			}   
        }
        //cout<<point<<endl;
        //cout<<str[point+1]<<endl;
        //int index = 0;
        if(point != str.length()-1 && str[point+1] == x){
            flag = true;
            rul[index].left = tem[i].left;
            swap(tem[i].right[point],tem[i].right[point+1]);
            rul[index].right = tem[i].right;
            //tem[i].right.insert(point+1,".");
            rul[index].future = tem[i].future;
            //Closure(index+1);
            index++;
            //in = index;
        }
    }
    return index;  //���ز���ʽ�ĸ���
}

void items(){  //����Ŀ��
	cout<<endl<<endl<<"[LR(1) item set cluster]";
    int index = 0;
    rul[index].left = start;
    rul[index].right = "." + rule[start].front();
    rul[index].future = '#';
    Closure(index+1);  //����հ�
//    for(auto it = Vn.begin();it!=Vn.end();it++){
//        V.push_back();
//    }
//    for(auto it = Vt.begin();it!=Vt.end();it++){
//        V.insert(*it);
//    }
    for(int i=0;i<=closure_num;++i){
    	for(auto it = V.begin();it!=V.end();it++){
	        bool flag=false;
	        int num = go_to(*it,flag);
	        if(flag){
	            //cout<<endl;
	            //for(int i=0;i<num;++i){
	                //cout<<rul[i].left<<"->"<<rul[i].right<<","<<rul[i].future<<endl;   
	            //}
	            closure_num++;
	            Closure(num);  //����������ı��ʽ��������հ�
//	            if(FlagofAction){
//	            	if(*it=='=') act[i][1]=closure_num;
//		            else if(*it=='*') act[i][2]=closure_num;
//		            else if(*it=='i') act[i][3]=closure_num;
//				}
	            
	            //num = 0;
	        }
	    }
	    closure_num_move++;
	}
    
}

void back(){  //��action������at�������act
	for(int i=1;i<=closure_num;++i){
		if(closure[i][0].left == start) {
			at[i][0] = 999;
			continue;
		}
		for(int j=0;j<closure[i].size();++j){
			string str = closure[i][j].right;
			int point = str.find(".");
			if(point == str.size()-1){
				for(int k=0;k<NumofRu;k++){
					if(closure[i][j].left == ru[k].left && closure[i][j].right == ru[k].right){
						for(int m=0;m<Vtt.size();++m){
							if(closure[i][j].future == Vtt[m]){
								at[i][m] = k*(-1);
							}
						}
					}
				}
			}
		}
	}
	for(int i=0;i<=closure_num;++i){
		for(int j=0;j<Vtt.size();++j){
			if(at[i][j] == 0){
				act[i][j] = "  ";
			}
			else if(at[i][j] == 999){
				act[i][j] = "acc";
			}
			else if(at[i][j]>0){
				int a = at[i][j];
			    string res;
			    stringstream ss;          //������ss
			    ss << a;                  //������aת������ss
			    ss >> res;                //����ssת�����ַ���
				act[i][j] = "s"+res;
			}
			else if(at[i][j]<0){
				int a = abs(at[i][j]);
			    string res;
			    stringstream ss;          //������ss
			    ss << a;                  //������aת������ss
			    ss >> res;                //����ssת�����ַ���
				act[i][j] = "r"+res;
			}
		}
	}
}

int action_aiming(int i,int j){ //����i��j�ܵ����ĸ�״̬
	int index = 0;
	for(int k=0;k<closure[i].size();++k){
		int point = closure[i][k].right.find(".");
		if(point != closure[i][k].right.length()-1){
			if(closure[i][k].right[point+1] == Vtt[j]){
			    rul[index].left = closure[i][k].left;
			    string str = closure[i][k].right;
			    swap(str[point],str[point+1]);
			    rul[index].right = str;
			    rul[index].future = closure[i][k].future;
			    index++;
			}
		}
	}
	if(index!=0){
		int num = Closure2(index);
		return num;
	}
	return -1;
}

void action_table(){  //����action��
	cout<<endl<<"[LR(0) analytical table]"<<endl;
	cout<<"Action:"<<endl;
	cout<<"      # ";
	for(int i=1;i<Vtt.size();++i){
		//cout<<Vtt[i]<<"    ";
		printf("%5c",Vtt[i]);
	}
	cout<<endl;
	
	for(int i=0;i<=closure_num;++i){
		for(int j=0;j<Vtt.size();j++){
			int num = action_aiming(i,j);
			minii_closure.clear();
			if(num != -1){
				at[i][j] = num;
			}
			else{
				at[i][j] = 0;
			}
		}
	}
	
	back();
	for(int i=0;i<=closure_num;++i){
		cout<<i<<"   ";
		for(int j=0;j<Vtt.size();++j){
			cout<<act[i][j]<<"    ";
		}
		cout<<endl;
	}
}

int goto_aiming(int i, int j){
	//cout<<closure[i].size()<<endl;
	vector<rule2> mini_closure;
	for(int k=0;k<closure[i].size();++k){
		int point = closure[i][k].right.find(".");
		if(point != closure[i][k].right.length()-1){
			if(closure[i][k].right[point+1] == Vnn[j]){
				string str = closure[i][k].right;
				swap(str[point],str[point+1]);
				rule2 r;
				r.left = closure[i][k].left;
				r.right = str;
				r.future = closure[i][k].future;
				mini_closure.push_back(r);
			}
		}
	}
	for(int k=0;k<=closure_num;++k){
		if(k!=i){
			if(closure_equal(mini_closure,closure[k])){
				return k;
			}
		}
	}
	return -1;
}

void goto_table(){
	cout<<endl<<"Goto:"<<endl;
	cout<<"    ";
	for(int i=1;i<Vnn.size();++i){
		cout<<Vnn[i]<<"   ";
	}
	cout<<endl;
	
	memset(gt,0,sizeof(gt));
	for(int i=0;i<=closure_num;++i){
		for(int j=1;j<Vnn.size();j++){
			int num = goto_aiming(i,j);
			if(num != -1){
				gt[i][j] = num;
			}
			else{
				gt[i][j] = 0;
			}
		}
	}
	for(int i=0;i<=closure_num;++i){
		cout<<i<<"   ";
		for(int j=1;j<Vnn.size();++j){
			if(gt[i][j] != 0)
				cout<<gt[i][j]<<"   ";
			else 
				cout<<"   ";
		}
		cout<<endl;
	}
    cout<<"�ķ���LR(1)�ķ���"<<endl<<endl;
}

void process(){  //�õ���������
	cout<<endl;
	cout<<"[parsing]"<<endl;
	stack<int> state;
	stack<char> symbol;
	state.push(0);
	symbol.push('#');
	cout<<"ջ�� ���� ���  ����           ע"<<endl;
	cout<<"------------------------------------------------------------------------"<<endl;
	int turn = 0;
	while(1){
		cout<<state.top()<<" "<<symbol.top()<<"   "<<input_string[turn]<<"   ";
		if(state.top()==1&&input_string[turn]=='#'){cout<<"acc   �ɹ�����"<<endl;break;}
		for(int i=0;i<Vtt.size();++i){
			if(Vtt[i]==input_string[turn]){  //��ʱΪ�ƽ�����
				cout<<act[state.top()][i]<<"   ";
				string str = act[state.top()][i];
				if(str > "s"){
					symbol.push(input_string[turn]);
					state.push(at[state.top()][i]);
					cout<<"��ջ"<<"   ";
					cout<<state.top()<<" "<<symbol.top()<<endl;
					turn++;
					break;
				}
				else if(str<"s"){   //��ʱΪ��Լ����
					//cout<<"��ջ"<<"   ";
					int num = abs(at[state.top()][i]);
					int quality = ru[num].right.length()-1;
					printf("��ջ%d�����ź�״̬  ",quality);
					for(int i=0;i<quality;++i){
						state.pop();
						symbol.pop();
					}
					symbol.push(ru[num].left);
					for(int i=1;i<Vnn.size();++i){
						if(Vnn[i] == ru[num].left){
							state.push(gt[state.top()][i]);
						}
					}
					cout<<"��ջ"<<" "<<state.top()<<" "<<symbol.top()<<"   ";
					int l = ru[num].right.length();
					string str = "";
					for(int i=0;i<l-1;++i) str+=ru[num].right[i];
					cout<<ru[num].left<<"->"<<str;
					cout<<endl;
				}	
			}
		}	
	}
	cout<<"------------------------------------------------------------------------"<<endl;
	cout<<"end!"<<endl;
}

void output(UNFINAL nf,FINAL yf,EXP *exp,char start){

    cout<<"CFG=(VN,VT,P,S)"<<endl;
	//��ӡ���ս��
	cout<<"    VN: ";
    printf("%s\n", nf.unfinal);

    //��ӡ�ս��
    cout<<"    VT: ";
    printf("%s\n", yf.final);

    //��ӡ����ʽ����
    printf("Production:\n");
    int k = 0;
    int expnum = exp[k].num;
    for (k = 1; k <= expnum; k++){
        printf("\t%d��%s\n", k-1,exp[k].expression);
    }

    //��ӡ��ʼ����
    cout<<"StartSymbol: ";
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
        V.push_back(nf.unfinal[i]);
        Vnn.push_back(nf.unfinal[i]);
		i++;
        nf.unfinal[i] = ' ';
    }

    //�����ս��
    Vtt.push_back('#');
    scanf("%d", &yf.num);
    for (i = 0; i < yf.num * 2; i++){
        scanf("%s", &yf.final[i]);
        Vt.insert(yf.final[i]);
        V.push_back(yf.final[i]);
        Vtt.push_back(yf.final[i]);
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
            // if (ch[j] >= '0' && ch[j] <= '9'){
            //     exp[i].expression[temp] = '-';
            //     temp++;
            //     exp[i].expression[temp] = '>';
            //     temp++;
            //     j++;
            // }
            // else{
                exp[i].expression[temp] = ch[j];
                temp++;
                j++;
            //}
        }
        exp[i].expression[temp] = '\0';
    }

    GetRule(exp);

    //���뿪ʼ����
    scanf("%s", &start);

	cin>>input_string;
	
    //����ķ�
    output(nf,yf,exp,start);

    //OutPutFirstSet(nf,yf,exp);
}

int main(){
    //������@���ű�ʾ
    input();
    make_first();
    items();
	action_table();
	goto_table();
	process();
    system("pause");
    return 0;
}
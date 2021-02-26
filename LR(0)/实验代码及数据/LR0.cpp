#include <bits/stdc++.h>
using namespace std;

char final[10];
int NumofFinal = 0;

char unfinal[10][3];
int NumofUnfinal=0;

struct rl //relation
{
    int strt; //start
    int numoftrns;
    bool end[100]; //�Ƿ����ս��
    int trns[100];
} rlt[100];
int numofrlt; //����ʽ����

char stc[10];
char ss[100];

int posofzjf(char a)
{
    if (a == '#')
        return 3;
    for (int i = 0; i < NumofFinal; i++)
        if (a == final[i])
            return i;
    return -2;
}

int posoffzjf(char a[])
{
    for (int i = 0; i < NumofFinal; i++)
        if (!strcmp(a, unfinal[i]))
            return i;
    return -2;
}

void init()
{
    FILE *f = fopen("datain.txt", "r+");

    fscanf(f, "%d", &NumofUnfinal);
    for (int i = 0; i < NumofUnfinal; i++)
        fscanf(f, "%s", unfinal[i]);

    fscanf(f, "%d", &NumofFinal);
    for (int i = 0; i < NumofFinal; i++)
    {
        while (final[i] == '\0' || final[i] == '\n' || final[i] == ' ')
            fscanf(f, "%c", &final[i]);
    }

    fscanf(f, "%d", &numofrlt);
    for (int i = 0; i < numofrlt; i++)
    {
        char temp[10];
        fscanf(f, "%s", temp);
        rlt[i].strt = posoffzjf(temp); //���ս��
        fscanf(f, "%d", &rlt[i].numoftrns);

        for (int j = 0; j < rlt[i].numoftrns; j++)
        {
            fscanf(f, "%s", temp);
            if (temp[0] >= 'A' && temp[0] <= 'Z')
            {
                rlt[i].end[j] = false;
                rlt[i].trns[j] = posoffzjf(temp);
            }
            else
            {
                rlt[i].end[j] = true;
                rlt[i].trns[j] = posofzjf(temp[0]);
            }
        }
    }
    fscanf(f, "%s", stc);
    fscanf(f, "%s", ss);
    return;
}

const int node = 9999;
struct GRP
{
    int numofRLT;
    int head[10];
    int numofTRS[10];
    bool end[10][100]; //�Ƿ����ս��
    int follow[10][100];
    int nodepos[10];
    int rltpos[10];
} I[100];
int numofGRP = 0;

void fillI(int gNum, int nPos, int rltpos)
{
    int no = I[gNum].numofRLT;
    I[gNum].head[no] = rlt[rltpos].strt;
    I[gNum].rltpos[no] = rltpos;

    int posP = 0;
    for (int i = 0; i < rlt[rltpos].numoftrns; i++)
    {
        if (i == nPos)
        {
            I[gNum].follow[no][posP] = node;
            I[gNum].nodepos[no] = nPos;
            I[gNum].end[no][posP] = true;
            posP++;
        }
        I[gNum].follow[no][posP] = rlt[rltpos].trns[i];
        I[gNum].end[no][posP] = rlt[rltpos].end[i];
        posP++;
    }
    if (nPos == rlt[rltpos].numoftrns)
    {
        I[gNum].follow[no][posP] = node;
        I[gNum].nodepos[no] = nPos;
        I[gNum].end[no][posP] = true;
        posP++;
    }
    I[gNum].numofTRS[I[gNum].numofRLT] = rlt[rltpos].numoftrns + 1;
    I[gNum].numofRLT++;
}

void findClouse(int no, int nuinq) //nuinq�����ڵ�ǰ��������е��Ѿ����ڸ���
{
    queue<int> q;
    for (int i = 0; i < nuinq; i++)
    {
        q.push(i);
    }
    while (!q.empty())
    {
        int num = q.front();
        q.pop();
        //���node����������ս������ֱ�Ӽ����������������Ҫ�����ս���������ϢҲ���б��������ջ�����жϡ�
        // �����Ͻ���˵Ӧ�ö�����ѹ��Ľ����ظ��жϣ�������ݹ� �Ĵ���
        if (I[no].end[num][I[no].nodepos[num] + 1] == false && I[no].nodepos[num] != I[no].numofTRS[num] - 1)
        //node���Ƿ��ս�� ����node�������һ�������ǲ���nodeλ��������λ��
        {
            int nw = I[no].nodepos[num] + 1;
            //����һ�����ս���ı��
            for (int i = 0; i < numofrlt; i++)
            {
                if (rlt[i].strt == I[no].follow[num][nw])
                {
                    fillI(no, 0, i);
                    q.push(I[no].numofRLT - 1);
                }
            }
        }
    }
}

int GO[100][4] = {0};
int numofGO = 0;

int checkrept(int a)
{
    for (int i = 0; i < numofGRP; i++)
    {
        if (I[i].head[0] == I[a].head[0] &&
            I[i].numofTRS[0] == I[a].numofTRS[0] &&
            I[i].nodepos[0] == I[a].nodepos[0] &&
            I[i].rltpos[0] == I[a].rltpos[0])
        {
            return i;
        }
    }

    return -1;
}

void PrjGrp()
{
    queue<int> Aq;
    Aq.push(0);
    //create I0 first
    queue<int> q;
    fillI(0, 0, 0);
    // q.push(0);
    // while (!q.empty())
    // {
    //     int num = q.front();
    //     q.pop();
    //     //���node����������ս������ֱ�Ӽ����������������Ҫ�����ս���������ϢҲ���б��������ջ�����жϡ�
    //     // �����Ͻ���˵Ӧ�ö�����ѹ��Ľ����ظ��жϣ�������ݹ� �Ĵ���
    //     if (I[0].end[num][I[0].nodepos[num] + 1] == false && I[0].nodepos[num] != I[0].numofTRS[num])
    //     //node���Ƿ��ս�� ����node�������һ�������ǲ���nodeλ��������λ��
    //     {
    //         int nw = I[0].nodepos[num] + 1;
    //         //����һ���ս���ı��
    //         for (int i = 0; i < numofrlt; i++)
    //         {
    //             if (rlt[i].strt == I[0].follow[0][nw])
    //             {
    //                 fillI(0, 0, rlt[i].strt);
    //                 q.push(I[0].numofRLT - 1);
    //             }
    //         }
    //     }
    // }

    findClouse(0, 1);
    numofGRP++;

    while (!Aq.empty())
    {
        int num = Aq.front();
        Aq.pop();
        // Ĭ�������û����һ��I�е�node��������ͬ�������������Ҫ��������ͬ��������кϲ�����
        // ֮����ǽ�����node������ĩβ����������µ�push
        for (int i = 0; i < I[num].numofRLT; i++)
        {
            if (I[num].nodepos[i] != I[num].numofTRS[i] - 1)
            {
                GO[numofGO][0] = num;
                fillI(numofGRP, I[num].nodepos[i] + 1, I[num].rltpos[i]);
                // ����һ��֮����Ҫ���ڲ��ҵ���ǰ���ǰ����rlt�����жϣ��ж�ǰ���ǰ����rlt�Ƿ������ǰ��
                // �����ֻ�й������û�г��ֹ��򷵻�-1��
                // ����Ѿ����ֹ�����ֱ�ӷ����Ǹ��ı�ţ��ŵ�GO�����ȥ
                if (checkrept(numofGRP) != -1)
                {
                    int nxtpos = I[numofGRP].nodepos[0];
                    GO[numofGO][1] = checkrept(numofGRP);
                    GO[numofGO][2] = I[numofGRP].follow[0][nxtpos - 1];
                    GO[numofGO][3] = I[numofGRP].end[0][nxtpos - 1];
                    numofGO++;
                    I[numofGRP].numofRLT = 0;
                    continue;
                }
                int nxtpos = I[numofGRP].nodepos[0];
                GO[numofGO][1] = numofGRP; //�����´������Ǹ�
                GO[numofGO][2] = I[numofGRP].follow[0][nxtpos - 1];
                GO[numofGO][3] = I[numofGRP].end[0][nxtpos - 1];

                findClouse(numofGRP, 1);
                Aq.push(numofGRP);
                numofGRP++;
                numofGO++;
            }
        }
    }
    return;
}
//�ɹ��ܹ������е������䵽���е�GO����Լ�I����У�ʣ�µ������ֻ�������

void output(int num, bool end)
{
    if (num == node)
    {
        cout << ".";
        return;
    }
    if (end)
    {
        cout << final[num];
        return;
    }
    else
    {
        cout << unfinal[num];
        return;
    }
}

//���Ƚ����е�I�������
void outputI()
{
    cout<<endl<<"LR(0)���ս������:"<<endl;
    for (int i = 0; i < numofGRP; i++)
    {
        cout << "I" << i << ":" << endl;
        for (int j = 0; j < I[i].numofRLT; j++)
        {
            cout << "\t" << unfinal[I[i].head[j]] << "->";
            for (int k = 0; k < I[i].numofTRS[j]; k++)
            {
                output(I[i].follow[j][k], I[i].end[j][k]);
            }
            cout << endl;
        }
    }
}

void outputGO()
{   
    cout<<endl<<"DFA"<<endl;
    for (int i = 0; i < numofGO; i++)
    {
        cout << "  (" << GO[i][0] << ",";
        output(GO[i][2], GO[i][3]);
        cout << ")->";
        cout << GO[i][1] << endl;
    }
}

int Actionr[10][10] = {0};
int Arlie;
int Arhang;

int Actions[10][10] = {0};
int Aslie;
int Ashang;

int Goto[10][10] = {0};
int Glie;
int Ghang;

void creatActionGoto()
{
    Arlie = NumofFinal + 1;
    Arhang = numofGRP;
    Aslie = NumofFinal + 1;
    Ashang = numofGRP;
    Glie = NumofUnfinal;
    Ghang = numofGO;

    for (int i = 0; i < numofGO; i++)
    {
        if (GO[i][3] == true) //������������ս�� Ӧ������s
        {
            Actions[GO[i][0]][GO[i][2]] = GO[i][1];
        }
        else
        {
            Goto[GO[i][0]][GO[i][2]] = GO[i][1];
        }
    }
    for (int i = 0; i < numofGO; i++)
    {
        if (I[i].nodepos[0] == I[i].numofTRS[0] - 1)
        {
            if (I[i].rltpos[0] == 0)
                Actionr[i][NumofFinal] = -1;
            // �������ĩβֱ��
            else
                for (int j = 0; j < Arlie; j++)
                {
                    Actionr[i][j] = I[i].rltpos[0];
                }
        }
    }
    // for (int i = 0; i < Arhang; i++)
    // {
    //     for (int j = 0; j < Arlie; j++)
    //     {
    //         cout << Actionr[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    // for (int i = 0; i < Ashang; i++)
    // {
    //     for (int j = 0; j < Aslie; j++)
    //     {
    //         cout << Actions[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;   
    // for (int i = 0; i < Ghang; i++)
    // {
    //     for (int j = 0; j < Glie; j++)
    //     {
    //         cout << Goto[i][j] << " ";
    //     }
    //     cout << endl;
    // }
}

void outputActionGoto()
{
    cout<<endl<<"LR(0)������"<<endl;
    cout << "   ";
    for (int i = 0; i < NumofFinal; i++)
    {
        cout << final[i] << "   ";
    }
    cout << "#" << endl;
    for (int i = 0; i < Arhang; i++)
    {
        cout << i << "  ";
        for (int j = 0; j < Arlie; j++)
        {
            if (Actionr[i][j] != 0)
            {
                if (Actionr[i][j] == -1)
                    cout << "acc";
                else
                    cout << "r" << Actionr[i][j] << "  ";
            }
            else if (Actions[i][j] != 0)
            {
                cout << "s" << Actions[i][j] << "  ";
            }
            else
            {
                cout << "    ";
            }
        }
        cout << endl;
    }

    cout << " ";
    for (int i = 0; i < NumofUnfinal; i++)
    {
        cout << " ";
        cout << unfinal[i];
    }
    cout << endl;
    for (int i = 0; i < Ghang; i++)
    {
        cout << i << ": ";
        for (int j = 0; j < Glie; j++)
        {
            if (Goto[i][j] == 0)
                cout << "  ";
            else
                cout << Goto[i][j] << " ";
        }
        cout << endl;
    }
}

void coutrlt(int i)
{
    cout << unfinal[rlt[i].strt] << "->";
    for (int j = 0; j < rlt[i].numoftrns; j++)
    {
        if (rlt[i].end[j] == true)
        {
            if (rlt[i].trns[j] != -1)
                cout << final[rlt[i].trns[j]];
            else
            {
                cout << "��";
            }
        }
        else
            cout << unfinal[rlt[i].trns[j]];
    };
}

string pross()
{
    cout<<endl<<"�ƽ���Լ���̣�"<<endl;
    stack<int> data;
    data.push(0);
    cout << ss << endl;
    int p = 0;
    for (int i = 0; i < strlen(ss); i++)
    {
        int flag = posofzjf(ss[i]);
        int num1 = Actions[data.top()][flag];
        int num2 = Actionr[data.top()][flag];
        if (num1 != 0)
        {
            cout<<"�ƽ���"<<num1<<","<<ss[i]<<endl;
            data.push(num1);
        }
        else if (num2 != 0)
        {
            if (flag == 3 && num2 == -1)
            {
                data.pop();
                break;
            }
            cout<<"��Լ��";
            coutrlt(num2);
            for (int c = 0; c < rlt[num2].numoftrns; c++)
            {
                data.pop();
            }
            int temp = data.top();
            cout<<"��ջ��"<<Goto[temp][rlt[num2].strt]<<","<<unfinal[rlt[num2].strt]<<endl;
            data.push(Goto[temp][rlt[num2].strt]);
            i--;
        }
        else
        {
            return "ERROR";
        }
    }

    if (data.top() == 0)
    {
        return "�ɹ����գ�";
    }
    return "ERROR";
}

int main()
{
    init();
    PrjGrp();
    outputI();
    outputGO();
    creatActionGoto();
    outputActionGoto();
    cout << pross() << endl;
    system("pause");
    return 0;
}
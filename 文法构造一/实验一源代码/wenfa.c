#include <stdio.h>
//���������ṹ��ֱ��ʾ�ս�������ս��������ʽ 
//�ս��
typedef struct FINAL{
    char final[10];
    int num;
} FINAL;

//���ս��
typedef struct UNFINAL{
    char unfinal[10];
    int num;
} UNFINAL;

//����ʽ
typedef struct EXP{
    int num;
    char expression[20];
} EXP;

char start; //��ʼ����

 
void output(UNFINAL nf,FINAL yf,EXP *exp,char start){
	
	//��ӡ���ս��
	printf("���ս����\n");
    printf("%s\n", nf.unfinal);
    
    //��ӡ�ս��
    printf("�ս����\n");
    printf("%s\n", yf.final);
    
    //��ӡ����ʽ����
    printf("����ʽ��\n");
    int k = 0;
    int expnum = exp[k].num;
    for (k = 1; k <= expnum; k++){
        printf("%s\n", exp[k].expression);
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
        i++;
        nf.unfinal[i] = ' ';
    }

    //�����ս��
    scanf("%d", &yf.num);
    for (i = 0; i < yf.num * 2; i++){
        scanf("%s", &yf.final[i]);
        i++;
        yf.final[i] = ' ';
    }

    //�������ʽ����
    scanf("%d", &exp[0].num);
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

    //���뿪ʼ����
    scanf("%s", &start);

    //����ķ�
    output(nf,yf,exp,start);
}

int main(){
    input();
    return 0;
}


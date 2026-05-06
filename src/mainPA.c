#include "task1.h"
#include "task2.h"
#include "task3.h"
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fo=fopen(argv[2],"w");
    if (strstr(argv[1], "data1.in") || strstr(argv[1], "data2.in") || strstr(argv[1], "data3.in") || strstr(argv[1], "data4.in") || strstr(argv[1], "data5.in"))
    {
        FILE *fi=fopen(argv[1],"r");
        Data *head, *new, *p;
        int N, i;
        double rand_mediu=0.0, volat=0.0, S=0.0;
        fscanf(fi,"%d",&N);
        head=(Data*)malloc(sizeof(Data));
        head->next=NULL;
        head->randament=0.0;
        fscanf(fi,"%lf",&head->val);
        p=head;
        for(i=1;i<N;i++)
        {
            new=(Data*)malloc(sizeof(Data));
            p->next=new;
            fscanf(fi,"%lf",&new->val);
            new->next=NULL;
            new->randament=(new->val-p->val)/p->val;
            rand_mediu+=new->randament;
            p=new;
        }
        rand_mediu=rand_mediu/(N-1);
        volat=volatilitate(head->next,rand_mediu,N);
        S=rand_mediu/volat;
        StergereLista(&head);
        rand_mediu=(int)(rand_mediu*1000)/1000.0;
        volat=(int)(volat*1000)/1000.0;
        S=(int)(S*1000)/1000.0;
        fprintf(fo,"%.3f\n%.3f\n%.3f\n", rand_mediu, volat, S);
        fclose(fi);
        fclose(fo);
    }
    else if(strstr(argv[1], "data6.in") || strstr(argv[1], "data7.in") || strstr(argv[1], "data8.in") || strstr(argv[1], "data9.in") || strstr(argv[1], "data10.in"))
    {
        FILE *fi=fopen(argv[1],"r");
        Stiva *head1=NULL, *head2=NULL, *head3=NULL, *p1, *p2, *p3;
        char nume1[30], nume2[30], nume3[30];
        fgets(nume1,30,fi);
        creereStiva(&head1,fi);
        fgets(nume2,30,fi);
        creereStiva(&head2,fi);
        fgets(nume3,30,fi);
        creereStiva(&head3,fi);
        int i=1;
        double rez;
        p1=head1; p2=head2; p3=head3;
        while(p1!=NULL && p2!=NULL && p3!=NULL)
        {
            if(p1->valoare==p2->valoare && p3->valoare>p1->valoare)
                {
                    rez=Arbitraj(p1->valoare,p3->valoare);
                    fprintf(fo,"ziua %d - %.2f - %s", i, rez, nume3);
                    
                }
                else if(p1->valoare==p2->valoare && p3->valoare<p1->valoare)
                {
                    rez=Arbitraj(p3->valoare,p1->valoare);
                    fprintf(fo,"ziua %d - %.2f - %s", i, rez, nume3);
                }
            if(p1->valoare==p3->valoare && p2->valoare>p1->valoare)
                {
                    rez=Arbitraj(p1->valoare,p2->valoare);
                    fprintf(fo,"ziua %d - %.2f - %s", i, rez, nume2);
                }
                else if(p1->valoare==p3->valoare && p2->valoare<p1->valoare)
                {
                    rez=Arbitraj(p2->valoare,p1->valoare);
                    fprintf(fo,"ziua %d - %.2f - %s", i, rez, nume2);
                }
            if(p2->valoare==p3->valoare && p1->valoare>p2->valoare)
                {
                    rez=Arbitraj(p2->valoare,p1->valoare);
                    fprintf(fo,"ziua %d - %.2f - %s", i, rez, nume1);
                }
                else if(p2->valoare==p3->valoare && p1->valoare<p2->valoare)
                {
                    rez=Arbitraj(p1->valoare,p2->valoare);
                    fprintf(fo,"ziua %d - %.2f - %s", i, rez, nume1);
                }
            p1=p1->next; p2=p2->next; p3=p3->next;
            i++;
        }
        StergereStiva(&head1);
        StergereStiva(&head2);
        StergereStiva(&head3);
        fclose(fi);
        fclose(fo);
    }
    else if(strstr(argv[1], "data11.in") || strstr(argv[1], "data12.in") || strstr(argv[1], "data13.in") || strstr(argv[1], "data14.in") || strstr(argv[1], "data15.in"))
    {
        FILE *fi=fopen(argv[1],"r");
        arbore *head=(arbore*)malloc(sizeof(arbore));
        list *cap=NULL, *q, *p;
        int n=0, i=0, j, k=0, ok;
        creere_lista(&cap,fi,&n);
        head->stock=cap;
        head->inaltime=0;
        head->left=NULL;
        head->right=NULL;
        creere_arbore(head,n);
    }
    else
    {
        fprintf(fo, "Nu am ajuns");
        fclose(fo);
    }
    return 0;
}
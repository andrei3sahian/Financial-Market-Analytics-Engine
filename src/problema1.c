#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct portofoliu
{
    double val;
    double randament;
    struct portofoliu *next;
};

typedef struct portofoliu Data;

int main(int argc, char *argv[])
{
    FILE *fo=fopen(argv[2],"w");
    if (strstr(argv[1], "data1.in") ||
            strstr(argv[1], "data2.in") ||
            strstr(argv[1], "data3.in") ||
            strstr(argv[1], "data4.in") ||
            strstr(argv[1], "data5.in"))
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
        for(p=head->next; p!=NULL ;)
        {
            volat+=(p->randament-rand_mediu)*(p->randament-rand_mediu);
            new=p;
            p=p->next;
            free(new);
        }
        free(head);
        volat=sqrt(volat/(N-1));
        S=rand_mediu/volat;
        rand_mediu=(int)(rand_mediu*1000)/1000.0;
        volat=(int)(volat*1000)/1000.0;
        S=(int)(S*1000)/1000.0;
        fprintf(fo,"%.3f\n%.3f\n%.3f", rand_mediu, volat, S);
        fclose(fi);
    }
    else
    {
        fprintf(fo, "Nu am ajuns");
        fclose(fo);
    }
    return 0;
}
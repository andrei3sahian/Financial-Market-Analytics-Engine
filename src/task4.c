#include "task4.h"

int creere_mat_adiacenta(int d, FILE *fi, int N, graph *g)
{
    double v[20], min, max;
    int i, j, nr_intervale, indice1, indice2, con;
    for(i=0;i<N;i++)
    {
        fscanf(fi,"%lf ",&v[i]);
    }
    min=v[0]; max=v[0];
    for(i=1;i<N;i++)
    {
        if(min>v[i])
            min=v[i];
        if(max<v[i])
            max=v[i];
    }
    nr_intervale=(int)(((int)max-(int)min)/d)+1;
    g->a=(int**)malloc(sizeof(int*)*nr_intervale);
    for(i=0;i<nr_intervale;i++)
    {
        g->a[i]=(int*)malloc(sizeof(int)*nr_intervale);
        g->nod[i]=(Nodes*)malloc(sizeof(Nodes));
        g->nod[i]->inceput=(int)min+i*d;
        g->nod[i]->final=(int)min+(i+1)*d;
        g->nod[i]->prob.numarator=0;
        g->nod[i]->prob.numitor=0;
    }
    for(i=0;i<nr_intervale;i++)
    {
        for(j=0;j<nr_intervale;j++)
        {
            g->a[i][j]=0;
        }
    }
    for(i=0;i<N-1;i++)
    {
        indice1=(int)((v[i]-(int)min)/d);
        indice2=(int)((v[i+1]-(int)min)/d);
        g->a[indice1][indice2]++;      //de la i la j
    }
    for(i=0;i<nr_intervale;i++)
    {
        con=0;
        for(j=0;j<nr_intervale;j++)
        {
            con+=g->a[i][j];
        }
        g->nod[i]->nr_legaturi=con;
    }
    return nr_intervale;
}

void push(Queue **head, int n, int numarator, int numitor)
{
    Queue *new=(Queue*)malloc(sizeof(Queue));
    new->q=n;
    new->next=NULL;
    new->numarator=numarator;
    new->numitor=numitor;
    if((*head)==NULL)
    {
        (*head)=new;
    }
    else
    {
        Queue *temp=(*head);
        while(temp->next!=NULL)
            temp=temp->next;
        temp->next=new;
    }
}

void pop(Queue **head)
{
    Queue *temp=(*head);
    if((*head)!=NULL)
    {
        (*head)=(*head)->next;
        free(temp);
    }
}

void afisare(Nodes *n, FILE *fo)
{
    if(n->prob.numarator==n->prob.numitor)
        fprintf(fo,"%d\n", n->prob.numarator);
    else
        fprintf(fo,"%d/%d\n", n->prob.numarator, n->prob.numitor);
}

int cmmdc(int a, int b)
{
    int r;
    while(b!=0)
    {
        r=a%b;
        a=b;
        b=r;
    }
    return a;
}

void calcul_fractii(Queue *head, graph *g, probabil v[], int (*n), int nr_intervale)
{
    int numarator_initial, numitor_initial, i, numarator_final, numitor_final;
    Queue *fiu=head;
    while(fiu!=NULL)
    {
        for(i=0;i<nr_intervale;i++)
        {
            if(g->a[i][fiu->q]!=0)
            {
                numarator_initial=g->a[i][fiu->q]*g->nod[i]->prob.numarator;
                numitor_initial=g->nod[i]->nr_legaturi*g->nod[i]->prob.numitor;
                if(numarator_final==0 && numitor_final==0)
                {
                    numarator_final=numarator_initial;
                    numitor_final=numitor_initial;
                }
                else
                {
                    if(numitor_final!=numitor_initial)
                    {
                        int acelasi;
                        acelasi=cmmdc(numitor_final,numitor_initial);
                        numitor_final=acelasi;
                        numarator_final=numarator_final*(acelasi/numitor_final)+numarator_initial*(acelasi/numitor_initial);
                    }
                }
            }
        }
        v[(*n)].numarator=numarator_final;
        v[(*n)].numitor=numitor_final;
        (*n)++;
        fiu=fiu->next;
    }
}

void inactivare(Queue *head, graph *g, int nr_intervale)
{
    int v[20], n=0, i, j, ok;
    while(head!=NULL)
    {
        v[n]=head->q;
        n++;
        head=head->next;
    }
    for(i=0;i<nr_intervale;i++)
    {
        ok=0;
        for(j=0;j<n;j++)
        {
            if(i==v[j])
                ok=1;
        }
        if(ok==0)
        {
            g->nod[i]->prob.numarator=0;
            g->nod[i]->prob.numitor=0;
        }
    }
}

void probabilitati(graph *g, int K, int inceput, int final, FILE *fo, int nr_intervale)
{
    Queue *head=(Queue*)malloc(sizeof(Queue));
    head=NULL;
    int contor=1, i, j, contor_activ;
    for(i=1;i<=K;i++)
    {
        if(i==1)
        {
            push(&head,inceput,g->nod[inceput]->prob.numarator,g->nod[inceput]->prob.numitor);
            g->nod[inceput]->prob.numarator=1;
            g->nod[inceput]->prob.numitor=1;
            afisare(g->nod[final],fo);
        }
        else
        {
            while(contor)
            {
                for(j=0;j<nr_intervale;j++)
                {
                    if(g->a[head->q][j]!=0)
                    {
                        push(&head,j,g->nod[head->q]->prob.numarator,g->nod[head->q]->prob.numitor);
                        contor_activ++;
                    }
                }
                probabil v[contor_activ];
                int n=0;
                contor--;
                calcul_fractii(head,g,v,&n,nr_intervale);
                Queue *temp=head;
                for(i=0;i<n;i++)
                {
                    g->nod[temp->q]->prob.numitor=v[i].numitor;
                    g->nod[temp->q]->prob.numarator=v[i].numarator;
                    temp=temp->next;
                }
            }
            afisare(g->nod[final],fo);
        }
        contor=contor_activ;
        inactivare(head,g,nr_intervale);                       //funcite pentru a initializa toate nodurile inactive cu prob=0
        while(contor_activ!=0)
        {
            pop(&head);
            contor_activ--;
        }
    }
}
#include "task4.h"

int creere_mat_adiacenta(int d, FILE *fi, int N, graph *g)
{
    double v[20], min, max;
    int i, j, nr_intervale;
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
        g->nod[i]->prob.numitor=1;
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
        int indice1=(int)((v[i]-(int)min)/d);
        int indice2=(int)((v[i+1]-(int)min)/d);
        g->a[indice1][indice2]++;      //de la i la j
    }
    for(i=0;i<nr_intervale;i++)
    {
        int con=0;
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

void afisare(const Nodes *n, FILE *fo, int zi, int K)
{
    if(n->prob.numarator==n->prob.numitor || n->prob.numitor==1)
        fprintf(fo,"%d", n->prob.numarator);
    else
        fprintf(fo,"%d/%d", n->prob.numarator, n->prob.numitor);
    if(zi<K)
        fprintf(fo,"\n");
}

int cmmdc(int a, int b)
{
    if(a==0 || b==0)
        return 1;
    while(b!=0)
    {
        int r=a%b;
        a=b;
        b=r;
    }
    return a;
}

int cmmmc(int a, int b)
{
    if(a==0 || b==0)
        return 1;
    return (a/cmmdc(a,b))*b;
}

void calcul_fractii(Queue *head, graph *g, probabil v[], int (*n), int nr_intervale)
{
    Queue *fiu=head;
    while(fiu!=NULL)
    {
        int numarator_final=0;
        int numitor_final=0;
        for(int i=0;i<nr_intervale;i++)
        {
            if(g->a[i][fiu->q]!=0 && g->nod[i]->nr_legaturi > 0 && g->nod[i]->prob.numarator > 0)
            {
                int numarator_initial=g->a[i][fiu->q]*g->nod[i]->prob.numarator;
                int numitor_initial=g->nod[i]->nr_legaturi*g->nod[i]->prob.numitor;
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
                        acelasi=cmmmc(numitor_final,numitor_initial);
                        numarator_final=numarator_final*(acelasi/numitor_final)+numarator_initial*(acelasi/numitor_initial);
                        numitor_final=acelasi;
                    }
                    else
                    {
                        numarator_final+=numarator_initial;
                    }
                }
            }
        }
        int d = cmmdc(numitor_final, numarator_final);
        if(d != 1)
        {
            numitor_final  /= d;
            numarator_final /= d;
        }
        v[(*n)].numarator=numarator_final;
        v[(*n)].numitor=numitor_final;
        (*n)++;
        fiu=fiu->next;
    }
}

void inactivare(Queue *head, graph *g, int nr_intervale)
{
    int v[20], n=0;
    while(head!=NULL)
    {
        v[n]=head->q;
        n++;
        head=head->next;
    }
    for(int i=0;i<nr_intervale;i++)
    {
        int ok=0;
        for(int j=0;j<n;j++)
        {
            if(i==v[j])
                ok=1;
        }
        if(ok==0)
        {
            g->nod[i]->prob.numarator=0;
            g->nod[i]->prob.numitor=1;
        }
    }
}

void probabilitati(graph *g, int K, int inceput, int final, FILE *fo, int nr_intervale) 
{
    int zi=1, j, i;
    g->nod[inceput]->prob.numarator=1;
    g->nod[inceput]->prob.numitor=1;
    afisare(g->nod[final],fo,zi,K);
    for(zi=2;zi<=K;zi++) 
    {
        Queue *head_activ=NULL;
        for(j=0; j<nr_intervale;j++) 
        {
            int are_predecesor=0;
            for(i=0;i<nr_intervale;i++) 
            {
                if(g->nod[i]->prob.numarator>0 && g->a[i][j]>0) 
                {
                    are_predecesor=1;
                    break;
                }
            }
            if(are_predecesor!=0) 
                push(&head_activ, j, 0, 1);
        }
        int n_v=0;
        probabil v[40];
        calcul_fractii(head_activ,g,v,&n_v,nr_intervale);
        inactivare(head_activ,g,nr_intervale);
        Queue *temp=head_activ;
        for(i=0;i<n_v;i++) 
        {
            g->nod[temp->q]->prob.numarator=v[i].numarator;
            g->nod[temp->q]->prob.numitor=v[i].numitor;
            temp=temp->next;
        }
        afisare(g->nod[final],fo,zi,K);
        while (head_activ!= NULL)
            pop(&head_activ);
    }
}

void elibereaza_graf(graph *g, int nr_intervale) 
{
    int i;
    for(i=0;i<nr_intervale;i++) 
    {
        free(g->a[i]);
        free(g->nod[i]);
    }
    free(g->a);
    free(g);
}
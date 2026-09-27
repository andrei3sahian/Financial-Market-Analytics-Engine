#include "task4.h"

int creere_mat_adiacenta(int d, FILE *fi, int N, graph *g)
{
    double v[20], min, max;
    int i, j, nr_intervale;
    for(i=0;i<N;i++)
    {
        fscanf(fi,"%lf ",&v[i]);            //se retin valorile in vector
    }
    min=v[0]; max=v[0];
    for(i=1;i<N;i++)
    {
        if(min>v[i])
            min=v[i];               //se gasesc in vector minimul si maximul pentru a face intervalele
        if(max<v[i])
            max=v[i];
    }
    nr_intervale=(int)(((int)max-(int)min)/d)+1;            //se retin nr de intervale pt a gasi dimensiunea matricii
    g->a=(int**)malloc(sizeof(int*)*nr_intervale);
    for(i=0;i<nr_intervale;i++)
    {
        g->a[i]=(int*)malloc(sizeof(int)*nr_intervale);
        g->nod[i]=(Nodes*)malloc(sizeof(Nodes));
        g->nod[i]->inceput=(int)min+i*d;                //se creeaza nodurile cu intervale si probabilitatile de 0
        g->nod[i]->final=(int)min+(i+1)*d;
        g->nod[i]->prob.numarator=0;
        g->nod[i]->prob.numitor=1;
    }
    for(i=0;i<nr_intervale;i++)
    {
        for(j=0;j<nr_intervale;j++)                 //se initializeaza matricea cu 0
        {
            g->a[i][j]=0;
        }
    }
    for(i=0;i<N-1;i++)
    {
        int indice1=(int)((v[i]-(int)min)/d);
        int indice2=(int)((v[i+1]-(int)min)/d);             //se gasesc intervalele intre care trebuie puse legaturi
        g->a[indice1][indice2]++;                           //legatura este de la i la j
    }
    for(i=0;i<nr_intervale;i++)
    {
        int con=0;
        for(j=0;j<nr_intervale;j++)                         //se retine cate legaturi face fiecare nod pentru
        {                                                   //a-l pune la numitorul ponderilor din legaturi
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
    if(zi<K)                                                            //sa nu se mai puna ultimul \n
        fprintf(fo,"\n");
}

int cmmdc(int a, int b)
{
    if(a==0 || b==0)
        return 1;
    while(b!=0)
    {                                                                   //cmmdc pentru simplificari
        int r=a%b;
        a=b;
        b=r;
    }
    return a;
}

int cmmmc(int a, int b)
{
    if(a==0 || b==0)                                                   //pentru aducerea la acelasi numitor
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
            if(g->a[i][fiu->q]!=0 && g->nod[i]->nr_legaturi > 0 && g->nod[i]->prob.numarator > 0)       //se verifica daca nodul este activ
            {
                int numarator_initial=g->a[i][fiu->q]*g->nod[i]->prob.numarator;                //se inmultesc valorile care se afla deja in nod cu ce vine din
                int numitor_initial=g->nod[i]->nr_legaturi*g->nod[i]->prob.numitor;             //probabilitatile legaturii
                if(numarator_final==0 && numitor_final==0)                  //se adauga la numitor si numarator valorile efective nesimplificate
                {
                    numarator_final=numarator_initial;
                    numitor_final=numitor_initial;          //se retin valorile venite din ponderile legaturilor ca finale daca nu au probabilitati
                }
                else
                {
                    if(numitor_final!=numitor_initial)
                    {
                        int acelasi;
                        acelasi=cmmmc(numitor_final,numitor_initial);                           //se aduce la acelasi numitor
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
            numitor_final  /= d;                    //se gaseste o eventuala simplificare a fractiilor
            numarator_final /= d;
        }
        v[(*n)].numarator=numarator_final;
        v[(*n)].numitor=numitor_final;          //se adauga in vector numitorul si numaratorul final
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
        n++;                        //se retin indicii salvati din coada
        head=head->next;
    }
    for(int i=0;i<nr_intervale;i++)
    {
        int ok=0;
        for(int j=0;j<n;j++)
        {
            if(i==v[j])
                ok=1;               //se verifica daca apare nodul in coada si daca apare ramane cu probabilitatea pe care o are
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
    g->nod[inceput]->prob.numarator=1;              //nodul de inceput incepe cu probabilitatea 1
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
                if(g->nod[i]->prob.numarator>0 && g->a[i][j]>0)             //se verifica daca exista legatura intre un nod care este activ
                {
                    are_predecesor=1;
                    break;
                }
            }
            if(are_predecesor!=0) 
                push(&head_activ, j, 0, 1);             //daca nodul are un predecesor activ se adauga in coada
        }
        int n_v=0;
        probabil v[40];
        calcul_fractii(head_activ,g,v,&n_v,nr_intervale);           //se calculeaza pentru fiecare nod din coada probabilitatile si se retin in v
        inactivare(head_activ,g,nr_intervale);                  //se sterg probabilitatile din nodurile care nu se mai afla in coada
        Queue *temp=head_activ;
        for(i=0;i<n_v;i++) 
        {
            g->nod[temp->q]->prob.numarator=v[i].numarator;         //se pun pe fiecare nod noile probabilitati pentru a continua calculele
            g->nod[temp->q]->prob.numitor=v[i].numitor;             //in ziua urmatoare
            temp=temp->next;
        }
        afisare(g->nod[final],fo,zi,K);         //se afiseaza probabilitatea ndin nodul final
        while (head_activ!= NULL)
            pop(&head_activ);                   //se sterg elementele din coada
    }
}

void elibereaza_graf(graph *g, int nr_intervale) 
{
    int i;
    for(i=0;i<nr_intervale;i++)             //se elibereaza liniile din matrice si fiecare nod corespunzator liniei
    {
        free(g->a[i]);
        free(g->nod[i]);
    }
    free(g->a);             //se elibereaza matricea
    free(g);                //se elibereaza graful
}
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
    if(n->prob.numarator==n->prob.numitor || n->prob.numitor==1)
        fprintf(fo,"%d\n", n->prob.numarator);
    else
        fprintf(fo,"%d/%d\n", n->prob.numarator, n->prob.numitor);
}

int cmmdc(int a, int b)
{
    int r;
    if(a==0 || b==0)
        return 1;
    while(b!=0)
    {
        r=a%b;
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
    int numarator_initial=0, numitor_initial=0, i, numarator_final=0, numitor_final=0;
    Queue *fiu=head;
    while(fiu!=NULL)
    {
        numarator_final=0;
        numitor_final=0;
        for(i=0;i<nr_intervale;i++)
        {
            if(g->a[i][fiu->q]!=0 && g->nod[i]->nr_legaturi > 0 && g->nod[i]->prob.numarator > 0)
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
                        acelasi=cmmmc(numitor_final,numitor_initial);
                        numarator_final=numarator_final*(acelasi/numitor_final)+numarator_initial*(acelasi/numitor_initial);
                        numitor_final=acelasi;
                    }
                }
            }
        }
        if(cmmdc(numitor_final,numarator_final)!=1)
        {
            numitor_final=numitor_final/cmmdc(numitor_final,numarator_final);
            numarator_final=numarator_final/cmmdc(numitor_final,numarator_final);
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
            g->nod[i]->prob.numitor=1;
        }
    }
}

void probabilitati(graph *g, int K, int inceput, int final, FILE *fo, int nr_intervale) {
    int zi, j, i;
    g->nod[inceput]->prob.numarator = 1;
    g->nod[inceput]->prob.numitor = 1;

    // Afișăm probabilitatea pentru ziua 1
    afisare(g->nod[final], fo);

    // Calculăm pentru următoarele K-1 zile
    for (zi = 2; zi <= K; zi++) {
        // 1. Creăm o listă (Queue) cu nodurile care ar putea fi active AZI.
        // Un nod j este activ dacă există un nod i (care a avut probabilitate ieri)
        // care are legătură către j.
        Queue *head_activ = NULL;
        for (j = 0; j < nr_intervale; j++) {
            int are_predecesor = 0;
            for (i = 0; i < nr_intervale; i++) {
                if (g->nod[i]->prob.numarator > 0 && g->a[i][j] > 0) {
                    are_predecesor = 1;
                    break;
                }
            }
            if (are_predecesor) {
                // Adăugăm nodul j în coadă (fără duplicate, pentru că verificăm j-ul o singură dată)
                push(&head_activ, j, 0, 1); 
            }
        }

        // 2. Calculăm noile fracții pentru aceste noduri folosind funcția ta
        // Folosim un vector temporar 'v' ca să nu suprascriem datele de "ieri" în timp ce calculăm
        int n_v = 0;
        int nr_noduri_coada = 0;
        Queue *tmp = head_activ;
        while(tmp) { nr_noduri_coada++; tmp = tmp->next; }
        
        probabil v[nr_noduri_coada];
        calcul_fractii(head_activ, g, v, &n_v, nr_intervale);

        // 3. Aplicăm inactivarea pentru nodurile care NU sunt în head_activ
        inactivare(head_activ, g, nr_intervale);

        // 4. Actualizăm nodurile care SUNT în head_activ cu noile valori calculate
        tmp = head_activ;
        for (i = 0; i < n_v; i++) {
            g->nod[tmp->q]->prob.numarator = v[i].numarator;
            g->nod[tmp->q]->prob.numitor = v[i].numitor;
            tmp = tmp->next;
        }

        // 5. Afișăm probabilitatea nodului target pentru ziua curentă
        afisare(g->nod[final], fo);

        // 6. Curățăm coada pentru a o lua de la capăt în ziua următoare
        while (head_activ != NULL) {
            pop(&head_activ);
        }
    }
}

/*
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
            contor_activ=1;
            afisare(g->nod[final],fo);
        }
        else
        {
            while(contor!=0)
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
                calcul_fractii(head,g,v,&n,nr_intervale);
                Queue *temp=head;
                for(i=0;i<n;i++)
                {
                    g->nod[temp->q]->prob.numitor=v[i].numitor;
                    g->nod[temp->q]->prob.numarator=v[i].numarator;
                    temp=temp->next;
                }
                while(contor_activ!=0)
                {
                    pop(&head);
                    contor_activ--;
                }
                contor--;
            }
            afisare(g->nod[final],fo);
        }
        contor=contor_activ;
        inactivare(head,g,nr_intervale);                       //funcite pentru a initializa toate nodurile inactive cu prob=0
    }
}*/
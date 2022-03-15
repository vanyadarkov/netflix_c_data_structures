#include "lista.h"

//Initializarea unei noi celule
//Primeste info deja alocata
TLista InitCelula(void * info)
{   
    TLista a = (TLista)calloc(1,sizeof(TCelula));
    if(!a) return NULL;
    a->info = info;
    a->urm = NULL;
    return a;
}

//Inserare in lista de categorii(nu top10)
//returneaza pozitia unde a fost inserata sau -1 in caz de erori
int inserare_lista(TLista * l, void * element, Fcmp f)
{
    if(*l == NULL)//daca e inserare in lista vida
    {
        *l = InitCelula(element);
        if(*l == NULL)
        {
            fprintf(stderr, "Eroare initializare celula la inserare\n");;
            return -1;
        }
        return 1;
    }
    TLista p,aux,ant;
    // comparam cu primul element daca nu cumva trebuie sa 
    // schimbam inceputul listei
    int result = f((*l)->info, element);
    if(result == 1)
    {
        aux = InitCelula(element);
        if(!aux)
        {
            fprintf(stderr, "Eroare alocare celula la inserare\n");
            return -1;
        }
        aux->urm = *l;
        *l = aux;
        return 1;
    }
    int i;
    //parcurgem lista de categorii
    for(i = 1, ant = NULL, p = *l; p != NULL; ant = p, p = p->urm, i++)
    {
        //comparam fiecare element
        result = f(p->info, element);
        //daca cumva exista -> return
        if(result == 0) return -1;
        //daca e result 1 -> inseram in fata celulei curente
        if(result == 1)
        {
            aux = InitCelula(element);
            if(!aux)
            {
                fprintf(stderr, "Eroare alocare celula la inserare\n");
                return -1;
            }
            aux->urm = ant->urm;
            ant->urm = aux;
            return i;
        }
    }
    //Inserare la sfarsit(caz in care functia de comparare nu a gasit nici o celula
    //in fata careia s-ar fi putut de insera
    ant->urm = InitCelula(element);
    if(!(ant->urm))
    {
        fprintf(stderr, "Eroare alocare celula la inserare\n");
        return -1;
    }
    return i;
}

//Inserare in lista de top10
//*l - lista, element -> serialul alocat, pos -> pozitia in top
TLista inserare_top_10(TLista * l, void * element, int pos)
{
    TLista p;
    TLista ant, extra = NULL, found, found_anterior;
    int i;
    TLista aux = InitCelula(element);
    if(!aux)
    {
        fprintf(stderr, "Eroare InitCelula in inserare_top_10\n") ;
        return NULL;
    }
    //daca e lista vida
    //insereaza
    if(*l == NULL)
    {
        *l = aux;
        return NULL;
    }
    //parcurgere lista de categorii
    for (i = 1, ant = NULL, p = *l; i <= 10; i++, ant = p, p = p->urm)
    {
        //daca cumva pozitia de inserat este prea mare
        if(i < pos && p == NULL) return NULL;
        //daca am ajuns la pozitia dorita
        //memoreaza pe gasit si anteriorul sau
        if (i == pos)
        {
            found = p;
            found_anterior = ant;
        }
        //daca in lista deja sunt 10 elemente
        //memoreaza-l pentru a-l returna
        //si sparge legatura precedentului cu el
        if(i == 10 && p != NULL)
        {
            extra = p;
            ant->urm = NULL;
        }
        //daca nu am ajuns la pozitia a 10 si deja e NULL
        // incheie for-ul(ca sa nu facem p = p->urm)
        if(p == NULL) break;
    }
    if(found_anterior == NULL)
    {
        //daca e inserare in pozitia 1
		aux->urm = *l;
        *l = aux;
    }
    else
    {
        //inserare in interior
        found_anterior->urm = aux;
        //daca e inserare in ultima pozitia(10)
        if(pos == 10) aux->urm = NULL;
        //de altfel ->
        else aux->urm = found;
    }
    return extra;
}

//Functie de afisare a unei liste cu ajutorul Fafi print
void afisare_lista(TLista l, Fafi print, FILE * output)
{
    fprintf(output,"[");
    for(TLista p = l; p != NULL; p = p->urm)
    {
        print(p->info,output);
        if(p->urm == NULL) break;
        else fprintf(output,", ");
    }
    fprintf(output,"].\n");
}

//Functie de eliminare din lista(fara dezalocare)
//care implica si refacere legaturi
void eliminare_din_lista(TLista *l, TLista el)
{
    if(*l == NULL) return;
    if(*l == el)
    {
        *l = (*l)->urm;
        el->urm = NULL;
        return;
    }
    TLista ant, p;
    for(ant = NULL, p = *l; p != NULL; ant = p, p = p->urm)
    {
        if(p == el)
        {
            ant->urm = p->urm;
            el->urm = NULL;
            return;
        }
    }
}

//Distruge lista de categorii cu ajutorul TF elib
//pentru informatia continuta de celula
void distruge_categorii(TLista * categorii, TF elib)
{
    TLista p,aux;
    for(int i = 0; i < 4; i++)
    {
        for(p = categorii[i]; p != NULL; )
        {
            aux = p;
            p = p->urm;
            elib(aux->info);
            free(aux);
        }
        categorii[i] = NULL;
    }
    free(categorii);
}
#include "coada.h"

//Initializeaza coada
TCoada InitQ()
{
    TCoada c;
    c = (TCoada)calloc(1,sizeof(Coada));
    if (!c) return NULL;

    c->inceput = NULL;
    c->sfarsit = NULL;
    c->number_of_elements = 0;
    return c;
}

//Insereaza in coada
int InsQ(TCoada c, TLista aux)
{
    if(c->inceput == NULL && c->sfarsit == NULL)
    {
        c->inceput = aux;
        c->sfarsit = aux;
    }
    else
    {
        c->sfarsit->urm = aux;
        c->sfarsit = aux;
    }
    c->number_of_elements++;
    return 1;
}

//Reseteaza o coada(elemente in ea deja nu se afla)
void ResetQ(TCoada c)
{
    c->inceput = c->sfarsit = NULL;
    c->number_of_elements = 0;
}

//Distruge o coada
void DistrQ(TCoada *c)
{
    ResetQ(*c);
    free(*c);
    *c = NULL;
}

//Extrage dintr-o coada
void ExtrQ(TCoada c, TLista * el)
{
    if(VC(c))
    {
        return;
    }
    TLista aux = c->inceput;
    c->inceput = aux->urm;
    *el = aux;
    //spargerea legaturii cu elementele ce urmeaza
    (*el)->urm = NULL;
    if(c->inceput == NULL) c->sfarsit = NULL;
    c->number_of_elements--;
}

//Concateneaza doua cozi(ad->destinatia; as->sursa)
void ConcatQ(TCoada ad, TCoada as)
{
    if(VC(as)) return;
    TLista aux;
    while(VC(as) != 1)
    {
        ExtrQ(as,&aux);
        InsQ(ad,aux);
    }
    ResetQ(as);
}

//Extrage din coada un element in baza unei referinte
//c -> coada, element -> referinta, cmp -> functia de comparare
//res -> parametru care specifica daca e nevoie de eliminat din coada
//sau nu (1 -> elimina din coada, 0 -> nu elimina ci doar intoarce 
//un pointer catre celula respectiva)
TLista extrage_ref_coada(TCoada c, void * element, Fcmp cmp, int res)
{
    if(VC(c)) return NULL;
    TCoada c_aux = InitQ();
    if(!c_aux) return NULL;
    TLista aux,gasit = NULL;
    while(VC(c) != 1)
    {
        ExtrQ(c,&aux);
        if(cmp(aux->info,element) == 0)
        {
            gasit = aux;
            if(res == 1)//daca res e 1 -> reface coada 
                        //doar ca fara celula gasita
                        //si sterge legatura ei cu elementele
                        //urmatoare
            {
                ConcatQ(c_aux, c);
                ConcatQ(c,c_aux);
                gasit->urm = NULL;
                DistrQ(&c_aux);
                return gasit;
            }
        }
        InsQ(c_aux,aux);
    }
    ConcatQ(c,c_aux);
    DistrQ(&c_aux);
    return gasit;
}

//functie de afisare a cozilor watch_later si history
//(poarta denumirea afi_later deoarece initial a fost
//implementata pentru a afisa doar watch_later)
//se foloseste Fafi f ca functie de afisare
void afi_later(TCoada c, Fafi f, FILE * output)
{
    if(VC(c) == 1) return;
    TCoada c_aux = InitQ();
    if(!c_aux)
    {
        fprintf(stderr,"Eroare InitQ pentru coada aux in afi_later");
        return;
    }
    TLista el;
    while(VC(c) == 0)
    {
        ExtrQ(c, &el);
        InsQ(c_aux, el);
        f(el->info, output);
        if(!VC(c)) fprintf(output,", ");
    }
    ConcatQ(c,c_aux);
    DistrQ(&c_aux);
}

//Functie de distrugere a unei cozi
//e -> coada ce trebuie eliminata
//elib -> functie de eliberare a informatiei
//din celulele cozii
void distruge_coada(TCoada * e, TF elib)
{
    TCoada c = *e;
    TLista aux;
    while(VC(c) != 1)
    {
        ExtrQ(c, &aux);
        elib(aux->info);
        free(aux);
    }
    DistrQ(e);
}
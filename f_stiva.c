#include "stiva.h"

//Functie de initializare a unei structuri de tip TStiva
TStiva InitS()
{
    TStiva s;
    s = (TStiva)calloc(1,sizeof(Stiva));
    if(!s) return NULL;

    s->varf = NULL;
    s->number_of_elements = 0;
    return s;
}

//Inserare in stiva
void Push(TStiva s, TLista aux)
{
    aux->urm = VF(s);
    VF(s) = aux;
    s->number_of_elements++;
}

//Extragere din stiva
void Pop(TStiva s, TLista * aux)
{
    if(SV(s)) return;
    *aux = VF(s);
    VF(s) = (*aux)->urm;
    (*aux)->urm = NULL;
    s->number_of_elements--;
    return;
}

//Functie de rasturnare a unei stive sursa in stiva destinatie
void Rastoarna(TStiva d, TStiva s)
{
    TLista p = VF(s);
    TLista aux;
    while(p)
    {
        Pop(s, &aux);
        Push(d,aux);
        p = VF(s);
    }
}

//Functie de distrugere a unei stive
void DistrS(TStiva *s)
{
    VF(*s)=NULL;
    free(*s);
    *s = NULL;
}

//Functie ce afiseaza stiva history cu ajutorul
//functiei Fafi f de afisare
void afi_history(TStiva s, Fafi f, FILE * output)
{
    if (SV(s)) return;
    TStiva s_aux = InitS();
    if(!s_aux)
    {
        fprintf(stderr,"Eroare InitS pentru stiva aux in afi_history");
        return;
    }
    TLista aux;
    while(!SV(s))
    {
        Pop(s,&aux);
        Push(s_aux,aux);
        f(aux->info,output);
        if(!SV(s)) fprintf(output,", ");
    }
    Rastoarna(s,s_aux);
    DistrS(&s_aux);
}

//Functie de extragere a unei celule din stiva s
//in baza unei referinte element cu ajutorul functiei de comparare
// Fcmp cmp. Parametrul res specifica nevoia de a extrage sau nu
//celula corespunzatoare din stiva (1 -> extrage, 0 -> nu extrage)
TLista extrage_ref_stiva(TStiva s, void * element, Fcmp cmp, int res)
{
    if(SV(s)) return NULL;
    TStiva s_aux = InitS();
    if(!s_aux)
    {
        fprintf(stderr,"Eroare InitS pentru stiva aux in extrage_ref_stiva");
        return NULL;
    }
    TLista aux, gasit = NULL;
    while(SV(s) != 1)
    {
        Pop(s,&aux); // extrage
        //verifica
        if(cmp(aux->info, element) == 0)
        {
            //salveaza
            gasit = aux;
            if (res == 0) //daca nu e nevoie sa eliminam din stiva
                Push(s_aux, aux);   //introduce inapoi in stiva aux
            continue;   //mergi la urmatorul pas
        }
        Push(s_aux, aux);
    }
    Rastoarna(s,s_aux); //reface stiva initiala
    DistrS(&s_aux); //distruge stiva auxiliara
    if(gasit && res == 1) gasit->urm = NULL; //daca e nevoie de a elimina din stiva
                                             //sparge legaturile cu urmatoarele elemente
    return gasit;
}


//Functie de distrugere a stivei s cu ajutorul functiei
//TF elib a informatiei din celulele ei
void distruge_stiva(TStiva * s, TF elib)
{
    TLista aux;
    while(SV(*s) != 1)
    {
        Pop(*s, &aux);
        elib(aux->info);
        free(aux);
    }
    DistrS(s);
}
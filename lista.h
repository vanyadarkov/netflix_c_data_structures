#ifndef _LISTA_SIMPLU_INLANTUINTA_
#define _LISTA_SIMPLU_INLANTUINTA_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Prototip functie de comparare
typedef int (*Fcmp)(void * , void *);
//Prototip functie de afisare
typedef void (*Fafi)(void *, FILE *);
//Prototip functie de prelucrare a void*
typedef void (*TF)(void *);

//Structura unei liste simplu inlantuite
typedef struct _celula_
{
    void * info;
    struct _celula_ * urm;
}   TCelula, *TLista;


TLista InitCelula(void *);
int inserare_lista(TLista * l, void * element, Fcmp f);
void afisare_lista(TLista l, Fafi print, FILE *);
TLista inserare_top_10(TLista * l, void * element, int pos);
void eliminare_din_lista(TLista *l, TLista el);
void distruge_categorii(TLista * categorii, TF elib);

#endif
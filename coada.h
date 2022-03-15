#ifndef _COADA_
#define _COADA_
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structura unei cozi
typedef struct _coada_
{
    TLista inceput;
    TLista sfarsit;
    int number_of_elements;
} Coada, *TCoada;

//Prototip functie de prelucrare a void*
typedef void (*TF)(void *);

TCoada InitQ();
int InsQ(TCoada, TLista);
void ExtrQ(TCoada , TLista *);
void ResetQ(TCoada);
void DistrQ(TCoada *c);
void ConcatQ(TCoada ad, TCoada as);
void afi_later(TCoada c, Fafi f, FILE *);
TLista extrage_ref_coada(TCoada c, void * element, Fcmp cmp, int res);
void distruge_coada(TCoada * c, TF elib);

//test coada vida (1 => Coada vida 0 => Coada ne vida)
#define VC(a) ((((TCoada)a)->inceput)==NULL && (((TCoada)a)->sfarsit)==NULL) 

#endif
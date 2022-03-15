#ifndef _STIVA_
#define _STIVA_
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structura unei stive
typedef struct _stiva_
{
    TLista varf;
    int number_of_elements;
} Stiva, *TStiva;

//Obtinerea varfului stivei
#define VF(a) (((TStiva)a)->varf)
//Testi stiva vida ( 1 => stiva vida 0 => stiva nevida )
#define SV(a)   ((((TStiva)a)->varf) == NULL)

TStiva InitS();
void Push(TStiva s, TLista aux);
void Pop(TStiva s, TLista * aux);
void DistrS(TStiva *s);
void Rastoarna(TStiva d, TStiva s);
void distruge_stiva(TStiva * s, TF elib);
TLista extrage_ref_stiva(TStiva s, void * element, Fcmp cmp, int res);
void afi_history(TStiva s,Fafi f, FILE * output);

#endif
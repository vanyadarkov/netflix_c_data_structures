#ifndef _SERIAL_
#define _SERIAL

#include "coada.h"
#include "lista.h"
#include "stiva.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 33

//Structura unui serial
typedef struct _serial_{
    int ID;
    char * name;
    float rating;
    int number_of_seasons;
    TCoada seasons;
}  Serial;

void AfisareSerial(void * e, FILE *);
void * AlocareSerial(int ID, char * name, float rating, int nr_s,TCoada seasons);
void DistrugeSerial(void * e);
int Comparare(void * e1, void * e2);
void add_serial(TLista * categorii, Fcmp cmp, FILE * output);
TLista find_serial(TLista * categorii, void * element, Fcmp cmp);
void add_new_season(void * g, void * element, FILE * output);
void show(char * request, Fafi print, TLista * categorii, TCoada watch_later, TStiva currently_watching, TStiva history, FILE * output);
void prepare_for_add_sezon(TLista * categorii, TCoada watch_later, TStiva currently_watching, TStiva history, Fcmp cmp, FILE * output);
void prepare_for_add_top10(TLista * categorii, TCoada watch_later, TStiva currently_watching, TStiva history,Fafi print, Fcmp cmp, FILE * output);
void watch_serial_later(TLista * categorii,TCoada watch_later, Fcmp cmp, FILE * output);
void prepare_for_watch(TLista * categorii, TCoada watch_later, TStiva currently_watching, TStiva history,Fafi print, Fcmp cmp, FILE * output);
int watch_serial(void *s, int duration);

#endif
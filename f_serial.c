#include "serial.h"

//functia de afisare a serialului
void AfisareSerial(void * e, FILE * output)
{
    if(!e) return;
    Serial * s = e;
    fprintf(output, "(%s, %.1f)", s->name, s->rating);
}

//functia care dezaloca un serial si campurile interioare alocate
void DistrugeSerial(void * e)
{
    if(!e) return;
    Serial * s = e;
    free(s->name);
    TLista aux;
    if(s->seasons != NULL)
    {
        while(VC(s->seasons) != 1)
        {
            ExtrQ(s->seasons, &aux);
            distruge_coada((TCoada*)(&(aux->info)), free);
            free(aux);
        }
        DistrQ(&(s->seasons));
    }
    free(s);
}

//functie care primeste un serial si intoarce ID-ul lui
int get_serial_id(void * x)
{
    Serial * s = x;
    return s->ID;
}

//functia care aloca un serial
void * AlocareSerial(int ID, char * name, float rating, int nr_s, TCoada seasons)
{
    Serial * serial = (Serial *)calloc(1, sizeof(Serial));
    if (!serial) return NULL;
    serial->ID = ID;
    serial->name = calloc(MAX_NAME, sizeof(char));
    if(!serial->name)
    {
        free(serial);
        return NULL;
    }
    strcpy(serial->name,name);
    serial->rating = rating;
    serial->number_of_seasons = nr_s;
    if(seasons == NULL) return serial;
    serial->seasons = seasons;
    return serial;
}

//functia de comparare
int Comparare(void * e1, void * e2)
{

    Serial* s1 = e1;
    Serial* s2 = e2;
    if(strcmp(s1->name, s2->name) == 0) return 0;
    if(s2->rating > s1->rating) return 1;
    if(s2->rating < s1->rating) return -1;
    if(s2->rating == s1->rating)
    {
        int result = strcmp(s1->name, s2->name);
        if (result == 0) return 0;
        if (result < 0 ) return 2;
        if (result > 0) return 1;
    }
    return -1;   
}
//Functie de adaugare serial nou in lista de categorii
void add_serial(TLista * categorii, Fcmp cmp, FILE * output)
{
    //Extragere valori necesare din linie
    int ID = atoi(strtok(NULL, " "));
    char * name = strtok(NULL," ");
    float rating = atof(strtok(NULL," "));
    int nr_of_seasons = atoi(strtok(NULL, " "));
    TCoada c_episoade;
    TCoada c_sezoane = InitQ();
    if(!c_sezoane)
    {
        fprintf(stderr, "Eroare InitQ\n");
        return;
    }
    int nr_of_episodes;
    //Introducerea valorilor in coada sezon a structurii de serial
    for (int i = 0; i < nr_of_seasons; i++)
    {
        c_episoade = InitQ();
        if(!c_episoade)
        {
            fprintf(stderr, "Eroare InitQ pentru sezonul %d\n", i + 1);
            return;
        }
        nr_of_episodes = atoi(strtok(NULL, " "));
        //Extragere si introducere in coada de episoade episoadele serialului
        for(int j = 1; j <= nr_of_episodes; j++)
        {
            int * duration = calloc(1, sizeof(int));
            if(!duration) 
            {
                fprintf(stderr, "Eroare alocare episod %d in sezonul %d\n", j, i);
                return;
            }
            *duration = atoi(strtok(NULL, " "));
            //Alocare celula cu informatia durata episodului
            TLista aux = InitCelula(duration);
            if(!aux)
            {
                fprintf(stderr, "Eroare InitCelula pentru add top10");
                return;
            }
            //Inserare coada episod in coada de sezoane
            InsQ(c_episoade,aux);
        }
        TLista aux = InitCelula(c_episoade);
        if(!aux)
        {
            fprintf(stderr, "Eroare InitCelula pentru add top10");
            return;
        }
        InsQ(c_sezoane,aux);
    }
    //Alocare structura serial cu fiecare camp pregatit
    void * element = AlocareSerial(ID,name,rating,nr_of_seasons,c_sezoane);
    if(!element)
    {
        fprintf(stderr, "Eroare alocare serial\n");
        return;
    }
    //Inserare propriu zisa in categorii[ID - 1 ]
    //            (adica la lista corespunzatoare categoriei serialului curent)
    fprintf(output,"Serialul %s a fost adaugat la pozitia %d.\n",name, 
                        inserare_lista(&categorii[ID - 1], element, cmp));
}

//functie de pregatire pentru a adauga un sezon nou
void prepare_for_add_sezon(TLista * categorii, TCoada watch_later, 
                            TStiva currently_watching, TStiva history, 
                                                Fcmp cmp, FILE * output)
{
    char * name = strtok(NULL, " ");
    int nr_of_episodes = atoi(strtok(NULL, " "));
    TCoada c_episoade = InitQ();
    if(!c_episoade)
    {
        fprintf(stderr,"Eroare alocare coada episoade la adaugare sezon\n");
        return;
    }
    //Aceeasi procedura ca la add_serial
    //doar ca de data aceasta se face pentru un singur sezon
    for(int i = 1; i <= nr_of_episodes; i++)
    {
        int * duration = calloc(1,sizeof(int));
        if(!duration) 
        {
            fprintf(stderr, "eroare alocare int pentru sezon nou\n");
            return;
        }
        *duration = atoi(strtok(NULL, " "));
        TLista aux = InitCelula(duration);
        if(!aux)
        {
            fprintf(stderr, "Eroare InitCelula pentru episod nou\n");
            return;
        }
        InsQ(c_episoade, aux);
    }
    TCoada c_sezon = InitQ();
    if(!c_sezon)
    {
        fprintf(stderr,"Eroare alocare coada sezon la adaugare sezon\n");
        return;
    }
    TLista aux = InitCelula(c_episoade);
    if(!aux)
    {
        fprintf(stderr, "Eroare InitCelula pentru coada de sezon nou\n");
        return;
    }
    InsQ(c_sezon,aux);
    //Alocare un serial auxiliar pentru a identifica in ce structura
    //se afla serialul in care trebuie de inserat
    void *element = AlocareSerial(1, name, 0.0, 1, c_sezon);
    if(!element)
    {
        fprintf(stderr, "Eroare alocare serial aux pentru add_sez\n");
        return;
    }
    //cautam ca nu cumva sa fie deja in history
    TLista gasit = extrage_ref_stiva(history, element, cmp, 0);
    if(gasit != NULL) 
    {
        DistrugeSerial(element);
        return;
    }
    //cautam in categorii
    gasit = find_serial(categorii, element, cmp);
    if(gasit == NULL)
    {
        //apoi in watch later
        gasit = extrage_ref_coada(watch_later,element,cmp, 0);
        if (gasit == NULL)
        {
            //apoi in currently_wataching
            gasit = extrage_ref_stiva(currently_watching,element, cmp, 0);
            if (gasit == NULL)
            {
                DistrugeSerial(element);
                return;
            } 
        }
    }
    //adaugarea propriu zisa a sezonului in Serialul gasit
    add_new_season(gasit->info, element, output);
    DistrugeSerial(element);
}

//functia care pregateste pentru inserare in top10
void prepare_for_add_top10(TLista * categorii, TCoada watch_later, 
                            TStiva currently_watching, TStiva history, 
                                    Fafi print, Fcmp cmp, FILE * output)
{
    //pentru serialele din top 10 ID lor va fi 4(in lista de categorii 4 = top10)
    int ID = 4;
    int position = atoi(strtok(NULL, " "));
    char * name = strtok(NULL," ");
    float rating = atof(strtok(NULL," "));
    int nr_of_seasons = atoi(strtok(NULL, " "));
    TCoada c_episoade;
    TCoada c_sezoane = InitQ();
    if(!c_sezoane)
    {
        fprintf(stderr,"Eroare alocare coada sezoane la adaugare top10\n");
        return;
    }
    int nr_of_episodes;
    //la fel procesul de completare a cozii de sezoane
    //cu coada de episoada
    for (int i = 0; i < nr_of_seasons; i++)
    {
        c_episoade = InitQ();
        if(!c_episoade)
        {
            fprintf(stderr, "Eroare InitQ pentru sezonul %d add top 10\n", i + 1);
            return;
        }
        nr_of_episodes = atoi(strtok(NULL, " "));
        for(int j = 1; j <= nr_of_episodes; j++)
        {
            int * duration = calloc(1, sizeof(int));
            if(!duration) 
            {
                fprintf(stderr, "Eroare alocare episod %d in episodul %d add top10\n", j, i);
                return;
            }
            *duration = atoi(strtok(NULL, " "));
            TLista aux = InitCelula(duration);
            if(!aux)
            {
                fprintf(stderr, "Eroare InitCelula pentru add top10");
                return;
            }
            InsQ(c_episoade,aux);
        }
        TLista aux = InitCelula(c_episoade);
        if(!aux)
        {
            fprintf(stderr, "Eroare InitCelula pentru add top10");
            return;
        }
        InsQ(c_sezoane,aux);
    }
    void * element = AlocareSerial(ID,name,rating,nr_of_seasons,c_sezoane);
    if(!element)
    {
        fprintf(stderr, "Eroare alocare serial\n");
        return;
    }
    //functia propriu zisa de inserare in lista de top10
    TLista extra = inserare_top_10(&categorii[3], element, position);
    //verificare daca nu au fost > 10 seriale in top 10
    if(extra != NULL) 
    {
        //daca da distruge serialul si celula in care era stocat
        DistrugeSerial(extra->info);
        free(extra);
    }
    fprintf(output,"Categoria top10: ");
    afisare_lista(categorii[3], print,output);
}

//functie de afisare a fiecarei categorii de seriale
void show(char * request, Fafi print, TLista * categorii, 
                TCoada watch_later, TStiva currently_watching, 
                                    TStiva history, FILE * output)
{
    if(strcmp(request,"top10") == 0)
    {   
        fprintf(output,"Categoria top10: ");
        afisare_lista(categorii[3], print, output);
        return;
    }
    if((int)(*request) >= 48 && (int)(*request) <= 57)
    {
        fprintf(output,"Categoria %d: ", atoi(request));
        afisare_lista(categorii[atoi(request) - 1], print, output);
        return;
    }
    if(strcmp(request,"later") == 0)
    {
        fprintf(output,"Categoria %s: [",request);
        afi_later(watch_later, print, output);
        fprintf(output,"].\n");
        return;
    }
    if(strcmp(request,"history") == 0)
    {
        fprintf(output,"Categoria %s: [",request);
        afi_history(history, print, output);
        fprintf(output,"].\n");
        return;
    }
    if(strcmp(request,"watching") == 0)
    {
        fprintf(output,"Categoria %s: [",request);
        afi_history(currently_watching, print, output);
        fprintf(output,"].\n");
        return;
    }
}

//functia care primeste vectorul de categorii, o structura de serial
//si functia de comparare
//returneaza celula care are acelasi nume de serial ca structura
//de referinta primita ca parametru
TLista find_serial(TLista * categorii, void * element, Fcmp cmp)
{
    TLista p;
    for(int i = 0; i < 4; i++)
    {
        for(p = categorii[i]; p != NULL; p = p->urm)
        {
            if(cmp(p->info, element) == 0)
            {
                return p;
            }
        }
    }
    return NULL;
}

//functie de adaugarea propriu zisa a unui sezon intr-o structura de serial
//deja existenta
//g - serialul in care adaugam, element - serialul care contine coada
//cu sezonul ce trebuie adaugat, output - stream-ul de afisare
void add_new_season(void * g, void * element, FILE * output)
{
    Serial * gasit = g;
    Serial * aux = element;
    TLista sezon;
    //se extrage unicul sezon din serialul auxiliar
    ExtrQ(aux->seasons, &sezon);
    //si se insereaza in serialul principal
    InsQ(gasit->seasons, sezon);
    fprintf(output,"Serialul %s are un sezon nou.\n", gasit->name);
    //s-a marit numarul de sezoane in serialul gasit
    gasit->number_of_seasons++;
}

//functie de adaugare a unui serial in coada watch_later
void watch_serial_later(TLista * categorii,TCoada watch_later, Fcmp cmp, FILE * output)
{
    char * nume = strtok(NULL," ");
    //alocare unui element aux in care ne intereseaza doar campul nume
    void * element = AlocareSerial(0, nume, 0.0, 0, NULL);
    if(!element)
    {
        fprintf(stderr, "Eroare alocare serial aux in watch_serial_later\n");
        return;
    }
    //folosim element pentru a gasi serialul care il adaugam in watch_later
    TLista gasit = find_serial(categorii, element, cmp);
    DistrugeSerial(element); //disrugem elementul aux
    if (!gasit) return; //daca nu a fost gasit in categorii -> continuam la alta linie
    InsQ(watch_later, gasit);   //de altfel insereaza in watch_later
    //si elimina din lista de categorii unde se afla
    eliminare_din_lista(&categorii[get_serial_id(gasit->info) - 1], gasit); 
    fprintf(output,"Serialul %s se afla in coada de asteptare pe pozitia %d.\n", 
                                nume, watch_later->number_of_elements);
}

//functie de pregatire inainte de a privi un serial
void prepare_for_watch(TLista * categorii, TCoada watch_later, 
                            TStiva currently_watching, TStiva history, 
                                    Fafi print, Fcmp cmp, FILE * output)
{
    char * name = strtok(NULL, " ");
    int duration = atoi(strtok(NULL, " "));
    void * element = AlocareSerial(0, name, 0.0, 0, NULL);
    if(!element)
    {
        fprintf(stderr, "Eroare alocare serial aux in prepare_for_watch\n");
        return;
    }
    //cautam in stiva history(daca se afla acolo, nu mai putem sa privim serialul)
    //cerut
    TLista gasit = extrage_ref_stiva(history,element,cmp, 0);
    if(gasit != NULL) 
    {
        DistrugeSerial(element);
        return;
    }
    //de altfel cautam in restul structurilor in care ar putea sa se afle
    gasit = find_serial(categorii, element, cmp);
    if(gasit == NULL)
    {
        gasit = extrage_ref_coada(watch_later,element,cmp, 1);
        if (gasit == NULL)
        {
            gasit = extrage_ref_stiva(currently_watching,element, cmp, 1);
            if (gasit == NULL)
            {
                DistrugeSerial(element);
                return;
            } 
        }
    }
    else
    {
        //daca s-a aflat in lista de categorii - eliminam de acolo
        eliminare_din_lista(&categorii[get_serial_id(gasit->info) - 1], gasit);
    }
    //daca a fost gasit -> nu mai avem nevoie de elementul auxiliar
    DistrugeSerial(element);
    //result -> primeste 1 -> serial vizionat integral
    //         primeste 0 -> serial vizionat partial
    //         primeste -1 -> in caz de insucces la alocare
    // iar functia watch_serial -> functia ce vizioneaaza propriu zis serialul
    int result = watch_serial(gasit->info,duration);
    if(result) 
    {
        fprintf(output,"Serialul %s a fost vizionat integral.\n",name);
        //daca a fost vizionat integral -> inseram in history
        Push(history,gasit);
    }
    else 
    {
        //daca nu -> il introducem in currently_watching
        Push(currently_watching,gasit);
    }
}


//functia propriu zisa de privire a unui serial
//primeste serialul pe care il privim si durata de vizionare
int watch_serial(void *s, int duration)
{
    Serial * serial = s;
    //avem nevoie de cozi auxiliare pentru a extrage
    //o coada de sezon sau o coada de seriale
    TCoada s_aux = InitQ();
    if(!s_aux) return -1;
    TCoada e_aux = InitQ();
    if(!e_aux) return -1;
    TLista sezon, episod;
    
    while(VC(serial->seasons) != 1)
    {
        //extragem un sezon din coada 
        ExtrQ(serial->seasons,&sezon);
        while(VC(sezon->info) != 1)
        {
            //din sezon extrag un episod
            ExtrQ(sezon->info,&episod);
            if(duration >= *(int*)(episod->info))
            {
                //s-a vizionat un episod intreg
                //si e nevoie sa il dezalocam
                //cat si sa decrementam durata de vizionare
                duration -= *(int*)(episod->info);
                free(episod->info);
                free(episod);
            }
            else
            {
                //s-a vizionat partial un episod
                *(int*)(episod->info) -= duration;
                duration = 0;
                //il introducem in coada auxiliara
                //ca eventual sa il introducem inapoi la sezonul sau corespunzator
                InsQ(e_aux,episod);
            }
        }
        if(VC(e_aux)) 
        {
            //daca s-au vizionat toate episoadele
            //distruge coada de episoade
            DistrQ((TCoada*)(&(sezon->info)));
            free(sezon);
            serial->number_of_seasons--;
            continue;
        }
        //de altfel -> introduce coada de episoade inapoi la sezon
        else ConcatQ(sezon->info, e_aux); 
        //insereaza sezonul in coada aux pentru a concatena la sfarsit
        //la caoda de sezoane initiala
        InsQ(s_aux,sezon);
    }
    ConcatQ(serial->seasons,s_aux);
    DistrQ(&s_aux);
    DistrQ(&e_aux);
    //daca nu a fost inserat nici un sezon
    //inseamna ca s-a vizionat serialul integral
    if(VC(serial->seasons) == 1)
    {
        DistrQ(&(serial->seasons));
        return 1;
    }
    return 0;
}
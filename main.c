#include "stiva.h"
#include "coada.h"
#include "lista.h"
#include "serial.h"

#define MAX_LINE 256


//Functie de citire din fisier de input
void read_from_file(FILE * input, FILE * output, TLista * categorii, 
                        TCoada watch_later, TStiva currently_watching, 
                                    TStiva history, Fcmp cmp, Fafi print)
{
    char * line = calloc(MAX_LINE, sizeof(char));
    if(!line) return;
    char * command;

    while (fgets(line,MAX_LINE, input) != NULL)
    {
        //Anulare \n la sfarsitul unei noi linii citite
        if(line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
        //Skip daca e o linie vida\n;
        if(strcmp(line,"") == 0) continue;

        //extragerea comenzii
        command = strtok(line," ");

        //recunoasterea comenzii si apelare de functii
        if(strcmp(command,"add") == 0)
        {
            add_serial(categorii, cmp, output);
        }
        if(strcmp(command,"show") == 0)
        {
            char * request = strtok(NULL, " ");
            show(request,print,categorii, watch_later,
                    currently_watching, history, output);
        }
        if(strcmp(command,"add_sez") == 0)
        {
            prepare_for_add_sezon(categorii, watch_later, 
                currently_watching, history, cmp, output);
        }
        if(strcmp(command,"add_top") == 0)
        {
            prepare_for_add_top10(categorii, watch_later, 
                currently_watching, history, print, cmp, output);
        }
        if(strcmp(command,"later") == 0)
        {
            watch_serial_later(categorii, watch_later, cmp, output);
        }
        if(strcmp(command,"watch") == 0)
        {
            prepare_for_watch(categorii, watch_later, currently_watching, 
                                                history, print, cmp, output);
        }
    }
    free(line);
}

int main(int argc, char *argv[])
{
    //Conditie necesara pentru de argumente
    if(argc < 3)
    {
        fprintf(stderr,"Argumente insuficiente\n");
        return -1;
    }
    //Alocare vector de lista pentru fiecare categorie
    TLista * serials_cat = (TLista*)calloc(4,sizeof(TLista));
    if(!serials_cat) 
    {
        fprintf(stderr, "Eroare alocare vector categorii\n");
        return -1;
    }
    //Init structuri
    TCoada watch_later = InitQ();
    if(!watch_later)
    {
        fprintf(stderr, "Eroare InitQ watch_later\n");
        free(serials_cat);
        return -1;
    }
    TStiva currently_watching = InitS();
    if(!currently_watching)
    {
        fprintf(stderr, "Eroare InitS currently_watching\n");
        DistrQ(&watch_later);
        free(serials_cat);
        return -1;
    }
    TStiva history = InitS();
    if(!history)
    {
        fprintf(stderr, "Eroare InitS history\n");
        DistrS(&currently_watching);
        DistrQ(&watch_later);
        free(serials_cat);
        return -1;
    }

    FILE * input_file = fopen(argv[1],"r");
    FILE * output_file = fopen(argv[2], "w");
    if(!output_file || !input_file)
    {
        fprintf(stderr,"eroare deschidere fisier\n");
        return -1;
    }

    read_from_file(input_file,output_file, serials_cat, watch_later, 
                currently_watching, history, Comparare, AfisareSerial);

    fclose(output_file);
    fclose(input_file);

    distruge_categorii(serials_cat, DistrugeSerial);
    distruge_coada(&watch_later, DistrugeSerial);
    distruge_stiva(&currently_watching,DistrugeSerial);
    distruge_stiva(&history, DistrugeSerial);
    
    return 0;
}
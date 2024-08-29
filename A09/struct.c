#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <csv.h>
#define BUFFER_SIZE 1024

typedef struct {
    char **Kopftabelle;
    char *** Zeilen;
    int Anzahl_Spalten;
    int Anzahl_Zeilen;
}csv_data;

void Zelle_Funct(void *s, size_t len, void *data);
void Zeile_Funct(int s, void *data);
void text_maj(char *satz);
void text_min(char *satz);
void print_csv(csv_data *Tomi, int Grossbuchstaben, int Kleinbuchstaben, int filter);


int main(int argc, char *argv[])
{
    FILE *tabelle = fopen("Aufgabe_09.csv", "r");
    if (!tabelle){
        perror("fehler beim Öffnen dem Datei");
        return(EXIT_FAILURE);
    }
    csv_data Tomi = {
        NULL , NULL, -1, 0
    };
    struct csv_parser p;
    csv_init(&p, 0);
    char buffer[BUFFER_SIZE];
    size_t bytes;
    while(bytes = fread(buffer, 1, BUFFER_SIZE, tabelle)>0){
        if (csv_parse(&p, buffer, bytes, Zelle_Funct, Zeile_Funct, &Tomi)!= bytes){
            fprintf(stderr, "Fehler während parsen der Datei\n");
            return(0);
      }
      csv_fini(&p, Zelle_Funct, Zeile_Funct, &Tomi);
      csv_free(&p);
      fclose(tabelle);
      int Grossbuchstaben=0, Kleinbuchstaben=0, filter=0, i, j;
      for (i=1; i<argc; i++){
        if (strcmp(argv[i], "-u")==0){
            Grossbuchstaben=1;
        }
        if (strcmp(argv[i], "-l")==0){
            Kleinbuchstaben=1;
      }
      if (strcmp(argv[i], "-f")==0){
            filter=1;
      }
    }
    print_csv(&Tomi, Grossbuchstaben, Kleinbuchstaben, filter);
    for (i=0; i<Tomi.Anzahl_Spalten;i++) free(Tomi.Kopftabelle[i]);
    for (i=0; i<Tomi.Anzahl_Zeilen; i++){
             for (j=0; j<Tomi.Anzahl_Spalten;j++) free(Tomi.Zeilen[i][j]);
    free(Tomi.Zeilen[i]);

    }
    free(Tomi.Kopftabelle);
    free(Tomi.Zeilen);
    return 0;
}
}

void Zelle_Funct(void *s, size_t len, void *data){

csv_data *Tomi = (csv_data *)data;
if (Tomi->Anzahl_Spalten < 0){
    Tomi->Kopftabelle = realloc(Tomi->Kopftabelle, (Tomi->Anzahl_Spalten+2)*sizeof(char *));
    Tomi->Kopftabelle[Tomi->Anzahl_Spalten] = strndup(s, len);
    Tomi->Anzahl_Spalten++;}
else {
    Tomi->Zeilen[Tomi->Anzahl_Zeilen] = realloc(Tomi->Zeilen[Tomi->Anzahl_Zeilen], (Tomi->Anzahl_Spalten+1)*sizeof(char *));
     Tomi->Zeilen[Tomi->Anzahl_Zeilen][Tomi->Anzahl_Spalten] = strndup(s,len);
}

}


void Zeile_Funct(int s, void *data){
csv_data *Tomi = (csv_data *)data;
if (Tomi->Anzahl_Spalten >= 0){
    Tomi->Anzahl_Zeilen++;
    Tomi->Zeilen = realloc(Tomi->Zeilen, Tomi->Anzahl_Zeilen*sizeof(char **));
    Tomi->Zeilen[Tomi->Anzahl_Zeilen-1] = NULL;
    Tomi->Anzahl_Spalten = 0;
}

}

void text_maj(char *satz){
while(*satz){
    *satz =toupper(*satz);
    satz++;
}
}
void text_min(char *satz){
while(*satz){
    *satz =tolower(*satz);
    satz++;
}
}
void print_csv(csv_data *Tomi, int Grossbuchstaben, int Kleinbuchstaben, int filter){
    int *co = malloc(Tomi->Anzahl_Spalten*sizeof(int));
    int i, j;
    for (i=0; i < Tomi->Anzahl_Spalten; i++ ){
        co[i=i];
    }
    if (filter){
        for (i= 0; i<Tomi->Anzahl_Spalten-1; i++){
            for (j=i+1; j<Tomi->Anzahl_Spalten; j++){
                if(strcmp(Tomi->Kopftabelle[co[i]], Tomi->Kopftabelle[co[j]]) >0 ){
                    int temp = co[i];
                    co[i] = co[j];
                    co[j] = temp;
                }
            }
        }
    }
    printf("data:\n");
    for(i=0; i< Tomi->Anzahl_Spalten; i++){
        char *Kopf = Tomi->Kopftabelle[co[i]];
        if (Grossbuchstaben){
            text_maj(Kopf);
        }
        if (Kleinbuchstaben){
         text_min(Kopf);
        }
      printf("%s\t", Kopf);
    }
    printf("\n");
    for(i=0; i<Tomi->Anzahl_Zeilen; i++){
        for(j=0; j<Tomi->Anzahl_Spalten; j++){
            char *f = Tomi->Zeilen[i][co[j]];
             if (f){text_maj(f); }
        if (f){text_min(f); }
      printf("%s\t", f);
        }
        printf("\n");
    }
    free(co);
}


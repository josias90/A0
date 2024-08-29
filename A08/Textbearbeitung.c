
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define PIPE_NAMED "Delano"
#define BUFFER_SIZE 1024
#include <ctype.h>


void text_maj(char *satz);
void text_min(char *satz);

int main(int argc, char *argv[]){
int opt;
int f;
char buffer[BUFFER_SIZE];
int Maj=0;
int Min=0;
while((opt=getopt(argc,argv,"ul"))!=-1){
    switch(opt){
    case 'u':
        Maj=1;
        break;
    case 'l':
        Min=1;
        break;
    default:

        fprintf(stderr, "ERROR");
        return(0);

    }
    }
f = open(PIPE_NAMED, O_RDONLY);
while(read(f, buffer, BUFFER_SIZE)>0){
    if (Maj){
        text_maj(buffer);
    }else if (Min){
        text_min(buffer);
    }else{
        printf("für Großbuchstaben geben sie bitte folgenden Befehl: ./Textbearbeitung -u\n");
        printf("für Kleinbuchstaben geben sie bitte folgenden Befehl: ./Textbearbeitung -l\n");
    }
     printf("text bearbeitet %s\n", buffer);
    }
    close(f);
    return(0);
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define PIPE_NAMED "Delano"
#define BUFFER_SIZE 1024


int main()
{  char buffer[BUFFER_SIZE];
   int f;
   f = open(PIPE_NAMED, O_WRONLY);
   if (f == -1) {
    printf("erreur d'ouverture du PIPE\n");
    return(0);
   }
   printf("Schreiben Sie einen Text ein\n");
   while(fgets(buffer, BUFFER_SIZE, stdin)!=NULL){
    write(f, buffer, strlen(buffer)+1);
   }
   close(f);
   return(0);

}


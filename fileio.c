#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileio.h"

static FILE* open_file(char *nomearquivo) {
    FILE* fp = fopen(nomearquivo,"rb");
    if(!fp){
        printf("Erro: Arquivo não encontrado.\n");
        return NULL;
    } else {
        return fp;
    }
}


FILE *trata_entrada(int argc, char **argv, char *nomearquivo)
{
  //char nomearquivo[1024];
  FILE* fd = NULL;
  if (argc == 1) {
    do {
      printf("Digite o nome do arquivo: ");
      scanf("%s",nomearquivo);
      fflush(stdin);
      fd = open_file(nomearquivo);
    } while(!fd);
  } else if (argc == 2) {
    strcpy(nomearquivo,argv[1]);
    fd = open_file(nomearquivo);

  } else {
    printf("Uso do programa: ./leitorexibidor [nome-do-class]\n");
    exit(0);
  }
  return fd;
}

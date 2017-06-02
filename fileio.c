/*
 * Arquivo fileio
 *
 * Objetivo principal: Cuidar da manipulacao de arquivos.
 *
 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileio.h"

static FILE* open_classfile(char *nomearquivo) {
    FILE* fp = fopen(nomearquivo,"rb");
    if(!fp){
        printf("Erro: Arquivo não encontrado.\n");
        return NULL;
    } else {
        return fp;
    }
}

FILE* open_output_file(char *nomearquivo) {
  FILE* fp = fopen(nomearquivo,"w+");
  if (fp)
    return fp;
  else
  {
    printf("Erro na abertura do arquivo de saida!\n");
    exit(0);
  }
}

FILE *trata_entrada(int argc, char **argv, char *nomearquivo)
{
  FILE* fd = NULL;
  if (argc == 1) {
    do {
      printf("Digite o nome do arquivo: ");
      scanf("%s",nomearquivo);
      fflush(stdin);
      fd = open_classfile(nomearquivo);
    } while(!fd);
  } else if (argc == 2) {
    strcpy(nomearquivo,argv[1]);
    fd = open_classfile(nomearquivo);

  } else {
    printf("Uso do programa: ./leitorexibidor [nome-do-class]\n");
    exit(0);
  }
  return fd;
}

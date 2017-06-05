#include "leitor_exibidor.h"

u1 u1Read(FILE* fd) {
    u1 byte;
    fread(&byte, sizeof(u1), 1, fd);
    return byte;
}

u2 u2Read(FILE* fd) {
    u2 toReturn = 0;
    u1 byte1, byte2;
    fread(&byte1, sizeof(u1), 1, fd);
    fread(&byte2, sizeof(u1), 1, fd);
    toReturn = byte1 << 8;
    toReturn |= byte2;
    return toReturn;
}

u4 u4Read(FILE* fd) {
    u4 toReturn = u2Read(fd) << 16;
    toReturn |= u2Read(fd);
    return toReturn;
}

int findtype(char* type) {
    if (!strcmp(type, "ConstantValue")) {
        return CONSTANTVALUE;
    } else if (!strcmp(type, "Code")) {
        return CODE;
    } else if (!strcmp(type, "Exceptions")) {
        return EXCEPTIONS;
    } else if (!strcmp(type, "InnerClasses")) {
        return INNERCLASSES;
    } else {
        return OTHER;
    }
}

FILE* open_file(char *nomearquivo) {
    FILE* fp = fopen(nomearquivo, "rb");
    if (!fp) {
        printf("Erro: Arquivo não encontrado.\n");
        return NULL;
    } else {
        return fp;
    }
}

FILE * io_handler(int argc, char *argv[], char *nomearquivo, char *saidaarquivo)
{
  FILE *fd = NULL;
  if (argc == 1) {
    do {
      printf("Digite o nome do arquivo: ");
      scanf("%s", nomearquivo);
      fflush(stdin);
      fd = open_file(nomearquivo);
    } while (!fd);
  } else if (argc == 3) {
    strcpy(nomearquivo, argv[1]);
    fd = open_file(nomearquivo);
    if (!fd) {
      printf("Arquivo de entrada não encontrado!\n");
      exit(0);
    }
    strcpy(saidaarquivo, argv[2]);

  } else {
    printf("Uso do programa: ./leitorexibidor [nome-do-class nome-da-saída]\n");
    exit(0);
  }
  return fd;
}

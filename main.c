/*
 * Arquivo main.c 
 *
 * Objetivo principal cuidar da inicializacao e encerramento do programa.
 *
 * */

#include <stdint.h>
#include "leitor_exibidor.h"
#include "fileio.h"
#include "exibidor.h"

void shutdown(ClassFile *cf, FILE *clFile, FILE *fdout) {
  free_clFile(cf);
  fclose(clFile);
  fclose(fdout);
}

int main(int argc, char* argv[]){
    char nomearquivo[1024];
    ClassFile* cf = NULL;
    FILE *fdclFile = NULL;
    FILE *fdout = NULL;

    fdclFile = trata_entrada(argc, argv, nomearquivo);
    fdout = open_output_file("output.txt");
    cf = readClass(fdclFile);
    print_class(cf, nomearquivo, fdout);
    
    shutdown(cf, fdclFile, fdout);
    return 0;
}

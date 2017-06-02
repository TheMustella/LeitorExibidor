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

int main(int argc, char* argv[]){
    char nomearquivo[1024];
    FILE *fd = trata_entrada(argc, argv, nomearquivo);

    ClassFile* cf = readClass(fd);
    print_class(cf, nomearquivo);

    fclose(fd);
    return 0;
}

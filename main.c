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
    FILE *clFile = trata_entrada(argc, argv, nomearquivo);
    FILE *fdout = open_output_file("output.txt");

    ClassFile* cf = readClass(clFile);
    print_class(cf, nomearquivo, fdout);
    
    //shutdown();
    fclose(clFile);
    return 0;
}

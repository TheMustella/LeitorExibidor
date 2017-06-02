#include <stdint.h>
#include "leitor_exibidor.h"
#include "fileio.h"

int main(int argc, char* argv[]){
    char nomearquivo[1024];
    FILE *fd = trata_entrada(argc, argv, nomearquivo);

    ClassFile* cf = readClass(fd);
    print_class(cf, nomearquivo);

    fclose(fd);
    return 0;
}

#include "leitor_exibidor.h"
#include "exibidor.h"
#include "util.h"


int main(int argc, char* argv[]) {
    char nomearquivo[1024];
    char saidaarquivo[1024] = "output_class.txt";
    FILE* fd = NULL;
    fd = io_handler(argc, argv, nomearquivo, saidaarquivo);
    ClassFile* cf = readClass(fd);
    FILE* fout = fopen(saidaarquivo, "w");
    print_class(cf, nomearquivo, fout);
    fclose(fout);
    fclose(fd);
    printf("Finalizado!\n");
    return 0;
}

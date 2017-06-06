#include "leitor_exibidor.h"

u1 u1Read(FILE* fd);
u2 u2Read(FILE* fd);
u4 u4Read(FILE* fd);
void shutdown(FILE *fd, FILE *fout, ClassFile* cf);
int findtype(char* type);
FILE* open_file(char *nomearquivo);
FILE * io_handler(int argc, char *argv[], char *nomearquivo, FILE **fout);

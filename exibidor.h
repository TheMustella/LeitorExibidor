#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "leitor_exibidor.h"

int is_true(int code, int id);
void print_permissions(int code,FILE* fout);
void print_magic(ClassFile* cf,FILE* fout);
void print_versions(ClassFile* cf,FILE* fout);
void print_constantpool(ClassFile* cf,FILE* fout);
void print_classdata(ClassFile* cf,FILE* fout);
void print_interfaces(ClassFile* cf,FILE* fout);
void print_attribute(ClassFile* cf, attribute_info* att,FILE* fout);
void print_fields(ClassFile* cf,FILE* fout);
void print_methods(ClassFile* cf,FILE* fout);
void print_attributes(ClassFile* cf,FILE* fout);
void print_class(ClassFile* cf,char* nomearquivo,FILE* fout);
char* show_version(int code);
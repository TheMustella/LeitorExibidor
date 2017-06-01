#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#define CONSTANT_CLASS 7
#define CONSTANT_FIELDREF 9
#define CONSTANT_METHOD 10
#define CONSTANT_INTERFACE 11
#define CONSTANT_NAMEANDTYPE 12
#define CONSTANT_UTF8 1
#define CONSTANT_STRING 8
#define CONSTANT_INTEGER 3
#define CONSTANT_FLOAT 4
#define CONSTANT_LONG 5
#define CONSTANT_DOUBLE 6

// Especificando quantidades de bytes de acordo com nomes

typedef uint8_t     u1;
typedef uint16_t    u2;
typedef uint32_t    u4;

/*
 * Estrutura class_type_info: Componente da estrutura de InnerClasses_attribute.
 * Atributos:
 *
 *  inner_class_info_index (u2): Índice válido para constant_pool
 *  outer_class_info_index (u2): Zero ou Índice válido para constant_pool
 *  inner_name_index (u2): Zero ou Índice válido para constant_pool
 *  inner_class_access_flags(u2): Máscara especificando permissões de acesso e propriedades da classe ou interface declaradas
 * */

typedef struct {
    u2  inner_class_info_index;
    u2  outer_class_info_index;
    u2  inner_name_index;
    u2  inner_class_access_flags;
} classtype_info;

/*
 * Estrutura exception_table_info: Componente da estrutura de Code_attribute
 *
 * Atributos:
 *  
 *  start_pc (u2):
 *  end_pc (u2):
 *  handler_pc (u2):
 *  catch_type (u2):
   */

typedef struct {
    u2  start_pc;
    u2  end_pc;
    u2  handler_pc;
    u2  catch_type;
}   exception_table_info;

/*
 * Estrutura attribute_info: Armazena dados de atributos nas estruturas ClassFile, field_info, method_info e code_attribute;
 * 
 * Atributos:
 *
 *  attribute_name_index(u2): índice válido para constant_pool;
 *  attribute_length(u4): Tamanho, em bytes, do restante do atributo;
 *  attribute_type(union): Armazena variaveis atributo-específicas.
 * */

typedef struct _attribute_info {
    u2  attribute_name_index;
    u4  attribute_length;
    union {
        struct {
            u2 constantvalue_index;
        }   ConstantValue_attribute;
        struct {
            u2                      max_stack;
            u2                      max_locals;
            u4                      code_length;
            u1*                     code;
            u2                      exception_table_length;
            exception_table_info*   exception_table;
            u2  attrubutes_count;
            struct _attribute_info* attributes;
        }   Code_attribute;
        struct {
            u2  number_of_exceptions;
            u2* exception_index_table;
        }   Exceptions_attribute;
        struct {
            u2              number_of_classes;
            classtype_info* classes;
        }   InnerClasses_attribute;
    }   attribute_type;
}   attribute_info;

/*
 * Estrutura field_info e method_info: Campos e métodos da classe
 *
 * Atributos:
 *
 *  access_flags (u2): Máscara que especifica permissões de acesso e propriedades do campo;
 *  name_index (u2): indice válido para constant_pool;
 *  descriptor_index (u2): indice válido para constant_pool;
 *  attribute_count (u2): Número de atributos do campo;
 *  attributes (sizeof attribute_info*): tabela de atributos do campo.
 * */

typedef struct {
    u2              access_flags;
    u2              name_index;
    u2              descriptor_index;
    u2              attributes_count;
    attribute_info* attributes;
}   field_info, method_info;

/*
 * Estrutura cp_info: Armazenam informações simbólicas;
 *
 * Atributos:
 *
 *  tag (u1): Define o tipo de informação;
 *  info (union): Armazena informação.
 *
 * */

typedef struct {
    u1  tag;
    union {
        struct {
            u2  name_index;
        }   CONSTANT_Class_info;
        struct {
            u2  class_index;
            u2  name_and_type_index;
        }   CONSTANT_Fieldref_info, CONSTANT_Method_info,   CONSTANT_Interface_info;
        struct {
            u2  name_index;
            u2  descriptor_index;
        }   CONSTANT_NameAndType_info;
        struct {
            u2  length;
            u1* bytes;
        }   CONSTANT_Utf8_info;
        struct {
            u2  string_index;
        }   CONSTANT_String_info;
        struct {
            u4  bytes;
        }   CONSTANT_Integer_info;
        struct {
            u4  bytes;
        }   CONSTANT_Float_info;
        struct {
            u4  high_bytes;
            u4  low_bytes;
        }   CONSTANT_Long_info, CONSTANT_Double_info;
    }   info;
}   cp_info;


/*
 * Estrutura ClassFile: Descrição de um arquivo .class
 *
 * Atributos:
 *
 *  magic (u4): assinatura do arquivo .class
 *  minor_version (u2) e major_version (u2): indicam versão do formato na forma major.minor
 *  constant_pool_count (u2): Número de entradas na tabela constant_pool+1
 *  constant_pool: tabela de estruturas
 *  access_flags: Máscara de bits que especifica permissões de acesso e propriedades da classe ou interface
 *  this_class: Representa classe ou interface definida pelo arquivo
 *  super_class: Representa super classe direta da classe definida
 *  interfaces_count: Número de entradas no array interfaces[]
 *  interfaces: Array de interfaces
 *  fields_count: Número de variaveis da classe ou de intancias declaradas nesta classe ou interfaces
 *  fields: tabela de fields
 *  method_count: Número de métodos na tabela
 *  methods: tabela métodos
 *  attributes_count: Número de estruturas na tabela attributes
 *  attributes: Tabela de estruturas do tipo attribute_info
 *
 * */
typedef struct {
    u4              magic;
    u2              minor_version;
    u2              major_version;
    u2              constant_pool_count;
    cp_info*        constant_pool;
    u2              access_flags;
    u2              this_class;
    u2              super_class;
    u2              interfaces_count;
    u2*             interfaces;
    u2              fields_count;
    field_info*     fields;
    u2              method_count;
    method_info*    methods;
    u2              attributes_count;
    attribute_info  attributes;
}   ClassFile;

u1 u1Read(FILE* fd) {
    u1 byte;
    fread(&byte,sizeof(u1),1,fd);
    return byte;
}

u2 u2Read(FILE* fd) {
    u2 toReturn = 0;
    u1 byte1,byte2;
    fread(&byte1,sizeof(u1),1,fd);
    fread(&byte2,sizeof(u1),1,fd);
    toReturn = byte1<<8;
    toReturn |= byte2;
    return toReturn;
}

u4 u4Read(FILE* fd) {
    u4 toReturn = u2Read(fd)<<16;
    toReturn |= u2Read(fd);
    return toReturn;
}

FILE* open_file(char *nomearquivo) {
    FILE* fp = fopen(nomearquivo,"rb");
    if(!fp){
        printf("Erro: Arquivo não encontrado.\n");
        return NULL;
    } else {
        return fp;
    }
}

ClassFile* readClass(FILE* fd){
    ClassFile* cf = (ClassFile*) malloc(sizeof(ClassFile));
    cf->magic = u4Read(fd);
    cf->minor_version = u2Read(fd);
    cf->major_version = u2Read(fd);
    cf->constant_pool_count = u2Read(fd);
    cf->constant_pool = (cp_info*) malloc(sizeof(cp_info)*(cf->constant_pool_count-1));
    cp_info *cp;
    for(cp = cf->constant_pool;cp<cf->constant_pool+cf->constant_pool_count-1;++cp) {
        cp->tag = u1Read(fd);
        switch(cp->tag) {
            case CONSTANT_CLASS:
                cp->info.CONSTANT_Class_info.name_index = u2Read(fd);
            break;
            case CONSTANT_FIELDREF:
                cp->info.CONSTANT_Fieldref_info.class_index = u2Read(fd);
                cp->info.CONSTANT_Fieldref_info.name_and_type_index = u2Read(fd);
            break;
            case CONSTANT_METHOD:
                cp->info.CONSTANT_Method_info.class_index = u2Read(fd);
                cp->info.CONSTANT_Method_info.name_and_type_index = u2Read(fd);
            break;
            case CONSTANT_INTERFACE:
                cp->info.CONSTANT_Interface_info.class_index = u2Read(fd);
                cp->info.CONSTANT_Interface_info.name_and_type_index = u2Read(fd);
            break;
            case CONSTANT_NAMEANDTYPE:
                cp->info.CONSTANT_NameAndType_info.name_index = u2Read(fd);
                cp->info.CONSTANT_NameAndType_info.descriptor_index = u2Read(fd);
            break;
            case CONSTANT_UTF8:
                cp->info.CONSTANT_Utf8_info.length = u2Read(fd);
                cp->info.CONSTANT_Utf8_info.bytes = (u1*)malloc(sizeof(u1)*cp->info.CONSTANT_Utf8_info.length);
                u1* b;
                for(b=cp->info.CONSTANT_Utf8_info.bytes;b<cp->info.CONSTANT_Utf8_info.bytes+cp->info.CONSTANT_Utf8_info.length,++b) {
                    *b = u1Read(fd);
                }
            break;
            case CONSTANT_STRING:
                cp->info.CONSTANT_String_info.string_index = u2Read(fd);
            break;
            case CONSTANT_INTEGER:
                cp->info.CONSTANT_Integer_info.bytes = u4Read(fd);
            break;
            case CONSTANT_FLOAT:
                cp->info.CONSTANT_Float_info.bytes = u4Read(fd);
            break;
            case CONSTANT_LONG:
                cp->info.CONSTANT_Long_info.high_bytes = u4Read(fd);
                cp->info.CONSTANT_Long_info.low_bytes = u4Read(fd);
            break;
            case CONSTANT_DOUBLE:
                cp->info.CONSTANT_Double_info.high_bytes = u4Read(fd);
                cp->info.CONSTANT_Double_info.low_bytes = u4Read(fd);
            break;
        }
    }
    cf->access_flags = u2Read(fd);
    cf->this_class = u2Read(fd);
    cf->super_class = u2Read(fd);
    cf->interfaces_count = u2Read(fd);
    cf->interfaces = (u2*) malloc(sizeof(u2)*cf->interfaces_count);
    u2* bytes;
    for(bytes = cf->interfaces;bytes<cf->interfaces+cf->interfaces_count;++bytes) {
        *bytes = u2Read(fd);
    }
    cf->fields_count = u2Read(fd);
    
    return cf;
}

int main(int argc, char* argv[]){
    char nomearquivo[1024];
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

    } else {
        printf("Uso do programa: ./leitorexibidor [nome-do-class]\n");
        return 0;
    }
    ClassFile* cf = readClass(fd);
    fclose(fd);
    return 0;
}

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

int main(){

    return 0;
}
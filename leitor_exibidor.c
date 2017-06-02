#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "leitor_exibidor.h"
//#include "fileio.h"

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

void load_magic(ClassFile* cf,FILE* fd) {
    cf->magic = u4Read(fd);
}

void load_versions(ClassFile* cf,FILE* fd) {
    cf->minor_version = u2Read(fd);
    cf->major_version = u2Read(fd);
}

void load_constantpool(ClassFile* cf,FILE* fd) {
    cf->constant_pool_count = u2Read(fd);
    cf->constant_pool = (cp_info*) malloc(sizeof(cp_info)*(cf->constant_pool_count-1));
    cp_info *cp;
    for(cp = cf->constant_pool;cp<cf->constant_pool+cf->constant_pool_count-1;++cp) {
        cp->tag = u1Read(fd);
        switch(cp->tag) {
            case CLASS:
                cp->info.Class_info.name_index = u2Read(fd);
            break;
            case FIELDREF:
                cp->info.Fieldref_info.class_index = u2Read(fd);
                cp->info.Fieldref_info.name_and_type_index = u2Read(fd);
            break;
            case METHOD:
                cp->info.Method_info.class_index = u2Read(fd);
                cp->info.Method_info.name_and_type_index = u2Read(fd);
            break;
            case INTERFACE:
                cp->info.Interface_info.class_index = u2Read(fd);
                cp->info.Interface_info.name_and_type_index = u2Read(fd);
            break;
            case NAMEANDTYPE:
                cp->info.NameAndType_info.name_index = u2Read(fd);
                cp->info.NameAndType_info.descriptor_index = u2Read(fd);
            break;
            case UTF8:
                cp->info.Utf8_info.length = u2Read(fd);
                cp->info.Utf8_info.bytes = (u1*)malloc(sizeof(u1)*cp->info.Utf8_info.length);
                u1* b;
                for(b=cp->info.Utf8_info.bytes;b<cp->info.Utf8_info.bytes+cp->info.Utf8_info.length;++b) {
                    *b = u1Read(fd);
                }
            break;
            case STRING:
                cp->info.String_info.string_index = u2Read(fd);
            break;
            case INTEGER:
                cp->info.Integer_info.bytes = u4Read(fd);
            break;
            case FLOAT:
                cp->info.Float_info.bytes = u4Read(fd);
            break;
            case LONG:
                cp->info.Long_info.high_bytes = u4Read(fd);
                cp->info.Long_info.low_bytes = u4Read(fd);
            break;
            case DOUBLE:
                cp->info.Double_info.high_bytes = u4Read(fd);
                cp->info.Double_info.low_bytes = u4Read(fd);
            break;
        }
    }
}

void load_classdata(ClassFile* cf,FILE* fd) {
    cf->access_flags = u2Read(fd);
    cf->this_class = u2Read(fd);
    cf->super_class = u2Read(fd);
}

void load_interfaces(ClassFile* cf,FILE* fd) {
    cf->interfaces_count = u2Read(fd);
    cf->interfaces = (u2*) malloc(sizeof(u2)*cf->interfaces_count);
    u2* bytes;
    for(bytes = cf->interfaces;bytes<cf->interfaces+cf->interfaces_count;++bytes) {
        *bytes = u2Read(fd);
    }
}

int findtype(char* type) {
    if(!strcmp(type,"ConstantValue")) {
        return CONSTANTVALUE;
    } else if(!strcmp(type,"Code")) {
        return CODE;
    } else if(!strcmp(type,"Exceptions")) {
        return EXCEPTIONS;
    } else if(!strcmp(type,"InnerClasses")) {
        return INNERCLASSES;
    } else {
        return OTHER;
    }
}

void load_constantvalue_attr(attribute_info* att, FILE* fd) {
    att->type.ConstantValue.constantvalue_index = u2Read(fd);
}

void load_code_attr(attribute_info* att, ClassFile* cf,FILE* fd) {
    att->type.Code.max_stack = u2Read(fd);
    att->type.Code.max_locals = u2Read(fd);
    att->type.Code.code_length = u4Read(fd);
    att->type.Code.code = (u1*)malloc(sizeof(u1)*att->type.Code.code_length);
    u1* byte;
    for(byte=att->type.Code.code;byte<att->type.Code.code+att->type.Code.code_length;++byte) {
        *byte = u2Read(fd);
    }
    att->type.Code.exception_table_length = u2Read(fd);
    att->type.Code.exception_table = (exception_table_info*)malloc(sizeof(exception_table_info)*att->type.Code.exception_table_length);
    exception_table_info* exp_aux;
    for(exp_aux = att->type.Code.exception_table;exp_aux<att->type.Code.exception_table+att->type.Code.exception_table_length;++exp_aux) {
        exp_aux->start_pc = u2Read(fd);
        exp_aux->end_pc = u2Read(fd);
        exp_aux->handler_pc = u2Read(fd);
        exp_aux->catch_type = u2Read(fd);
    }
    att->type.Code.attributes_count = u2Read(fd);
    att->type.Code.attributes = (attribute_info*)malloc(sizeof(attribute_info)*att->type.Code.attributes_count);
    attribute_info* att_aux;
    for(att_aux=att->type.Code.attributes;att_aux<att->type.Code.attributes+att->type.Code.attributes_count;++att_aux) {
        load_attribute(att_aux,cf,fd);
    }
}

void load_exceptions_attr(attribute_info* att,FILE* fd) {
    att->type.Exceptions.number_of_exceptions = u2Read(fd);
    att->type.Exceptions.exception_index_table = (u2*) malloc(sizeof(u2)*att->type.Exceptions.number_of_exceptions);
    u2* bytes;
    for(bytes = att->type.Exceptions.exception_index_table; bytes<att->type.Exceptions.exception_index_table+att->type.Exceptions.number_of_exceptions;++bytes) {
        *bytes = u2Read(fd);
    }
}

void load_innerclasses_attr(attribute_info* att,FILE* fd) {
    att->type.InnerClasses.number_of_classes = u2Read(fd);
    att->type.InnerClasses.classes = (classtype_info*) malloc(sizeof(classtype_info)*att->type.InnerClasses.number_of_classes);
    classtype_info* classtype_aux;
    for(classtype_aux = att->type.InnerClasses.classes;classtype_aux<att->type.InnerClasses.classes+att->type.InnerClasses.number_of_classes;++classtype_aux) {
        classtype_aux->inner_class_info_index = u2Read(fd);
        classtype_aux->outer_class_info_index = u2Read(fd);
        classtype_aux->inner_name_index = u2Read(fd);
        classtype_aux->inner_class_access_flags = u2Read(fd);
    }
}

void load_other_attr(attribute_info* att, FILE* fd) {
    att->type.Other.bytes = (u1*) malloc(sizeof(u1)*att->attribute_length);
    u1* bytes;
    for(bytes = att->type.Other.bytes;bytes<att->type.Other.bytes+att->attribute_length;++bytes) {
        *bytes = u1Read(fd);
    }
}

void load_attribute(attribute_info* att,ClassFile* cf,FILE* fd) {
    char* type;
    att->attribute_name_index = u2Read(fd);
    att->attribute_length = u4Read(fd);
    type = (char*)malloc(sizeof(char)*cf->constant_pool[att->attribute_name_index-1].info.Utf8_info.length);
    strcpy(type,(char*)cf->constant_pool[att->attribute_name_index-1].info.Utf8_info.bytes);
    int i=findtype(type);
    switch(i) {
        case CONSTANTVALUE:
            load_constantvalue_attr(att,fd);
        break;
        case CODE:
            load_code_attr(att,cf,fd);
        break;
        case EXCEPTIONS:
            load_exceptions_attr(att,fd);
        break;
        case INNERCLASSES:
            load_innerclasses_attr(att,fd);
        break;
        case OTHER:
            load_other_attr(att,fd);
        break;
    }
}

void load_fields(ClassFile* cf,FILE* fd) {
    cf->fields_count = u2Read(fd);
    cf->fields = (field_info*)malloc(sizeof(field_info)*cf->fields_count);
    field_info* field_aux;
    for(field_aux = cf->fields;field_aux<cf->fields+cf->fields_count;++field_aux) {
        field_aux->access_flags = u2Read(fd);
        field_aux->name_index = u2Read(fd);
        field_aux->descriptor_index = u2Read(fd);
        field_aux->attributes_count = u2Read(fd);
        field_aux->attributes = malloc(sizeof(attribute_info)*field_aux->attributes_count);
        attribute_info* attribute_aux;
        for(attribute_aux=field_aux->attributes;attribute_aux<field_aux->attributes+field_aux->attributes_count;++attribute_aux) {
            load_attribute(attribute_aux,cf,fd);
        }
    }
}

void load_methods(ClassFile* cf,FILE* fd) {
    cf->method_count = u2Read(fd);
    cf->methods = (method_info*) malloc(sizeof(method_info)*cf->method_count);
    method_info* method_aux;
    for(method_aux = cf->methods;method_aux<cf->methods+cf->method_count;++method_aux) {
        method_aux->access_flags = u2Read(fd);
        method_aux->name_index = u2Read(fd);
        method_aux->descriptor_index = u2Read(fd);
        method_aux->attributes_count = u2Read(fd);
        method_aux->attributes = (attribute_info*) malloc(sizeof(attribute_info)*method_aux->attributes_count);
        attribute_info* att_aux;
        for(att_aux = method_aux->attributes;att_aux<method_aux->attributes+method_aux->attributes_count;++att_aux) {
            load_attribute(att_aux,cf,fd);
        }
    }
}

void load_attributes(ClassFile* cf, FILE* fd) {
    cf->attributes_count = u2Read(fd);
    cf->attributes = (attribute_info*) malloc(sizeof(attribute_info)*cf->attributes_count);
    attribute_info* att_aux;
    for(att_aux = cf->attributes;att_aux<cf->attributes+cf->attributes_count;++att_aux) {
        load_attribute(att_aux,cf,fd);
    }
}

ClassFile* readClass(FILE* fd) {
    ClassFile* cf = (ClassFile*) malloc(sizeof(ClassFile));
    load_magic(cf,fd);
    load_versions(cf,fd);
    load_constantpool(cf,fd);
    load_classdata(cf,fd);
    // load_interfaces(cf,fd);
    // load_fields(cf,fd);
    // load_methods(cf,fd);
    // load_attributes(cf,fd);
    return cf;
}

void print_magic(ClassFile* cf) {
    printf("MAGIC: %x\n",cf->magic);
}

void print_versions(ClassFile* cf) {
    printf("VERSION:\n");
    printf("\tMINOR: %d\n",cf->minor_version);
    printf("\tMAJOR: %d\n\n",cf->major_version);
}

void print_constantpool(ClassFile* cf) {
    long long Long;
    printf("CONSTANT POOL COUNT: %d\n",cf->constant_pool_count);
    printf("CONSTANT_POOL:\n");
    cp_info* cp;
    for(cp = cf->constant_pool;cp<cf->constant_pool+cf->constant_pool_count-1;++cp) {
        switch(cp->tag) {
            case CLASS:
                printf("\tCP_INFO: CLASS\n");
                printf("\tNAME_INDEX: %d\n\n",cp->info.Class_info.name_index);
            break;
            case FIELDREF:
                printf("\tCP_INFO: FIELDREF\n");
                printf("\tCLASS_INDEX: %d\n",cp->info.Fieldref_info.class_index);
                printf("\tNAMEANDTYPE_INDEX: %d\n\n",cp->info.Fieldref_info.name_and_type_index);
            break;
            case METHOD:
                printf("\tCP_INFO: METHOD\n");
                printf("\tCLASS_INDEX: %d\n",cp->info.Method_info.class_index);
                printf("\tNAMEANDTYPE_INDEX: %d\n\n",cp->info.Method_info.name_and_type_index);
            break;
            case INTERFACE:
                printf("\tCP_INFO: INTERFACE\n");
                printf("\tCLASS_INDEX: %d\n",cp->info.Interface_info.class_index);
                printf("\tNAMEANDTYPE_INDEX: %d\n\n",cp->info.Interface_info.name_and_type_index);
            break;
            case NAMEANDTYPE:
                printf("\tCP_INFO: NAMEANDTYPE\n");
                printf("\tNAME_INDEX: %d\n",cp->info.NameAndType_info.name_index);
                printf("\tDESCRIPTOR_INDEX: %d\n\n",cp->info.NameAndType_info.descriptor_index);
            break;
            case UTF8:
                printf("\tCP_INFO: UTF8\n");
                printf("\tLENGTH: %d\n",cp->info.Utf8_info.length);
                printf("\tVALUE: %s\n\n",(char*)cp->info.Utf8_info.bytes);
            break;
            case STRING:
                printf("\tCP_INFO: STRING\n");
                printf("\tSTRING_INDEX: %d\n\n",cp->info.String_info.string_index);
            break;
            case INTEGER:
                printf("\tCP_INFO: INTEGER\n");
                printf("\tBYTES: %x\n",cp->info.Integer_info.bytes);
                printf("\tVALUE: %d\n\n",cp->info.Integer_info.bytes);
            break;
            case FLOAT:
                printf("\tCP_INFO: FLOAT\n");
                printf("\tBYTES: %x\n",cp->info.Float_info.bytes);
                u4tofloat.U4 = cp->info.Float_info.bytes;
                printf("\tVALUE: %f\n\n",u4tofloat.Float);
            break;
            case LONG:
                printf("\tCP_INFO: LONG\n");
                printf("\tHIGH: %x\n",cp->info.Long_info.high_bytes);
                printf("\tLOW: %x\n", cp->info.Long_info.low_bytes);
                Long = ((long long) cp->info.Long_info.high_bytes << 32) | (cp->info.Long_info.low_bytes);
                printf("\tVALUE: %lld\n\n",Long);
            break;
            case DOUBLE:
                printf("\tCP_INFO: DOUBLE\n");
                printf("\tHIGH: %x\n",cp->info.Double_info.high_bytes);
                printf("\tLOW: %x\n", cp->info.Double_info.low_bytes);
                Long = ((long long) cp->info.Double_info.high_bytes << 32) | (cp->info.Double_info.low_bytes);
                printf("\tVALUE: %lld\n\n",Long);
            break;
        }
    }

}

void print_classdata(ClassFile* cf) {
    printf("ACCESS_FLAGS: %x\n",cf->access_flags);
    printf("THIS_CLASS: %d\n",cf->this_class);
    printf("SUPER_CLASS: %d\n\n",cf->super_class);
}

void print_interfaces(ClassFile* cf) {
    printf("INTERFACES_COUNT: %d\n",cf->interfaces_count);
    printf("INTERFACES:\n");
    u2* interface_aux;
    for(interface_aux = cf->interfaces;interface_aux<cf->interfaces+cf->interfaces_count;++interface_aux) {
        printf("\tINTERFACE: %d\n",*interface_aux);
    }
}



void print_class(ClassFile* cf,char* nomearquivo) {
    printf("Nome do .class: %s\n",nomearquivo);
    print_magic(cf);
    print_versions(cf);
    print_constantpool(cf);
    print_classdata(cf);
    // print_interfaces(cf);
    // print_fields(cf);
    // print_methods(cf);
    // print_attributes(cf);
}

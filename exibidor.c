/*
 * Arquivo exibidor.c
 *
 * Objetivo principal: Exibicao do arquivo .class lido pelo leitor.c
 *
 * */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "leitor_exibidor.h"

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

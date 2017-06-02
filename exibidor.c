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

static void print_magic(ClassFile* cf, FILE* fdout) {
    //printf("MAGIC: %x\n",cf->magic);
    fprintf(fdout, "MAGIC: %x\n",cf->magic);
}

static void print_versions(ClassFile* cf, FILE* fdout) {
    //printf("VERSION:\n");
    //printf("\tMINOR: %d\n",cf->minor_version);
    //printf("\tMAJOR: %d\n\n",cf->major_version);
    fprintf(fdout, "VERSION:\n");
    fprintf(fdout, "\tMINOR: %d\n",cf->minor_version);
    fprintf(fdout, "\tMAJOR: %d\n\n",cf->major_version);
}

static void print_constantpool(ClassFile* cf, FILE* fdout) {
    long long Long;
    //printf("CONSTANT POOL COUNT: %d\n",cf->constant_pool_count);
    //printf("CONSTANT_POOL:\n");
    fprintf(fdout, "CONSTANT POOL COUNT: %d\n",cf->constant_pool_count);
    fprintf(fdout, "CONSTANT_POOL:\n");
    cp_info* cp;
    for(cp = cf->constant_pool;cp<cf->constant_pool+cf->constant_pool_count-1;++cp) {
        switch(cp->tag) {
            case CLASS:
                printf("\tCP_INFO: CLASS\n");
                printf("\tNAME_INDEX: %d\n\n",cp->info.Class_info.name_index);
                fprintf(fdout, "\tCP_INFO: CLASS\n");
                fprintf(fdout, "\tNAME_INDEX: %d\n\n",cp->info.Class_info.name_index);
            break;
            case FIELDREF:
                printf("\tCP_INFO: FIELDREF\n");
                printf("\tCLASS_INDEX: %d\n",cp->info.Fieldref_info.class_index);
                printf("\tNAMEANDTYPE_INDEX: %d\n\n",cp->info.Fieldref_info.name_and_type_index);
                fprintf(fdout, "\tCP_INFO: FIELDREF\n");
                fprintf(fdout, "\tCLASS_INDEX: %d\n",cp->info.Fieldref_info.class_index);
                fprintf(fdout, "\tNAMEANDTYPE_INDEX: %d\n\n",cp->info.Fieldref_info.name_and_type_index);
            break;
            case METHOD:
                printf("\tCP_INFO: METHOD\n");
                printf("\tCLASS_INDEX: %d\n",cp->info.Method_info.class_index);
                printf("\tNAMEANDTYPE_INDEX: %d\n\n",cp->info.Method_info.name_and_type_index);
                fprintf(fdout, "\tCP_INFO: METHOD\n");
                fprintf(fdout, "\tCLASS_INDEX: %d\n",cp->info.Method_info.class_index);
                fprintf(fdout, "\tNAMEANDTYPE_INDEX: %d\n\n",cp->info.Method_info.name_and_type_index);
            break;
            case INTERFACE:
                printf("\tCP_INFO: INTERFACE\n");
                printf("\tCLASS_INDEX: %d\n",cp->info.Interface_info.class_index);
                printf("\tNAMEANDTYPE_INDEX: %d\n\n",cp->info.Interface_info.name_and_type_index);
                fprintf(fdout, "\tCP_INFO: INTERFACE\n");
                fprintf(fdout, "\tCLASS_INDEX: %d\n",cp->info.Interface_info.class_index);
                fprintf(fdout, "\tNAMEANDTYPE_INDEX: %d\n\n",cp->info.Interface_info.name_and_type_index);
            break;
            case NAMEANDTYPE:
                printf("\tCP_INFO: NAMEANDTYPE\n");
                printf("\tNAME_INDEX: %d\n",cp->info.NameAndType_info.name_index);
                printf("\tDESCRIPTOR_INDEX: %d\n\n",cp->info.NameAndType_info.descriptor_index);
                fprintf(fdout, "\tCP_INFO: NAMEANDTYPE\n");
                fprintf(fdout, "\tNAME_INDEX: %d\n",cp->info.NameAndType_info.name_index);
                fprintf(fdout, "\tDESCRIPTOR_INDEX: %d\n\n",cp->info.NameAndType_info.descriptor_index);
            break;
            case UTF8:
                printf("\tCP_INFO: UTF8\n");
                printf("\tLENGTH: %d\n",cp->info.Utf8_info.length);
                printf("\tVALUE: %s\n\n",(char*)cp->info.Utf8_info.bytes);
                fprintf(fdout, "\tCP_INFO: UTF8\n");
                fprintf(fdout, "\tLENGTH: %d\n",cp->info.Utf8_info.length);
                fprintf(fdout, "\tVALUE: %s\n\n",(char*)cp->info.Utf8_info.bytes);
            break;
            case STRING:
                printf("\tCP_INFO: STRING\n");
                printf("\tSTRING_INDEX: %d\n\n",cp->info.String_info.string_index);
                fprintf(fdout, "\tCP_INFO: STRING\n");
                fprintf(fdout, "\tSTRING_INDEX: %d\n\n",cp->info.String_info.string_index);
            break;
            case INTEGER:
                printf("\tCP_INFO: INTEGER\n");
                printf("\tBYTES: %x\n",cp->info.Integer_info.bytes);
                printf("\tVALUE: %d\n\n",cp->info.Integer_info.bytes);
                fprintf(fdout, "\tCP_INFO: INTEGER\n");
                fprintf(fdout, "\tBYTES: %x\n",cp->info.Integer_info.bytes);
                fprintf(fdout, "\tVALUE: %d\n\n",cp->info.Integer_info.bytes);
            break;
            case FLOAT:
                printf("\tCP_INFO: FLOAT\n");
                printf("\tBYTES: %x\n",cp->info.Float_info.bytes);
                fprintf(fdout, "\tCP_INFO: FLOAT\n");
                fprintf(fdout, "\tBYTES: %x\n",cp->info.Float_info.bytes);
                u4tofloat.U4 = cp->info.Float_info.bytes;
                printf("\tVALUE: %f\n\n",u4tofloat.Float);
                fprintf(fdout, "\tVALUE: %f\n\n",u4tofloat.Float);
            break;
            case LONG:
                printf("\tCP_INFO: LONG\n");
                printf("\tHIGH: %x\n",cp->info.Long_info.high_bytes);
                printf("\tLOW: %x\n", cp->info.Long_info.low_bytes);
                fprintf(fdout, "\tCP_INFO: LONG\n");
                fprintf(fdout, "\tHIGH: %x\n",cp->info.Long_info.high_bytes);
                fprintf(fdout, "\tLOW: %x\n", cp->info.Long_info.low_bytes);
                Long = ((long long) cp->info.Long_info.high_bytes << 32) | (cp->info.Long_info.low_bytes);
                printf("\tVALUE: %lld\n\n",Long);
                fprintf(fdout, "\tVALUE: %lld\n\n",Long);
            break;
            case DOUBLE:
                printf("\tCP_INFO: DOUBLE\n");
                printf("\tHIGH: %x\n",cp->info.Double_info.high_bytes);
                printf("\tLOW: %x\n", cp->info.Double_info.low_bytes);
                fprintf(fdout, "\tCP_INFO: DOUBLE\n");
                fprintf(fdout, "\tHIGH: %x\n",cp->info.Double_info.high_bytes);
                fprintf(fdout, "\tLOW: %x\n", cp->info.Double_info.low_bytes);
                Long = ((long long) cp->info.Double_info.high_bytes << 32) | (cp->info.Double_info.low_bytes);
                printf("\tVALUE: %lld\n\n",Long);
                fprintf(fdout, "\tVALUE: %lld\n\n",Long);
            break;
        }
    }

}

static void print_classdata(ClassFile* cf, FILE* fdout) {
    printf("ACCESS_FLAGS: %x\n",cf->access_flags);
    printf("THIS_CLASS: %d\n",cf->this_class);
    printf("SUPER_CLASS: %d\n\n",cf->super_class);
    fprintf(fdout, "ACCESS_FLAGS: %x\n",cf->access_flags);
    fprintf(fdout, "THIS_CLASS: %d\n",cf->this_class);
    fprintf(fdout, "SUPER_CLASS: %d\n\n",cf->super_class);
}

static void print_interfaces(ClassFile* cf, FILE *fdout) {
    printf("INTERFACES_COUNT: %d\n",cf->interfaces_count);
    printf("INTERFACES:\n");
    fprintf(fdout, "INTERFACES_COUNT: %d\n",cf->interfaces_count);
    fprintf(fdout, "INTERFACES:\n");
    u2* interface_aux;
    for(interface_aux = cf->interfaces;interface_aux<cf->interfaces+cf->interfaces_count;++interface_aux) {
        printf("\tINTERFACE: %d\n",*interface_aux);
        fprintf(fdout, "\tINTERFACE: %d\n",*interface_aux);
    }
}



void print_class(ClassFile* cf,char* nomearquivo, FILE* fdout) {
    printf("Nome do .class: %s\n",nomearquivo);
    print_magic(cf, fdout);
    print_versions(cf, fdout);
    print_constantpool(cf, fdout);
    print_classdata(cf, fdout);
    // print_interfaces(cf, fdout);
    // print_fields(cf, fdout);
    // print_methods(cf, fdout);
    // print_attributes(cf, fdout);
}

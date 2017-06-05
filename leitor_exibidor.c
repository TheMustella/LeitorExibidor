#include "leitor_exibidor.h"

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
    /*if (cf->magic != 0xcafebabe)
		EXIT; //arquivo não eh .class
    */
}



void load_versions(ClassFile* cf,FILE* fd) {
    cf->minor_version = u2Read(fd);
    cf->major_version = u2Read(fd);
}

void load_constantpool(ClassFile* cf,FILE* fd) {
    cf->constant_pool_count = u2Read(fd);
    if(cf->constant_pool_count <=1) {
        cf->constant_pool = NULL;
        return;
    }
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
                cp->info.Utf8_info.bytes = (u1*)malloc(sizeof(u1)*cp->info.Utf8_info.length); //length diz o numero de bytes UTF8 desse cp_info
                u1* b;
                for(b=cp->info.Utf8_info.bytes ; b < cp->info.Utf8_info.bytes + cp->info.Utf8_info.length ; ++b) { //laco para leitura desses bytes
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
                cp->info.Long_info.high_bytes = u4Read(fd); //estrutura de 64 bytes
                cp->info.Long_info.low_bytes = u4Read(fd);
            break;
            case DOUBLE:
                cp->info.Double_info.high_bytes = u4Read(fd); //estrutura de 64 bytes
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
    if(cf->interfaces_count<=0) {
        cf->interfaces = NULL;
        return;
    }
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
    if(att->type.Code.code_length <= 0) {
        att->type.Code.code = NULL;
    } else {
        att->type.Code.code = (u1*)malloc(sizeof(u1)*att->type.Code.code_length);
        u1* byte;
        for(byte=att->type.Code.code;byte<att->type.Code.code+att->type.Code.code_length;++byte) {
            *byte = u1Read(fd);
        }
    }
    att->type.Code.exception_table_length = u2Read(fd);
    if(att->type.Code.exception_table_length <= 0) {
        att->type.Code.exception_table = NULL;
    } else {
        att->type.Code.exception_table = (exception_table_info*)malloc(sizeof(exception_table_info)*att->type.Code.exception_table_length);
        exception_table_info* exp_aux;
        for(exp_aux = att->type.Code.exception_table;exp_aux<att->type.Code.exception_table+att->type.Code.exception_table_length;++exp_aux) {
            exp_aux->start_pc = u2Read(fd);
            exp_aux->end_pc = u2Read(fd);
            exp_aux->handler_pc = u2Read(fd);
            exp_aux->catch_type = u2Read(fd);
        }
    }
    att->type.Code.attributes_count = u2Read(fd);
    if(att->type.Code.attributes_count <= 0) {
        att->type.Code.attributes = NULL;    
    } else {
        att->type.Code.attributes = (attribute_info*)malloc(sizeof(attribute_info)*att->type.Code.attributes_count);
        attribute_info* att_aux;
        for(att_aux=att->type.Code.attributes;att_aux<att->type.Code.attributes+att->type.Code.attributes_count;++att_aux) {
            load_attribute(att_aux,cf,fd);
        }
    }
}

void load_exceptions_attr(attribute_info* att,FILE* fd) {
    att->type.Exceptions.number_of_exceptions = u2Read(fd);
    if(att->type.Exceptions.number_of_exceptions <= 0) {
        att->type.Exceptions.exception_index_table = NULL;    
    } else {
        att->type.Exceptions.exception_index_table = (u2*) malloc(sizeof(u2)*att->type.Exceptions.number_of_exceptions);
        u2* bytes;
        for(bytes = att->type.Exceptions.exception_index_table; bytes<att->type.Exceptions.exception_index_table+att->type.Exceptions.number_of_exceptions;++bytes) {
            *bytes = u2Read(fd);
        }
    }
}

void load_innerclasses_attr(attribute_info* att,FILE* fd) {
    att->type.InnerClasses.number_of_classes = u2Read(fd);
    if(att->type.InnerClasses.number_of_classes <=0) {
        att->type.InnerClasses.classes = NULL;
        return;
    }
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
    if(att->attribute_length <= 0) {
        att->type.Other.bytes = NULL;
        return;
    }
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
    if(cf->fields_count <= 0) {
        cf->fields = NULL;
        return;
    }
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
    if(cf->method_count <= 0) {
        cf->methods = NULL;
        return;
    }
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
    if(cf->attributes_count <= 0) {
        cf->attributes = NULL;
        return;
    }
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
    load_interfaces(cf,fd);
    load_fields(cf,fd);
    load_methods(cf,fd);
    load_attributes(cf,fd);
    return cf;
}

int is_true(int code, int id) {
    return code & (1<<id);
}

void print_permissions(int code,FILE* fout) {
    if(is_true(code,0)) fprintf(fout,"public ");
    else if(is_true(code,1)) fprintf(fout,"private ");
    else if(is_true(code,2)) fprintf(fout,"protected ");

    if (is_true(code,3)) fprintf(fout,"static ");
    if (is_true(code,4)) fprintf(fout,"final ");
    if (is_true(code,5)) fprintf(fout,"super ");
    if (is_true(code,6)) fprintf(fout,"volatile ");
    if (is_true(code,7)) fprintf(fout,"transient ");
    if (is_true(code,8)) fprintf(fout,"native ");
    if (is_true(code,9)) fprintf(fout,"interface ");
    if (is_true(code,10)) fprintf(fout,"abstract ");
}

void print_magic(ClassFile* cf,FILE* fout) {
    fprintf(fout,"MAGIC: %x\n\n",cf->magic);
}

void print_versions(ClassFile* cf,FILE* fout) {
    fprintf(fout,"VERSION:\n");
    fprintf(fout,"\tMINOR: %d\n",cf->minor_version);
    fprintf(fout,"\tMAJOR: %d\n\n",cf->major_version);
}

void print_constantpool(ClassFile* cf,FILE* fout) {
    long long Long;
    fprintf(fout,"CONSTANT POOL COUNT: %d\n",cf->constant_pool_count);
    fprintf(fout,"CONSTANT_POOL:\n");
    cp_info* cp;
    for(cp = cf->constant_pool;cp<cf->constant_pool+cf->constant_pool_count-1;++cp) {
        switch(cp->tag) {
            case CLASS:
                fprintf(fout,"\tCP_INFO: CLASS\n");
                fprintf(fout,"\tNAME_INDEX: %d: %s\n\n",cp->info.Class_info.name_index,(char*)cf->constant_pool[cp->info.Class_info.name_index-1].info.Utf8_info.bytes);
            break;
            case FIELDREF:
                fprintf(fout,"\tCP_INFO: FIELDREF\n");
                fprintf(fout,"\tCLASS_INDEX: %d: %s\n",cp->info.Fieldref_info.class_index,(char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.class_index-1].info.Class_info.name_index-1].info.Utf8_info.bytes);
                fprintf(fout,"\tNAMEANDTYPE_INDEX: %d: %s%s\n\n",cp->info.Fieldref_info.name_and_type_index,(char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index-1].info.NameAndType_info.name_index-1].info.Utf8_info.bytes,(char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index-1].info.NameAndType_info.descriptor_index-1].info.Utf8_info.bytes);
            break;
            case METHOD:
                fprintf(fout,"\tCP_INFO: METHOD\n");
                fprintf(fout,"\tCLASS_INDEX: %d: %s\n",cp->info.Method_info.class_index,(char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.class_index-1].info.Class_info.name_index-1].info.Utf8_info.bytes);
                fprintf(fout,"\tNAMEANDTYPE_INDEX: %d: %s%s\n\n",cp->info.Method_info.name_and_type_index,(char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index-1].info.NameAndType_info.name_index-1].info.Utf8_info.bytes,(char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index-1].info.NameAndType_info.descriptor_index-1].info.Utf8_info.bytes);
            break;
            case INTERFACE:
                fprintf(fout,"\tCP_INFO: INTERFACE\n");
                fprintf(fout,"\tCLASS_INDEX: %d: %s\n",cp->info.Interface_info.class_index,(char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.class_index-1].info.Class_info.name_index-1].info.Utf8_info.bytes);
                fprintf(fout,"\tNAMEANDTYPE_INDEX: %d: %s%s\n\n",cp->info.Interface_info.name_and_type_index,(char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.name_and_type_index-1].info.NameAndType_info.name_index-1].info.Utf8_info.bytes,(char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.name_and_type_index-1].info.NameAndType_info.descriptor_index-1].info.Utf8_info.bytes);
            break;
            case NAMEANDTYPE:
                fprintf(fout,"\tCP_INFO: NAMEANDTYPE\n");
                fprintf(fout,"\tNAME_INDEX: %d: %s\n",cp->info.NameAndType_info.name_index,(char*)cf->constant_pool[cp->info.NameAndType_info.name_index-1].info.Utf8_info.bytes);
                fprintf(fout,"\tDESCRIPTOR_INDEX: %d: %s\n\n",cp->info.NameAndType_info.descriptor_index,(char*)cf->constant_pool[cp->info.NameAndType_info.descriptor_index-1].info.Utf8_info.bytes);
            break;
            case UTF8:
                fprintf(fout,"\tCP_INFO: UTF8\n");
                fprintf(fout,"\tLENGTH: %d\n",cp->info.Utf8_info.length);
                fprintf(fout,"\tVALUE: %s\n\n",(char*)cp->info.Utf8_info.bytes);
            break;
            case STRING:
                fprintf(fout,"\tCP_INFO: STRING\n");
                fprintf(fout,"\tSTRING_INDEX: %d: %s\n\n",cp->info.String_info.string_index,(char*)cf->constant_pool[cp->info.String_info.string_index-1].info.Utf8_info.bytes);
            break;
            case INTEGER:
                fprintf(fout,"\tCP_INFO: INTEGER\n");
                fprintf(fout,"\tBYTES: %x\n",cp->info.Integer_info.bytes);
                fprintf(fout,"\tVALUE: %d\n\n",cp->info.Integer_info.bytes);
            break;
            case FLOAT:
                fprintf(fout,"\tCP_INFO: FLOAT\n");
                fprintf(fout,"\tBYTES: %x\n",cp->info.Float_info.bytes);
                u4tofloat.U4 = cp->info.Float_info.bytes;
                fprintf(fout,"\tVALUE: %f\n\n",u4tofloat.Float);
            break;
            case LONG:
                fprintf(fout,"\tCP_INFO: LONG\n");
                fprintf(fout,"\tHIGH: %x\n",cp->info.Long_info.high_bytes);
                fprintf(fout,"\tLOW: %x\n", cp->info.Long_info.low_bytes);
                Long = ((long long) cp->info.Long_info.high_bytes << 32) | (cp->info.Long_info.low_bytes);
                fprintf(fout,"\tVALUE: %lld\n\n",Long);
            break;
            case DOUBLE:
                fprintf(fout,"\tCP_INFO: DOUBLE\n");
                fprintf(fout,"\tHIGH: %x\n",cp->info.Double_info.high_bytes);
                fprintf(fout,"\tLOW: %x\n", cp->info.Double_info.low_bytes);
                Long = ((long long) cp->info.Double_info.high_bytes << 32) | (cp->info.Double_info.low_bytes);
                fprintf(fout,"\tVALUE: %lld\n\n",Long);
            break;
        }
    }

}

void print_classdata(ClassFile* cf,FILE* fout) {
    fprintf(fout,"ACCESS_FLAGS: %x ",cf->access_flags);
    print_permissions(cf->access_flags,fout);
    fprintf(fout,"\n");   
    fprintf(fout,"THIS_CLASS: %d\n",cf->this_class);
    fprintf(fout,"SUPER_CLASS: %d\n\n",cf->super_class);
}

void print_interfaces(ClassFile* cf,FILE* fout) {
    fprintf(fout,"INTERFACES_COUNT: %d\n",cf->interfaces_count);
    if(cf->interfaces_count<=0) return;
    fprintf(fout,"INTERFACES:\n");
    u2* interface_aux;
    for(interface_aux = cf->interfaces;interface_aux<cf->interfaces+cf->interfaces_count;++interface_aux) {
        fprintf(fout,"\tINTERFACE: %d\n\n",*interface_aux);
    }
}

void print_attribute(ClassFile* cf, attribute_info* att,FILE* fout) {
    long long Long;
    char* type;
    type = (char*)malloc(sizeof(char)*cf->constant_pool[att->attribute_name_index-1].info.Utf8_info.length);
    strcpy(type,(char*)cf->constant_pool[att->attribute_name_index-1].info.Utf8_info.bytes);
    int i=findtype(type);
    fprintf(fout,"\tATTRIBUTE_NAME_INDEX: %d : %s\n",att->attribute_name_index,(char*)cf->constant_pool[att->attribute_name_index-1].info.Utf8_info.bytes);
    fprintf(fout,"\tATTRIBUTE_LENGTH: %d\n\n",att->attribute_length);
    switch(i) {
        case CONSTANTVALUE:
            fprintf(fout,"\tTYPE: CONSTANT_VALUE\n");
            fprintf(fout,"\tCONSTANTVALUE_INDEX: %d\n\n",att->type.ConstantValue.constantvalue_index);
            switch(cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].tag){
                case INTEGER:
                    fprintf(fout,"\tCP_INFO: INTEGER\n");
                    fprintf(fout,"\tBYTES: %x\n",cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Integer_info.bytes);
                    fprintf(fout,"\tVALUE: %d\n\n",cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Integer_info.bytes);
                break;
                case FLOAT:
                    fprintf(fout,"\tCP_INFO: FLOAT\n");
                    fprintf(fout,"\tBYTES: %x\n",cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Float_info.bytes);
                    u4tofloat.U4 = cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Float_info.bytes;
                    fprintf(fout,"\tVALUE: %f\n\n",u4tofloat.Float);
                break;
                case LONG:
                    fprintf(fout,"\tCP_INFO: LONG\n");
                    fprintf(fout,"\tHIGH: %x\n",cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Long_info.high_bytes);
                    fprintf(fout,"\tLOW: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Long_info.low_bytes);
                    Long = ((long long) cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Long_info.high_bytes << 32) | (cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Long_info.low_bytes);
                    fprintf(fout,"\tVALUE: %lld\n\n",Long);
                break;
                case DOUBLE:
                    fprintf(fout,"\tCP_INFO: DOUBLE\n");
                    fprintf(fout,"\tHIGH: %x\n",cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Double_info.high_bytes);
                    fprintf(fout,"\tLOW: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Double_info.low_bytes);
                    Long = ((long long) cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Double_info.high_bytes << 32) | (cf->constant_pool[att->type.ConstantValue.constantvalue_index-1].info.Double_info.low_bytes);
                    fprintf(fout,"\tVALUE: %lld\n\n",Long);
                break;
                }
        break;
        case CODE:
            fprintf(fout,"\tTYPE: CODE\n");
            fprintf(fout,"\tMAX_STACK: %d\n",att->type.Code.max_stack);
            fprintf(fout,"\tMAX_LOCALS: %d\n",att->type.Code.max_locals);
            fprintf(fout,"\tCODE_LENGTH: %d\n",att->type.Code.code_length);
            fprintf(fout,"\tCODE:\n");
            u1* code;
            for(code = att->type.Code.code;code<att->type.Code.code+att->type.Code.code_length;++code) {
                fprintf(fout,"\t\t%x\n",*code);
            }
            fprintf(fout,"\tEXCEPTION_TABLE_LENGTH: %d\n", att->type.Code.exception_table_length);
            exception_table_info* exp_aux;
            for(exp_aux = att->type.Code.exception_table;exp_aux<att->type.Code.exception_table+att->type.Code.exception_table_length;++exp_aux) {
                fprintf(fout,"\tEXCEPTION:\n");
                fprintf(fout,"\t\tSTART_PC: %d\n",exp_aux->start_pc);
                fprintf(fout,"\t\tEND_PC: %d\n",exp_aux->end_pc);
                fprintf(fout,"\t\tHANDLER_PC: %d\n",exp_aux->handler_pc);
                fprintf(fout,"\t\tCATCH_TYPE: %d\n\n",exp_aux->catch_type);
            }
            fprintf(fout,"\tATTRIBUTES_COUNT: %d\n",att->type.Code.attributes_count);
            attribute_info* att_aux;
            for(att_aux=att->type.Code.attributes;att_aux<att->type.Code.attributes+att->type.Code.attributes_count;++att_aux) {
                print_attribute(cf,att_aux,fout);
            }
        break;
        case EXCEPTIONS:
            fprintf(fout,"\tTYPE: EXCEPTIONS\n");
            fprintf(fout,"\tNUMBER_OF_EXCEPTIONS: %d\n",att->type.Exceptions.number_of_exceptions);
            u2* expt_aux;
            for(expt_aux=att->type.Exceptions.exception_index_table;expt_aux<att->type.Exceptions.exception_index_table+att->type.Exceptions.number_of_exceptions;++expt_aux) {
                fprintf(fout,"\tEXCEPTION:\n");
                fprintf(fout,"\t\tCLASS: %d\n\n",*expt_aux);
            }
        break;
        case INNERCLASSES:
            fprintf(fout,"\tTYPE: INNER CLASSES:\n");
            fprintf(fout,"\tNUMBER_OF_CLASSES: %d\n",att->type.InnerClasses.number_of_classes);
            classtype_info* classtype_aux;
            for(classtype_aux = att->type.InnerClasses.classes;classtype_aux<att->type.InnerClasses.classes+att->type.InnerClasses.number_of_classes;++classtype_aux) {
                fprintf(fout,"\tINNER CLASS:\n");
                fprintf(fout,"\t\tINNER CLASS: %d\n",classtype_aux->inner_class_info_index);
                fprintf(fout,"\t\tOUTER CLASS: %d\n",classtype_aux->outer_class_info_index);
                fprintf(fout,"\t\tINNER NAME: %d\n",classtype_aux->inner_name_index);
                fprintf(fout,"\t\tINNER CLASS ACCESS FLAGS: %x ",classtype_aux->inner_class_access_flags);
                print_permissions(classtype_aux->inner_class_access_flags,fout);
                fprintf(fout,"\n\n");
    fprintf(fout,"\n"); 
            }
        break;
        case OTHER:
        break;
    }
}

void print_fields(ClassFile* cf,FILE* fout) {
    fprintf(fout,"FIELDS_COUNT: %d\n",cf->fields_count);
    if(cf->fields_count<=0) return;
    fprintf(fout,"FIELDS:\n");
    field_info* field_aux;
    for(field_aux = cf->fields;field_aux<cf->fields+cf->fields_count;++field_aux) {
        fprintf(fout,"\tACCESS_FLAGS: %x ",field_aux->access_flags);
        print_permissions(field_aux->access_flags,fout);
        fprintf(fout,"\n");
        fprintf(fout,"\tNAME_INDEX: %d: %s\n",field_aux->name_index,(char*)cf->constant_pool[field_aux->name_index-1].info.Utf8_info.bytes);
        fprintf(fout,"\tDESCRIPTOR_INDEX: %d: %s\n",field_aux->descriptor_index,(char*)cf->constant_pool[field_aux->descriptor_index-1].info.Utf8_info.bytes);
        fprintf(fout,"\tATTRIBUTE_COUNT: %d\n\n",field_aux->attributes_count);
        attribute_info* att_aux;
        for(att_aux = field_aux->attributes;att_aux = field_aux->attributes+field_aux->attributes_count;++att_aux) {
            fprintf(fout,"FIELD_ATTRIBUTE:\n");
            print_attribute(cf,att_aux,fout);
        }
    }   
}

void print_methods(ClassFile* cf,FILE* fout) {
    fprintf(fout,"METHODS_COUNT: %d\n",cf->method_count);
    if(cf->method_count<=0) return;
    fprintf(fout,"METHODS:\n");
    method_info* method_aux;
    for(method_aux = cf->methods;method_aux<cf->methods+cf->method_count;++method_aux) {
        fprintf(fout,"\tACCESS_FLAGS: %x ",method_aux->access_flags);
        print_permissions(method_aux->access_flags,fout);
        fprintf(fout,"\n\n");
        fprintf(fout,"\tNAME_INDEX: %d: %s\n",method_aux->name_index,(char*)cf->constant_pool[method_aux->name_index-1].info.Utf8_info.bytes);
        fprintf(fout,"\tDESCRIPTOR_INDEX: %d: %s\n",method_aux->descriptor_index,(char*)cf->constant_pool[method_aux->descriptor_index-1].info.Utf8_info.bytes);
        fprintf(fout,"\tATTRIBUTE_COUNT: %d\n\n",method_aux->attributes_count);
        attribute_info* att_aux;
        for(att_aux = method_aux->attributes;att_aux < method_aux->attributes+method_aux->attributes_count;++att_aux) {
            fprintf(fout,"METHOD_ATTRIBUTE:\n");
            print_attribute(cf,att_aux,fout);
        }
    } 
}

void print_attributes(ClassFile* cf,FILE* fout) {
    fprintf(fout,"ATTRIBUTES_COUNT: %d\n",cf->attributes_count);
    if(cf->attributes_count<=0) return;
    fprintf(fout,"ATTRIBUTES:\n");
    attribute_info* att_aux;
    for(att_aux = cf->attributes;att_aux<cf->attributes+cf->attributes_count;++att_aux) {
        fprintf(fout,"ATTRIBUTE:\n");
        print_attribute(cf,att_aux,fout);
    }
}

void print_class(ClassFile* cf,char* nomearquivo,FILE* fout) {
    fprintf(fout,"Nome do .class: %s\n\n",nomearquivo);
    fprintf(fout,"----------------------------------------------\n\n");
    print_magic(cf,fout);
    fprintf(fout,"----------------------------------------------\n\n");
    print_versions(cf,fout);
    fprintf(fout,"----------------------------------------------\n\n");
    print_constantpool(cf,fout);
    fprintf(fout,"----------------------------------------------\n\n");
    print_classdata(cf,fout);
    fprintf(fout,"----------------------------------------------\n\n");
    print_interfaces(cf,fout);
    fprintf(fout,"----------------------------------------------\n\n");
    print_fields(cf,fout);
    fprintf(fout,"----------------------------------------------\n\n");
    print_methods(cf,fout);
    fprintf(fout,"----------------------------------------------\n\n");
    print_attributes(cf,fout);
    fprintf(fout,"----------------------------------------------\n\n");
}

int main(int argc, char* argv[]){
    char nomearquivo[1024];
    char saidaarquivo[1024] = "output_class.txt";
    FILE* fd = NULL;
    if (argc == 1) {
        do {
            printf("Digite o nome do arquivo: ");
            scanf("%s",nomearquivo);
            fflush(stdin);
            fd = open_file(nomearquivo);
        } while(!fd);
    } else if (argc == 3) {
        strcpy(nomearquivo,argv[1]);
        fd = open_file(nomearquivo);
        if(!fd) {
            printf("Arquivo de entrada não encontrado!\n");
            return 0;
        }
        strcpy(saidaarquivo,argv[2]);

    } else {
        printf("Uso do programa: ./leitorexibidor [nome-do-class nome-da-saída]\n");
        return 0;
    }
    ClassFile* cf = readClass(fd);
    FILE* fout = fopen(saidaarquivo,"w");
    print_class(cf, nomearquivo,fout);
    fclose(fout);
    fclose(fd);
    printf("Finalizado!\n");
    return 0;
}

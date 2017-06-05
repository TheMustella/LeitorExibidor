#include "exibidor.h"
#include "util.h"

int is_true(int code, int id) {
    return code & (1 << id);
}

void print_permissions(int code, FILE* fout) {
    if (is_true(code, 0)) fprintf(fout, "public ");
    else if (is_true(code, 1)) fprintf(fout, "private ");
    else if (is_true(code, 2)) fprintf(fout, "protected ");

    if (is_true(code, 3)) fprintf(fout, "static ");
    if (is_true(code, 4)) fprintf(fout, "final ");
    if (is_true(code, 5)) fprintf(fout, "super ");
    if (is_true(code, 6)) fprintf(fout, "volatile ");
    if (is_true(code, 7)) fprintf(fout, "transient ");
    if (is_true(code, 8)) fprintf(fout, "native ");
    if (is_true(code, 9)) fprintf(fout, "interface ");
    if (is_true(code, 10)) fprintf(fout, "abstract ");
}

void print_magic(ClassFile* cf, FILE* fout) {
    fprintf(fout, "MAGIC: %x\n\n", cf->magic);
}

void print_versions(ClassFile* cf, FILE* fout) {
    fprintf(fout, "MINOR VERSION: %d\n", cf->minor_version);
    fprintf(fout, "MAJOR VERSION: %d\n", cf->major_version);

    fprintf(fout, "CONSTANT POOL COUNT: %d\n", cf->constant_pool_count);

    fprintf(fout, "ACCESS_FLAGS: %x ", cf->access_flags);
    print_permissions(cf->access_flags, fout);
    fprintf(fout, "\n");
    fprintf(fout, "THIS_CLASS: %d\n", cf->this_class);
    fprintf(fout, "SUPER_CLASS: %d\n", cf->super_class);

    fprintf(fout, "INTERFACES_COUNT: %d\n", cf->interfaces_count);
    fprintf(fout, "FIELDS_COUNT: %d\n", cf->fields_count);
    fprintf(fout, "METHODS_COUNT: %d\n", cf->method_count);
    fprintf(fout, "ATTRIBUTES_COUNT: %d\n\n", cf->attributes_count);

}

void print_constantpool(ClassFile* cf, FILE* fout) {
    int i = 1;
    long long Long;
    fprintf(fout, "CONSTANT POOL COUNT: %d\n", cf->constant_pool_count);
    fprintf(fout, "CONSTANT_POOL:\n");
    cp_info* cp;
    for (cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; ++cp) {
        fprintf(fout, "[%d]\n", i++);
        switch (cp->tag) {
        case CLASS:
            fprintf(fout, "\tCP_INFO: CLASS\n");
            fprintf(fout, "\tNAME_INDEX: %d: %s\n\n", cp->info.Class_info.name_index, (char*)cf->constant_pool[cp->info.Class_info.name_index - 1].info.Utf8_info.bytes);
            break;
        case FIELDREF:
            fprintf(fout, "\tCP_INFO: FIELDREF\n");
            fprintf(fout, "\tCLASS_INDEX: %d: %s\n", cp->info.Fieldref_info.class_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(fout, "\tNAMEANDTYPE_INDEX: %d: %s%s\n\n", cp->info.Fieldref_info.name_and_type_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes, (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case METHOD:
            fprintf(fout, "\tCP_INFO: METHOD\n");
            fprintf(fout, "\tCLASS_INDEX: %d: %s\n", cp->info.Method_info.class_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(fout, "\tNAMEANDTYPE_INDEX: %d: %s%s\n\n", cp->info.Method_info.name_and_type_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes, (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case INTERFACE:
            fprintf(fout, "\tCP_INFO: INTERFACE\n");
            fprintf(fout, "\tCLASS_INDEX: %d: %s\n", cp->info.Interface_info.class_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(fout, "\tNAMEANDTYPE_INDEX: %d: %s%s\n\n", cp->info.Interface_info.name_and_type_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes, (char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.name_and_type_index - 1].info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case NAMEANDTYPE:
            fprintf(fout, "\tCP_INFO: NAMEANDTYPE\n");
            fprintf(fout, "\tNAME_INDEX: %d: %s\n", cp->info.NameAndType_info.name_index, (char*)cf->constant_pool[cp->info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(fout, "\tDESCRIPTOR_INDEX: %d: %s\n\n", cp->info.NameAndType_info.descriptor_index, (char*)cf->constant_pool[cp->info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case UTF8:
            fprintf(fout, "\tCP_INFO: UTF8\n");
            fprintf(fout, "\tLENGTH: %d\n", cp->info.Utf8_info.length);
            fprintf(fout, "\tVALUE: %s\n\n", (char*)cp->info.Utf8_info.bytes);
            break;
        case STRING:
            fprintf(fout, "\tCP_INFO: STRING\n");
            fprintf(fout, "\tSTRING_INDEX: %d: %s\n\n", cp->info.String_info.string_index, (char*)cf->constant_pool[cp->info.String_info.string_index - 1].info.Utf8_info.bytes);
            break;
        case INTEGER:
            fprintf(fout, "\tCP_INFO: INTEGER\n");
            fprintf(fout, "\tBYTES: %x\n", cp->info.Integer_info.bytes);
            fprintf(fout, "\tVALUE: %d\n\n", cp->info.Integer_info.bytes);
            break;
        case FLOAT:
            fprintf(fout, "\tCP_INFO: FLOAT\n");
            fprintf(fout, "\tBYTES: %x\n", cp->info.Float_info.bytes);
            u4tofloat.U4 = cp->info.Float_info.bytes;
            fprintf(fout, "\tVALUE: %f\n\n", u4tofloat.Float);
            break;
        case LONG:
            fprintf(fout, "\tCP_INFO: LONG\n");
            fprintf(fout, "\tHIGH: %x\n", cp->info.Long_info.high_bytes);
            fprintf(fout, "\tLOW: %x\n", cp->info.Long_info.low_bytes);
            Long = ((long long) cp->info.Long_info.high_bytes << 32) | (cp->info.Long_info.low_bytes);
            fprintf(fout, "\tVALUE: %lld\n\n", Long);
            break;
        case DOUBLE:
            fprintf(fout, "\tCP_INFO: DOUBLE\n");
            fprintf(fout, "\tHIGH: %x\n", cp->info.Double_info.high_bytes);
            fprintf(fout, "\tLOW: %x\n", cp->info.Double_info.low_bytes);
            Long = ((long long) cp->info.Double_info.high_bytes << 32) | (cp->info.Double_info.low_bytes);
            fprintf(fout, "\tVALUE: %lld\n\n", Long);
            break;
        }
    }

}

void print_classdata(ClassFile* cf, FILE* fout) {
    fprintf(fout, "ACCESS_FLAGS: %x ", cf->access_flags);
    print_permissions(cf->access_flags, fout);
    fprintf(fout, "\n");
    fprintf(fout, "THIS_CLASS: %d\n", cf->this_class);
    fprintf(fout, "SUPER_CLASS: %d\n\n", cf->super_class);
}

void print_interfaces(ClassFile* cf, FILE* fout) {
    fprintf(fout, "INTERFACES_COUNT: %d\n", cf->interfaces_count);
    if (cf->interfaces_count <= 0) {
        fprintf(fout, "\n");
        return;
    }
    fprintf(fout, "INTERFACES:\n");
    u2* interface_aux;
    for (interface_aux = cf->interfaces; interface_aux < cf->interfaces + cf->interfaces_count; ++interface_aux) {
        fprintf(fout, "\tINTERFACE: %d\n\n", *interface_aux);
    }
}

void print_attribute(ClassFile* cf, attribute_info* att, FILE* fout) {
    long long Long;
    char* type;
    type = (char*)malloc(sizeof(char) * cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.length);
    strcpy(type, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
    int i = findtype(type);
    fprintf(fout, "\tATTRIBUTE_NAME_INDEX: %d : %s\n", att->attribute_name_index, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
    fprintf(fout, "\tATTRIBUTE_LENGTH: %d\n\n", att->attribute_length);
    switch (i) {
    case CONSTANTVALUE:
        fprintf(fout, "\tTYPE: CONSTANT_VALUE\n");
        fprintf(fout, "\tCONSTANTVALUE_INDEX: %d\n\n", att->type.ConstantValue.constantvalue_index);
        switch (cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].tag) {
        case INTEGER:
            fprintf(fout, "\tCP_INFO: INTEGER\n");
            fprintf(fout, "\tBYTES: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Integer_info.bytes);
            fprintf(fout, "\tVALUE: %d\n\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Integer_info.bytes);
            break;
        case FLOAT:
            fprintf(fout, "\tCP_INFO: FLOAT\n");
            fprintf(fout, "\tBYTES: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Float_info.bytes);
            u4tofloat.U4 = cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Float_info.bytes;
            fprintf(fout, "\tVALUE: %f\n\n", u4tofloat.Float);
            break;
        case LONG:
            fprintf(fout, "\tCP_INFO: LONG\n");
            fprintf(fout, "\tHIGH: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.high_bytes);
            fprintf(fout, "\tLOW: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.low_bytes);
            Long = ((long long) cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.high_bytes << 32) | (cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.low_bytes);
            fprintf(fout, "\tVALUE: %lld\n\n", Long);
            break;
        case DOUBLE:
            fprintf(fout, "\tCP_INFO: DOUBLE\n");
            fprintf(fout, "\tHIGH: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.high_bytes);
            fprintf(fout, "\tLOW: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.low_bytes);
            Long = ((long long) cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.high_bytes << 32) | (cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.low_bytes);
            fprintf(fout, "\tVALUE: %lld\n\n", Long);
            break;
        }
        break;
    case CODE:
        fprintf(fout, "\tTYPE: CODE\n");
        fprintf(fout, "\tMAX_STACK: %d\n", att->type.Code.max_stack);
        fprintf(fout, "\tMAX_LOCALS: %d\n", att->type.Code.max_locals);
        fprintf(fout, "\tCODE_LENGTH: %d\n", att->type.Code.code_length);
        fprintf(fout, "\tCODE:\n");
        u1* code;
        for (code = att->type.Code.code; code < att->type.Code.code + att->type.Code.code_length; ++code) {
            fprintf(fout, "\t\t%x\n", *code);
        }
        fprintf(fout, "\tEXCEPTION_TABLE_LENGTH: %d\n", att->type.Code.exception_table_length);
        exception_table_info* exp_aux;
        for (exp_aux = att->type.Code.exception_table; exp_aux < att->type.Code.exception_table + att->type.Code.exception_table_length; ++exp_aux) {
            fprintf(fout, "\tEXCEPTION:\n");
            fprintf(fout, "\t\tSTART_PC: %d\n", exp_aux->start_pc);
            fprintf(fout, "\t\tEND_PC: %d\n", exp_aux->end_pc);
            fprintf(fout, "\t\tHANDLER_PC: %d\n", exp_aux->handler_pc);
            fprintf(fout, "\t\tCATCH_TYPE: %d\n\n", exp_aux->catch_type);
        }
        fprintf(fout, "\tATTRIBUTES_COUNT: %d\n", att->type.Code.attributes_count);
        attribute_info* att_aux;
        for (att_aux = att->type.Code.attributes; att_aux < att->type.Code.attributes + att->type.Code.attributes_count; ++att_aux) {
            print_attribute(cf, att_aux, fout);
        }
        break;
    case EXCEPTIONS:
        fprintf(fout, "\tTYPE: EXCEPTIONS\n");
        fprintf(fout, "\tNUMBER_OF_EXCEPTIONS: %d\n", att->type.Exceptions.number_of_exceptions);
        u2* expt_aux;
        for (expt_aux = att->type.Exceptions.exception_index_table; expt_aux < att->type.Exceptions.exception_index_table + att->type.Exceptions.number_of_exceptions; ++expt_aux) {
            fprintf(fout, "\tEXCEPTION:\n");
            fprintf(fout, "\t\tCLASS: %d\n\n", *expt_aux);
        }
        break;
    case INNERCLASSES:
        fprintf(fout, "\tTYPE: INNER CLASSES:\n");
        fprintf(fout, "\tNUMBER_OF_CLASSES: %d\n", att->type.InnerClasses.number_of_classes);
        classtype_info* classtype_aux;
        for (classtype_aux = att->type.InnerClasses.classes; classtype_aux < att->type.InnerClasses.classes + att->type.InnerClasses.number_of_classes; ++classtype_aux) {
            fprintf(fout, "\tINNER CLASS:\n");
            fprintf(fout, "\t\tINNER CLASS: %d\n", classtype_aux->inner_class_info_index);
            fprintf(fout, "\t\tOUTER CLASS: %d\n", classtype_aux->outer_class_info_index);
            fprintf(fout, "\t\tINNER NAME: %d\n", classtype_aux->inner_name_index);
            fprintf(fout, "\t\tINNER CLASS ACCESS FLAGS: %x ", classtype_aux->inner_class_access_flags);
            print_permissions(classtype_aux->inner_class_access_flags, fout);
            fprintf(fout, "\n\n");
            fprintf(fout, "\n");
        }
        break;
    case OTHER:
        break;
    }
}

void print_fields(ClassFile* cf, FILE* fout) {
    int i1 = 0, i2 = 0;
    fprintf(fout, "FIELDS_COUNT: %d\n", cf->fields_count);
    if (cf->fields_count <= 0) {
        fprintf(fout, "\n");
        return;
    }
    fprintf(fout, "FIELDS:\n");
    field_info* field_aux;
    for (field_aux = cf->fields; field_aux < cf->fields + cf->fields_count; ++field_aux) {
        fprintf(fout, "\t[%d]\n", i1++);
        fprintf(fout, "\tNAME_INDEX: %d: %s\n", field_aux->name_index, (char*)cf->constant_pool[field_aux->name_index - 1].info.Utf8_info.bytes);
        fprintf(fout, "\tDESCRIPTOR_INDEX: %d: %s\n", field_aux->descriptor_index, (char*)cf->constant_pool[field_aux->descriptor_index - 1].info.Utf8_info.bytes);
        fprintf(fout, "\tACCESS_FLAGS: %x ", field_aux->access_flags);
        print_permissions(field_aux->access_flags, fout);
        fprintf(fout, "\n");
        fprintf(fout, "\tATTRIBUTE_COUNT: %d\n\n", field_aux->attributes_count);
        attribute_info* att_aux;
        for (att_aux = field_aux->attributes; att_aux < field_aux->attributes + field_aux->attributes_count; ++att_aux) {
            fprintf(fout, "[%d] FIELD_ATTRIBUTE:\n", i2++);
            print_attribute(cf, att_aux, fout);
        }
    }
}

void print_methods(ClassFile* cf, FILE* fout) {
    int i1 = 0, i2 = 0;
    fprintf(fout, "METHODS_COUNT: %d\n", cf->method_count);
    if (cf->method_count <= 0) {
        fprintf(fout, "\n");
        return;
    }
    fprintf(fout, "METHODS:\n");
    method_info* method_aux;
    for (method_aux = cf->methods; method_aux < cf->methods + cf->method_count; ++method_aux) {
        fprintf(fout, "[%d]\n", i1++);
        fprintf(fout, "\tNAME_INDEX: %d: %s\n", method_aux->name_index, (char*)cf->constant_pool[method_aux->name_index - 1].info.Utf8_info.bytes);
        fprintf(fout, "\tDESCRIPTOR_INDEX: %d: %s\n", method_aux->descriptor_index, (char*)cf->constant_pool[method_aux->descriptor_index - 1].info.Utf8_info.bytes);
        fprintf(fout, "\tACCESS_FLAGS: %x ", method_aux->access_flags);
        print_permissions(method_aux->access_flags, fout);
        fprintf(fout, "\n\n");
        fprintf(fout, "\tATTRIBUTE_COUNT: %d\n\n", method_aux->attributes_count);
        attribute_info* att_aux;
        for (att_aux = method_aux->attributes; att_aux < method_aux->attributes + method_aux->attributes_count; ++att_aux) {
            fprintf(fout, "\t[%d] METHOD_ATTRIBUTE:\n", i2++);
            print_attribute(cf, att_aux, fout);
        }
    }
}

void print_attributes(ClassFile* cf, FILE* fout) {
    int i = 0;
    fprintf(fout, "ATTRIBUTES_COUNT: %d\n", cf->attributes_count);
    if (cf->attributes_count <= 0) {
        fprintf(fout, "\n");
        return;
    }
    fprintf(fout, "ATTRIBUTES:\n");
    attribute_info* att_aux;
    for (att_aux = cf->attributes; att_aux < cf->attributes + cf->attributes_count; ++att_aux) {
        fprintf(fout, "[%d] ATTRIBUTE:\n", i++);
        print_attribute(cf, att_aux, fout);
    }
}

void print_class(ClassFile* cf, char* nomearquivo, FILE* fout) {
    fprintf(fout, "Nome do .class: %s\n\n", nomearquivo);
    fprintf(fout, "----------------------------------------------\n\n");
    print_magic(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    print_versions(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    // print_classdata(cf, fout);
    // fprintf(fout, "----------------------------------------------\n\n");
    print_constantpool(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    print_interfaces(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    print_fields(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    print_methods(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
    print_attributes(cf, fout);
    fprintf(fout, "----------------------------------------------\n\n");
}

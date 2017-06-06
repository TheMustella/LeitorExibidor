#include "exibidor.h"
#include "util.h"

int is_true(int code, int id) {
    return code & (1 << id);
}

void print_permissions(int code, FILE* fout) {
    fprintf(fout, "[");
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
    fprintf(fout, "]");
}

void print_magic(ClassFile* cf, FILE* fout) {
    fprintf(fout, "MAGIC: %x\n\n", cf->magic);
}

void print_versions(ClassFile* cf, FILE* fout) {
    fprintf(fout, "MINOR VERSION: %d\n", cf->minor_version);
    char *nome_versao =  show_version(cf->major_version);
    fprintf(fout, "MAJOR VERSION: %d - %s\n", cf->major_version, nome_versao);
    free(nome_versao);

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
            fprintf(fout, "\tVALUE: %u\n\n", cp->info.Integer_info.bytes);
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
    if (cf->interfaces_count == 0) {
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

char* tiponewarray_conteudo[] = {NULL, NULL, NULL, NULL, "T_BOOLEAN", "T_CHAR", "T_FLOAT", "T_DOUBLE", "T_BYTE", "T_SHORT","T_INT", "T_LONG"};

char* instrucoes_nomes[] = { //10 instrucoes por linha
    "nop", "aconst_null", "iconst_m1", "iconst_0", "iconst_1", "iconst_2", "iconst_3", "iconst_4", "iconst_5", "lconst_0", //0 ao 9
    "lconst_1", "fconst_0", "fconst_1", "fconst_2", "dconst_0", "dconst_1", "bipush", "sipush", "ldc", "ldc_w",
    "ldc2_w", "iload", "lload", "fload","dload", "aload", "iload_0", "iload_1", "iload_2", "iload_3",
    "lload_0", "lload_1", "lload_2", "lload_3", "fload_0", "fload_1", "fload_2", "fload_3", "dload_0", "dload_1",
    "dload_2", "dload_3", "aload_0", "aload_1", "aload_2", "aload_3", "iaload", "laload", "faload", "daload",
    "aaload", "baload", "caload", "saload", "istore", "lstore", "fstore", "dstore", "astore", "istore_0",
    "istore_1", "istore_2", "istore_3", "lstore_0", "lstore_1", "lstore_2", "lstore_3", "fstore_0", "fstore_1", "fstore_2",
    "fstore_3", "dstore_0", "dstore_1", "dstore_2", "dstore_3", "astore_0", "astore_1", "astore_2", "astore_3", "iastore",
    "lastore", "fastore", "dastore", "aastore", "bastore", "castore", "sastore", "pop", "pop2", "dup",
    "dup_x1", "dup_x2", "dup2", "dup2_x1", "dup2_x2", "swap", "iadd", "ladd", "fadd", "dadd",
    "isub", "lsub", "fsub", "dsub", "imul", "lmul", "fmul", "dmul", "idiv", "ldiv", //100 ao 109
    "fdiv", "ddiv", "irem", "lrem", "frem", "drem", "ineg", "lneg", "fneg", "dneg",
    "ishl", "lshl", "ishr", "lshr", "iushr", "lushr", "iand", "land", "ior", "lor",
    "ixor", "lxor", "iinc", "i2l", "i2f", "i2d", "l2i", "l2f", "l2d", "f2i",
    "f2l", "f2d", "d2i", "d2l", "d2f", "i2b", "i2c", "i2s", "lcmp", "fcmpl",
    "fcmpg", "dcmpl", "dcmpg", "ifeq", "ifne", "iflt", "ifge","ifgt", "ifle", "if_icmpeq", //150 ao 159
    "if_icmpne", "if_icmplt", "if_icmpge", "if_icmpgt", "if_icmple", "if_acmpeq", "if_acmpne", "goto", "jsr", "ret",
    "tableswitch", "lookupswitch", "ireturn", "lreturn", "freturn", "dreturn", "areturn", "return", "getstatic", "putstatic",
    "getfield", "putfield", "invokevirtual", "invokespecial", "invokestatic", "invokeinterface", "invokedynamic", "new", "newarray", "anewarray",
    "arraylength", "athrow", "checkcast", "instanceof", "monitorenter", "monitorexit", "wide", "multianewarray", "ifnull", "ifnonnull",
    "goto_w", "jsr_w", "breakpoint", NULL, NULL, NULL, NULL, NULL, NULL, NULL, //200 ao 209
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, "impdep1", "impdep2" //250 ao 255
};

enum instrucoes_code { //10 instrucoes por linha
    nop, aconst_null, iconst_m1, iconst_0, iconst_1, iconst_2, iconst_3, iconst_4, iconst_5, lconst_0, //0 ao 9
    lconst_1, fconst_0, fconst_1, fconst_2, dconst_0, dconst_1, bipush, sipush, ldc, ldc_w,
    ldc2_w, iload, lload, fload,dload, aload, iload_0, iload_1, iload_2, iload_3,
    lload_0, lload_1, lload_2, lload_3, fload_0, fload_1, fload_2, fload_3, dload_0, dload_1,
    dload_2, dload_3, aload_0, aload_1, aload_2, aload_3, iaload, laload, faload, daload,
    aaload, baload, caload, saload, istore, lstore, fstore, dstore, astore, istore_0,
    istore_1, istore_2, istore_3, lstore_0, lstore_1, lstore_2, lstore_3, fstore_0, fstore_1, fstore_2,
    fstore_3, dstore_0, dstore_1, dstore_2, dstore_3, astore_0, astore_1, astore_2, astore_3, iastore,
    lastore, fastore, dastore, aastore, bastore, castore, sastore, pop, pop2, dup,
    dup_x1, dup_x2, dup2, dup2_x1, dup2_x2, swap, iadd, ladd, fadd, dadd,
    isub, lsub, fsub, dsub, imul, lmul, fmul, dmul, idiv, ldiv, //100 ao 109
    fdiv, ddiv, irem, lrem, frem, drem, ineg, lneg, fneg, dneg,
    ishl, lshl, ishr, lshr, iushr, lushr, iand, land, ior, lor,
    ixor, lxor, iinc, i2l, i2f, i2d, l2i, l2f, l2d, f2i,
    f2l, f2d, d2i, d2l, d2f, i2b, i2c, i2s, lcmp, fcmpl,
    fcmpg, dcmpl, dcmpg, ifeq, ifne, iflt, ifge,ifgt, ifle, if_icmpeq, //150 ao 159
    if_icmpne, if_icmplt, if_icmpge, if_icmpgt, if_icmple, if_acmpeq, if_acmpne, goto2, jsr, ret,
    tableswitch, lookupswitch, ireturn, lreturn, freturn, dreturn, areturn, return2, getstatic, putstatic,
    getfield, putfield, invokevirtual, invokespecial, invokestatic, invokeinterface, invokedynamic, new, newarray, anewarray,
    arraylength, athrow, checkcast, instanceof, monitorenter, monitorexit, wide, multianewarray, ifnull, ifnonnull,
    goto_w, jsr_w, breakpoint, impdep1 = 254, impdep2 = 255 //250 ao 255
};

    long long Long;
    char* type;
    type = (char*)malloc(sizeof(char) * cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.length+1);
    strcpy(type, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
    int i = findtype(type);
    fprintf(fout, "\tATTRIBUTE_NAME_INDEX: %d : %s\n", att->attribute_name_index, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
    fprintf(fout, "\tATTRIBUTE_LENGTH: %u\n\n", att->attribute_length);
    switch (i) {
    case CONSTANTVALUE:
        fprintf(fout, "\tTYPE: CONSTANT_VALUE\n");
        fprintf(fout, "\tCONSTANTVALUE_INDEX: %d\n\n", att->type.ConstantValue.constantvalue_index);
        switch (cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].tag) {
        case INTEGER:
            fprintf(fout, "\tCP_INFO: INTEGER\n");
            fprintf(fout, "\tBYTES: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Integer_info.bytes);
            fprintf(fout, "\tVALUE: %u\n\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Integer_info.bytes);
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
        fprintf(fout, "\tCODE_LENGTH: %u\n", att->type.Code.code_length);
        fprintf(fout, "\tCODE:\n");
        u1* code;
        cp_info* cp;
        for (code = att->type.Code.code; code < att->type.Code.code + att->type.Code.code_length; ++code) {
            //fprintf(fout, "\t\t%d | %02x | ", (int) (code - (att->type.Code.code)), *code); //printa o codigo em hexa da instrucao e a instrucao em questao
            fprintf(fout, "\t\t%d | ", (int) (code - (att->type.Code.code))); //printa a instrucao sem o codigo em hexa

            fprintf(fout, "%s ", instrucoes_nomes[*code]); //printa a instrucao
            int8_t byte_aux;
            int16_t half_aux;
            int32_t word_aux;
            
            switch (*code) { //TRATAR INSTRUCOES QUE ARMAZENAM OPERANDOS NA PILHA
                case aload:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case anewarray:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case astore:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case bipush:
                    fprintf(fout, "%d", *(++code)); //byte
                    break;
                case checkcast:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case dload:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case dstore:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case fload:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case fstore:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case getfield:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    cp = cf->constant_pool +  half_aux - 1;
                    fprintf(fout, "#%d <%s.%s>", half_aux, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                    break;
                case getstatic:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case goto2: //REVER - MOSTRANDO VALOR INESPERADO
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case goto_w:
                    word_aux = *(++code); //byte1 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte2 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte3 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte4 de branch
                    fprintf(fout, "%d", word_aux); //print branchbyte

                    break;
                case if_acmpeq:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_acmpne:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmple:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmpgt:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmpge:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmplt:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmpne:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case if_icmpeq:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifeq:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifne:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifgt:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case iflt:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifle:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifge:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifnonnull:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case ifnull:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case iinc:
                    byte_aux = *(++code); //byte de index
                    fprintf(fout, "%d by %d", byte_aux, *(++code)); //print branchbyte
                    break;
                case iload:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case instanceof:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case invokedynamic:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    ++code; //leitura de 0 - caracteristica do invokedynamic
                    ++code; //leitura de 0 - caracteristica do invokedynamic
                    break;
                case invokeinterface:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    fprintf(fout, " count %d", *(++code)); //count
                    ++code; //leitura de 0 - caracteristica do invokedynamic
                    break;
                case invokespecial:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    cp = cf->constant_pool + half_aux - 1;
                    fprintf(fout, "#%d <%s.%s>", half_aux, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes, 
						(char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                    break;
                case invokestatic:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case invokevirtual:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    cp = cf->constant_pool + half_aux - 1;
                    fprintf(fout, "#%d <%s.%s>", half_aux, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                    break;
                case istore:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case jsr:
                    half_aux = *(++code); //byte1 de branch
                    half_aux = half_aux << 8; //shift de branch
                    half_aux += *(++code); //byte2 de branch
                    fprintf(fout, "%d", half_aux); //print branchbyte
                    break;
                case jsr_w:
                    word_aux = *(++code); //byte1 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte2 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte3 de branch
                    word_aux = word_aux << 8; //shift de branch
                    word_aux += *(++code); //byte4 de branch
                    fprintf(fout, "%d", word_aux); //print branchbyte
                    break;
                case ldc:
                    byte_aux = *(++code); //index
                    cp = cf->constant_pool + half_aux - 1;
                    fprintf(fout, "#%d", half_aux); //CONTEUDO PODE SER STRING, FLOAT...
                    break;
                case ldc_w:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case ldc2_w:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case lload:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case lstore:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case multianewarray:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    fprintf(fout, " dim %d", *(++code)); //dimensions
                    break;
                case new:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    cp = cf->constant_pool +  half_aux - 1;
                    fprintf(fout, "#%d, <%s>", half_aux, (char*)cf->constant_pool[cp->info.Class_info.name_index - 1].info.Utf8_info.bytes); //print indexbyte
                    break;
                case newarray:
                    fprintf(fout, "%s", tiponewarray_conteudo[*(++code)]); //string referente a codificacao do tipo em questao
                    break;
                case putfield:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index

                    cp = cf->constant_pool +  half_aux - 1;
                    fprintf(fout, "#%d <%s.%s>", half_aux, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes, 
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                    break;
                case putstatic:
                    half_aux = *(++code); //byte1 de index
                    half_aux = half_aux << 8; //shift de index
                    half_aux += *(++code); //byte2 de index
                    fprintf(fout, "#%d", half_aux); //print indexbyte
                    break;
                case ret:
                    fprintf(fout, "#%d", *(++code)); //index
                    break;
                case sipush:
                    half_aux = *(++code); //byte1
                        half_aux = half_aux << 8; //shift
                        half_aux += *(++code); //byte2
                        fprintf(fout, "%d", half_aux); //print
                    break;
                case wide:
                    if ((*(++code)) == iinc) { //caso o proximo byte seja iinc
                        half_aux = *(++code); //byte1 de index
                        half_aux = half_aux << 8; //shift de index
                        half_aux += *(++code); //byte2 de index
                        fprintf(fout, "#%d", half_aux); //print indexbyte
                        
                        half_aux = *(++code); //byte1 de constante
                        half_aux = half_aux << 8; //shift de constante
                        half_aux += *(++code); //byte2 de constante
                        fprintf(fout, " const %d", half_aux); //print constante
                    }
                    else {
                        half_aux = *(++code); //byte1 de index
                        half_aux = half_aux << 8; //shift de index
                        half_aux += *(++code); //byte2 de index
                        fprintf(fout, "#%d", half_aux); //print indexbyte
                    }
                    break;

                case lookupswitch: //REVER - ESTUDAR SOBRE
                    break;
                case tableswitch: //REVER - ESTUDAR SOBRE
                    break;
            }


            fprintf(fout, "\n");
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
    free(type);
}

void print_fields(ClassFile* cf, FILE* fout) {
    int i1 = 0, i2 = 0;
    fprintf(fout, "FIELDS_COUNT: %d\n", cf->fields_count);
    if (cf->fields_count == 0) {
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
    if (cf->method_count == 0) {
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
    if (cf->attributes_count == 0) {
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

char* show_version(int code) {
//Minor Version funciona como subversão
    
    char* nome_versao;
    switch (code) {
        case 45:
            nome_versao = (char*) malloc(sizeof(char) * 8);
            strcpy(nome_versao, "JDK 1.1");
            break;
        case 46:
            nome_versao = (char*) malloc(sizeof(char) * 8);
            strcpy(nome_versao, "JDK 1.2");
            break;
        case 47:
            nome_versao = (char*) malloc(sizeof(char) * 8);
            strcpy(nome_versao, "JDK 1.3");
            break;
        case 48:
            nome_versao = (char*) malloc(sizeof(char) * 8);
            strcpy(nome_versao, "JDK 1.4");
            break;
        case 49:
            nome_versao = (char*) malloc(sizeof(char) * 12);
            strcpy(nome_versao, "Java SE 5.0");
            break;
        case 50:
            nome_versao = (char*) malloc(sizeof(char) * 12);
            strcpy(nome_versao, "Java SE 6.0");
            break;
        case 51:
            nome_versao = (char*) malloc(sizeof(char) * 10);
            strcpy(nome_versao, "Java SE 7");
            break;
        case 52:
            nome_versao = (char*) malloc(sizeof(char) * 10);
            strcpy(nome_versao, "Java SE 8");
            break;
        case 53:
            nome_versao = (char*) malloc(sizeof(char) * 10);
            strcpy(nome_versao, "Java SE 9");
            break;
        default:
            nome_versao = (char*) malloc(sizeof(char) * 21);
            strcpy(nome_versao, "Java nao reconhecido");
            break;
    }
    return nome_versao;
}

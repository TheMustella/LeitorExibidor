/*
 * Arquivo util.c
 *
 * Objetivo principal: cuidar de funcoes auxiliares para o leitor e exibidor
 * */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include "leitor_exibidor.h"
#include "util.h"

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

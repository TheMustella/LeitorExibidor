#include <stdint.h>
#include <stdio.h>

#define class_filename "HelloWorld.class" //arquivo .class de teste

void printFullClass (FILE*);
uint8_t	read1Byte(FILE*);
uint16_t read2Byte(FILE*);
uint32_t read4Byte(FILE*);
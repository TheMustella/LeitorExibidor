#include "Lib.h"

void printFullClass (FILE* arquivo_class) { //Metodo para exibir todos os bytes do .class

	uint8_t atual = 0x0;
	while ((atual = getc(arquivo_class)) != (uint8_t) EOF) { //a comparaçao entre uint8 e EOF nunca seria verdadeira, casting necessario
		printf("%02x", atual);
	}

}

uint8_t read1Byte(FILE * arquivo_class){ //Le um byte a partir da posicao atual do FILE*
    uint8_t	final = getc(arquivo_class); //getc traz 1 byte
    if(final == (uint8_t) EOF){ //a comparaçao entre uint8 e EOF nunca seria verdadeira, casting necessario
        puts("ClassFormatError");
    }
    return final;
}

uint16_t read2Byte(FILE *arquivo_class){ //Le 2 bytes a partir da posicao atual do FILE*
    
    uint8_t parte1 = read1Byte (arquivo_class);
	uint8_t parte2 = read1Byte (arquivo_class);
	
	uint26_t final = (parte1 << 8) | (parte2);

	//printf("final eh: %x\n", final);

    return final;
}

uint32_t read4Byte(FILE *arquivo_class){ //Le 4 bytes a partir da posicao atual do FILE*
    
    uint8_t parte1 = read1Byte (arquivo_class);
	uint8_t parte2 = read1Byte (arquivo_class);
	uint8_t parte3 = read1Byte (arquivo_class);
	uint8_t parte4 = read1Byte (arquivo_class);
	
	uint32_t final = (parte1 << 24) | (parte2 << 16) | (parte3 << 8) | (parte4);

	//printf("final eh: %x\n", final);

    return final;
}

int	main (int argc, char* argv[]) {
	FILE* arquivo_class;

	arquivo_class = fopen(class_filename, "rb");

	//uint8_t bytelido = read1Byte(arquivo_class);
	//printf("%x\n", bytelido);

	uint32_t bytelido = read4Byte(arquivo_class);
	if (bytelido == 0xcafebabe) {
		puts("Leu CAFEBABE\n");
	}
	else {
		puts("Leu outra coisa\n");
	}
	printf("%x\n", bytelido);

	bytelido = read4Byte(arquivo_class);
	if (bytelido == 0xcafebabe) {
		puts("Leu CAFEBABE\n");
	}
	else {
		puts("Leu outra coisa\n");
	}
	printf("%x\n", bytelido);

	/*printFullClass (arquivo_class);

	puts("\n");

	while (1) {
		uint8_t bytelido = read1Byte(arquivo_class);
	};*/

	fclose(arquivo_class);
	return 0;

}
#include <stdio.h>

// Head of the functions that are located inside this file
#include "../includes/bitUtils.h"

// Include all the constants
#include "../includes/def.h"

// Include all macros
#include "../includes/macros.h"



void printBitVector(char bitVector) {
	for (int i = 0; i < NUCLEOTIDE_NUMBER * 2; i++) {
		printf("%d", getBit(i));
		if (i == 3) {
			printf(" ");
		}
	}
	printf("\n");
}

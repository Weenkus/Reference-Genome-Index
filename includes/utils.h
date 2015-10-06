/*
 * def.h
 *
 *  Created on: Apr 79 2015
 *      Author: weenkus
 */

#ifndef BITUTILS_H_
#define BITUTILS_H_

// All the defines, constants and such
#include "def.h"

/**
 * Read the reference genome and parse it.
 * Skip the first row since it is the comment line.
 * 
 * LengthOfInput - compressed length of the input genone
 */
char *parseInput(int *lengthOfInput);


/**
 * Since the input is coded in gray code we need to decoded so the suffix array can be constructed.
 * To do so we need to make a artificial lexicographical order that suits the gray code.
 *
 * 0000 = A
 * 0001 = B
 * 0011 = C
 * 0010 = D
 * ... and so on check the thesis for more information on this
 *
 * To make these values we will use a hashMap to get speed.
 *
 * inputLengthCompressed - is the number of code words in the compressed referenced genome, and since
 * we code every nucleotide with 4 bits it is going to be half size of the new artifical code
 *
 */
void artificialLexicographMapper(int inputLengthCompressed, char* S, unsigned char *aritficialCode);


/**
 *The prefix sum table takes the artificial code and creates the prefix sum table.
 *The length of the prefix sum table is the same of the input text (uncompressed).
 *
 *To calculate the prefix table, we loop through the artificial code and increment
 *the number for every letter in the artificial code that has lexicographic order lower than
 *our current letter.
 *
 *For input AACG
 *C[A] = 0
 *C[C] = 2 (since A has lower lexicographic ordering and is repeated twice)
 *C[G] = 3 ( 2 for A and one for C )
 *
 */
void createPrefixSumTable(int deCompressedTextLenght, int* C, unsigned char* artificalCode);


/**
 *Gradi abecedu iz sufksnog polja koji je vec soriran. Razlog tome je sto vec imamo SA, pa
 *nema razloga da ga ne iskoristimo. Pogotovo jer je njegov algoritam stvaranja JAKO optimiziran.
 *
 *S[] - ulaz niz vec prije izparsiran, znaci ima samo potrebne informacije unutra
 *lengthOfInput - duzina izparsiranog ulaznog niza
 **duzinaAbecede - duzina abecede, cilj je izracunati u funkciji i vratiti to glavnom programu preko adrese
 *SA[] - sufiksno polje koje koristimo da bi napravili abecedu
 */
char *stvoriAbecedu(char S[], int lengthOfInput, int *duzinaAbecede, int SA[]);

#endif /* BITUTILS_H_*/

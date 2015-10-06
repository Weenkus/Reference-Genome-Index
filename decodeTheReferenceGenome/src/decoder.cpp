/*
 * decoder.cpp
 *
 *  Created on: Apr 6, 2015
 *      Author: weenkus
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <unordered_set>
#include <string>

// Include all the constants
#include "../includes/def.h"

// Include all macros
#include "../includes/macros.h"

// Includes all functions need for easy bit usage, such as printBitVector
#include "../includes/bitUtils.h"

void fillTheIUPACtable(std::map<std::string, char>& IUPACmap);

int main(int argc, char **argv) {
	FILE *referenceGenome, *decodedReferenceGenome;

	// Opening all necessary files
	referenceGenome = fopen("../output/referenceGenome", "r+");
	if (referenceGenome == NULL) {
		printf("\nThe OS didn't manage to open the referenceGenome!");
		return 1;
	}
	decodedReferenceGenome = fopen("../output/decodedReferenceGenome", "w+");
	if (referenceGenome == NULL) {
		printf("\nThe OS didn't manage to open the decodedReferenceGenpome!");
		return 1;
	}

	// Initialise the IUPAC hash table
	std::map<std::string, char> IUPACtable;
	fillTheIUPACtable(IUPACtable);

	char encChar;
	bool comment = false;
	char codeTable[NUCLEOTIDE_NUMBER + 1];
	int index { 0 };

	while ((fscanf(referenceGenome, "%c", &encChar)) != EOF) {

		char bitVector = 0x00;
		char codeWord[NUCLEOTIDE_NUMBER + 1];
		codeWord[NUCLEOTIDE_NUMBER * 2] = '\0';	// * 2 cus there are two code words inside one byte
		int fillNumber = 0;

		// Start of comment line
		if (encChar == '#') {
			comment = true;
			// End of comment line
		} else if (comment == true && encChar == '\n') {
			comment = false;
			// Map the code table, within the comment line
		} else if (comment == true && index < NUCLEOTIDE_NUMBER) {
			codeTable[index] = encChar;
			index++;
			// Decode
		} else if (comment == false) {
			// Decode the first code word (first 4 bits of the byte)
			bitVector = encChar;

			// Decode the first code word / the first part of the byte (first 4 bits)
			for (int i = 0; i < NUCLEOTIDE_NUMBER; i++) {
				if ( getBit(i) == 1) {
					codeWord[fillNumber] = codeTable[i];
					++fillNumber;
				}
			}
			codeWord[fillNumber] = '\0';

			fprintf(decodedReferenceGenome, "%c", IUPACtable[codeWord]);
			fillNumber = 0;

			// Decode the second code word / the second part of the byte (last 4 bits)
			for (int i = 0; i < NUCLEOTIDE_NUMBER; i++) {
				if ( getBit(i + NUCLEOTIDE_NUMBER) == 1) {
					codeWord[fillNumber] = codeTable[i];
					++fillNumber;
				}
			}
			codeWord[fillNumber] = '\0';
			fprintf(decodedReferenceGenome, "%c", IUPACtable[codeWord]);
		}

	}

	fclose(referenceGenome);
	fclose(decodedReferenceGenome);
	return 0;
}

/**
 * Initialise the hash table with all the IUPAC table code words.
 *
 * To lower the time complexity, instead of sorting the decoded code word, we
 * insert all permutation in our hash code table. The slight increase in
 * space complexity is insignificant compared to a 3GB genome.
 */
void fillTheIUPACtable(std::map<std::string, char>& IUPACmap) {
	IUPACmap["-"] = '#';
	IUPACmap["A"] = 'A';

	// For clean file ending
	IUPACmap[""] = '-';

	IUPACmap["CGT"] = 'B';
	IUPACmap["CTG"] = 'B';
	IUPACmap["GCT"] = 'B';
	IUPACmap["GTC"] = 'B';
	IUPACmap["TGC"] = 'B';
	IUPACmap["TCG"] = 'B';

	IUPACmap["C"] = 'C';

	IUPACmap["AGT"] = 'D';
	IUPACmap["ATG"] = 'D';
	IUPACmap["GTA"] = 'D';
	IUPACmap["GAT"] = 'D';
	IUPACmap["TGA"] = 'D';
	IUPACmap["TAG"] = 'D';

	IUPACmap["G"] = 'G';

	IUPACmap["ACT"] = 'H';
	IUPACmap["ATC"] = 'H';
	IUPACmap["CAT"] = 'H';
	IUPACmap["CTA"] = 'H';
	IUPACmap["TCA"] = 'H';
	IUPACmap["TAC"] = 'H';

	IUPACmap["GT"] = 'K';
	IUPACmap["TG"] = 'K';

	IUPACmap["AC"] = 'M';
	IUPACmap["CA"] = 'M';

	IUPACmap["ACGT"] = 'N';
	IUPACmap["ACTG"] = 'N';
	IUPACmap["ATCG"] = 'N';
	IUPACmap["ATGC"] = 'N';
	IUPACmap["AGTC"] = 'N';
	IUPACmap["AGCT"] = 'N';

	IUPACmap["CAGT"] = 'N';
	IUPACmap["CATG"] = 'N';
	IUPACmap["CTAG"] = 'N';
	IUPACmap["CTGA"] = 'N';
	IUPACmap["CGTA"] = 'N';
	IUPACmap["CGAT"] = 'N';

	IUPACmap["GCAT"] = 'N';
	IUPACmap["GCTA"] = 'N';
	IUPACmap["GTCA"] = 'N';
	IUPACmap["GTAC"] = 'N';
	IUPACmap["GATC"] = 'N';
	IUPACmap["GACT"] = 'N';

	IUPACmap["TCGA"] = 'N';
	IUPACmap["TCAG"] = 'N';
	IUPACmap["TACG"] = 'N';
	IUPACmap["TAGC"] = 'N';
	IUPACmap["TGAC"] = 'N';
	IUPACmap["TGCA"] = 'N';

	IUPACmap["AG"] = 'R';
	IUPACmap["GA"] = 'R';

	IUPACmap["CG"] = 'S';
	IUPACmap["GC"] = 'S';

	IUPACmap["T"] = 'T';

	IUPACmap["ACG"] = 'V';

	IUPACmap["AT"] = 'W';
	IUPACmap["TA"] = 'W';

	IUPACmap["CT"] = 'Y';
	IUPACmap["TC"] = 'Y';
}


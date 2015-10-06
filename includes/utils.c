#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_set>
#include <map>

// Head of the functions that are located inside this file
#include "../includes/utils.h"

// Include all the constants
#include "../includes/def.h"

// Include all macros
#include "../includes/macros.h"

/**
 * Read the reference genome and parse it.
 * Skip the first row since it is the comment line.
 */
char *parseInput(int *lengthOfInput) {
	char *S;

	// Open the input file
	FILE *referenceGenome = fopen("../output/referenceGenome", "r+");
	if (referenceGenome == NULL) {
		printf("\nERROR reading the file! - parseInput function");
		exit(1);
	}

	// Get the length of the file
	fseek(referenceGenome, 0L, SEEK_END);
	*lengthOfInput = ftell(referenceGenome);
	fseek(referenceGenome, 0L, SEEK_SET);	// Get back to the start of the file

	// Read the file
	char refChar = fgetc(referenceGenome);
	int i { 0 }, lengthOfTheComment { 0 };
	bool comment = false, firstLoop = true;

	while (refChar != EOF) {
		// Reference genome information
		if (comment == false && lengthOfTheComment > 0) {
			if (firstLoop == true) {
				S = (char *) malloc(sizeof(char) * (*lengthOfInput));
				firstLoop = false;
			}
			// Add the new char to the input
			S[i] = refChar;
			++i;
			refChar = fgetc(referenceGenome);
			// In the middle of the comment line
		} else if (comment == true && refChar != '\n') {
			++lengthOfTheComment;
			refChar = fgetc(referenceGenome);
			// Ending the comment line
		} else if (comment == true && refChar == '\n') {
			++lengthOfTheComment;
			*lengthOfInput = (*lengthOfInput) - lengthOfTheComment;	// Calculate the size of the information part
			comment = false;
			refChar = fgetc(referenceGenome);
			// Starting the comment line
		} else if (refChar == '#') {
			comment = true;
			++lengthOfTheComment;
			refChar = fgetc(referenceGenome);
		}
	}
	// Append $ to the end of S
	//S[i] = '$';
	//++(*lengthOfInput);

	// Close the file
	fclose(referenceGenome);
	return S;
}

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
void artificialLexicographMapper(int inputLengthCompressed, char* S, unsigned char *aritficialCode) {

	// Initialise the hash map for the code mapping
	std::map<std::string, char> artificalMapper;
	artificalMapper["0000"] = 'A';
	artificalMapper["0001"] = 'B';
	artificalMapper["0011"] = 'C';
	artificalMapper["0010"] = 'D';
	artificalMapper["0110"] = 'E';
	artificalMapper["0111"] = 'F';
	artificalMapper["0101"] = 'G';
	artificalMapper["0100"] = 'H';

	artificalMapper["1100"] = 'I';
	artificalMapper["1101"] = 'J';
	artificalMapper["1111"] = 'K';
	artificalMapper["1110"] = 'L';
	artificalMapper["1010"] = 'M';
	artificalMapper["1011"] = 'N';
	artificalMapper["1001"] = 'O';
	artificalMapper["1000"] = 'P';

	// A is reserved for $

	// Decompress and transform 1 codeword (4 bits) to a char equivalent (1 byte) 0000 = A, 0001 = B, 0011 = C (gray code)
	int deCompressedTextIndex { 0 };
	for (int i = 0; i < inputLengthCompressed; i++) {
		char bitVector = S[i];

		// First 4 bits converted to char a equivalent
		char codeWordFirst[NUCLEOTIDE_NUMBER + 1];	// Using 2 code words to get faster performance
		char codeWordSecond[NUCLEOTIDE_NUMBER + 1];	// No need to clean values between foor lopps
		for (int j = 0; j < NUCLEOTIDE_NUMBER; j++) {
			if ( getBit(j) == 1)
				codeWordFirst[j] = '1';
			else
				codeWordFirst[j] = '0';
		}
		aritficialCode[deCompressedTextIndex] = artificalMapper[codeWordFirst];

		++deCompressedTextIndex;

		// Second 4 bits converted to a char equivalent
		for (int j = 0; j < NUCLEOTIDE_NUMBER; j++) {
			if ( getBit(j + NUCLEOTIDE_NUMBER) == 1)// We need to search the last 4 bits, for that reason we add the NUCLETIDE NUMBER
				codeWordSecond[j] = '1';
			else
				codeWordSecond[j] = '0';
		}
		aritficialCode[deCompressedTextIndex] = artificalMapper[codeWordSecond];
		++deCompressedTextIndex;
	}
	aritficialCode[deCompressedTextIndex] = '$'; // For $
	++deCompressedTextIndex;
}

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
void createPrefixSumTable(int deCompressedTextLenght, int* C, unsigned char* artificalCode) {
	C[0] = 0; // since there are no letters before the first once
	char letter;
	// Calculate the C for every letter, use the artificial code for this
	for (int i = 0; i < IUPAC_SIZE + 1; i++) {
		letter = artificialAlphabet[i];
		int sum { 0 };
		// Calculate the C for this alphabet letter
		for (int j = 0; j < deCompressedTextLenght; j++) {
			if (artificalCode[j] < letter) {
				//printf("%c < %c in number format %d < %d\n",artificalCode[j],letter,artificalCode[j],letter);
				++sum;
			}
		}
		// Enter the prefix sum into the C structure
		//if( i > 0 ) 
			//++sum; // Add one for $
		C[i] = sum;
	}
}

/**
 *Gradi abecedu iz sufksnog polja koji je vec soriran. Razlog tome je sto vec imamo SA, pa
 *nema razloga da ga ne iskoristimo. Pogotovo jer je njegov algoritam stvaranja JAKO optimiziran.
 */
char *stvoriAbecedu( char S[], int lengthOfInput, int *duzinaAbecede, int SA[]) {
	 char prethodniZnak, *abeceda = NULL;
	int i = 0;
	// Napravi abecedu preko sufiksnog polja
	abeceda = ( char *) malloc(sizeof(unsigned char) * (IUPAC_SIZE + 1));
	// Po dogvoru $ je leksikografski najmanji element u abecedi UVIJEK
	abeceda[0] = '$';
	*duzinaAbecede = 1;
	prethodniZnak = '$';
	// Predi cijeli ulaz i dodaj sva nova slova u abecedu
	for (i = 0; i < lengthOfInput; i++) {
		// Znak je vec u abecedi preskoci ili je $ koji smo odmah stavili
		if (S[SA[i]] == prethodniZnak || S[SA[i]] == '$') {
			continue;
		}
		// Znak treba staviti u abecedu, povecati njenu duzinu i azurirati varijablu prethodniZnak
		else {
			abeceda[*duzinaAbecede] = S[SA[i]];
			(*duzinaAbecede)++;
			prethodniZnak = S[SA[i]];
		}
	}
	// Radi i bez ovoga, makar od kada smo stavili stablo valica odluka je ovo vratiti
	abeceda[*(duzinaAbecede)] = '\0';

	return abeceda;
}


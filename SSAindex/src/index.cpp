/*
 * index.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: weenkus
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_set>
#include <map>
#include <malloc.h>
#include <vector>
#include <list>
#include <iostream>
#include <string.h>

// Include all the constants
#include "../includes/def.h"

// Include all macros
#include "../includes/macros.h"

// Includes all functions need for easy bit usage, such as printBitVector
#include "../includes/bitUtils.h"

// Includes all the functions for linear time suffix array construction - best case
#include "../lib/divsufsort.h"

// Includes all basic data structures in typedefs
#include "../includes/typedef.h"

// Includes all the utility functions needed for creating most of the basic data structures
#include "../includes/utils.h"

// Includes all functions for creating and querying the wavelet tree
#include "../includes/waveletTree.h"

// Includes all functions for data structure serialization
//#include "../includes/serialization.h"

/**
 * Search for a pattern in the reference genome.
 * Since the prefix sum table is made with the artificial code we use the artificial mapper to
 * return it to its original value and use it together with IUPAC code to get the prefix sum.
 *
 * The algorithm is a extension of the usual FM index backwards pattern search.
 * Since we coded the reference genome with IUPAC code a single letter from a patter matches a set of letters
 * in the referencing genome.
 * The set of letters we call a sub set, so A matches the subSetA, C matches the subSetC and so on.
 *
 *subSetA = "MHNVRDWA"
 *subSetC = "SBYCMHNV"
 *subSetG = "KGSBNVRD"
 *subSetT = "TKBYHNDW"
 *
 *All of the subsets are in the def.h file which you can find in the includes folder.
 *
 *So since one letter matches a set of letters called a sub set in our case, we need to check that letter
 *with the whole subset that's why for each letter of the patter when loop 8 times through each subset pair.
 *(MH,HN,NV,..,WA) for A in the pattern.
 *
 *All the suffix intervals that are invalid are discarded, the suffix interval is not valid if Lp > Rp for the
 *suffix interval is true.
 *Every valid suffix interval goes to a list and is used in the next iteration.
 *
 *In the next iteration we take the second last letter in the pattern and loop again through its subset but this time
 *we do it for each suffix interval we got from the previous iteration.
 *Doing this for all the patter latters (from the end) we can caluclate the occurance of the pattern in the reference genome.
 *
 *To get the occurrence of the patter in the reference genome we sum the (Rp - Lp + 1) for each valid suffix interval in the first letter
 *of the pattern (that will be our last iteration).
 *
 *If the algorithm is still unclear check the thesis memory efficient store of genomes of the same species, or
 *BWBBLE algorithm online.
 */
int patternSearch(char *pattern, node *root, int *C, int decompressedInputLength, char *alphabet, int alphabetLength) {
	int patternLength = strlen(pattern);

	// Check if the pattern has all letters within the alphabet
	for (int i = 0; i < patternLength; i++) {
		if (pattern[i] != 'A' && pattern[i] != 'C' && pattern[i] != 'G' && pattern[i] != 'T')
			return 0;
	}

	// Check if the pattern is longer than the input
	if (patternLength > decompressedInputLength)
		return 0;

	// Initialise the hash map for the code mapping used with the prefix sum table
	std::map<char, int> artificalMapper;
	artificalMapper['$'] = 0; // added at the end of the input text
	artificalMapper['#'] = 1; // 0000
	artificalMapper['T'] = 2; // 0001
	artificalMapper['K'] = 3; // 0011
	artificalMapper['G'] = 4; // 0010
	artificalMapper['S'] = 5; // 0110
	artificalMapper['B'] = 6; // 0111
	artificalMapper['Y'] = 7; // 0101
	artificalMapper['C'] = 8; // 0100

	artificalMapper['M'] = 9; // 1100
	artificalMapper['H'] = 10; // 1101
	artificalMapper['N'] = 11; // 1111
	artificalMapper['V'] = 12; // 1110
	artificalMapper['R'] = 13; // 1010
	artificalMapper['D'] = 14; // 1011
	artificalMapper['W'] = 15; // 1001
	artificalMapper['A'] = 16; // 1000

	// Initialise all the needed data structures
	std::vector<suffixInterval> suffIntervalSubSet(SUBSET_LENGTH);
	std::list<suffixInterval> finalSuffixIntervals;
	std::list<suffixInterval> suffIntervalBranching;

	// Start the backward search algorithm
	for (int i = (patternLength - 1); i >= 0; i--) {
		// Choose a subset
		char subSet[SUBSET_LENGTH + 1];
		if (pattern[i] == 'A')
			strcpy(subSet, subSetA);
		else if (pattern[i] == 'C')
			strcpy(subSet, subSetC);
		else if (pattern[i] == 'G')
			strcpy(subSet, subSetG);
		else if (pattern[i] == 'T')
			strcpy(subSet, subSetT);

		// Add the initial value of the suffix interval (which is the length of the input)
		if (i == (patternLength - 1)) {
			suffixInterval suf;
			suf.Lp = 0;
			suf.Rp = decompressedInputLength - 1;
			suffIntervalBranching.push_back(suf);
		}

		// Copy the contetnt of the branching to the secondBranching for the next iteration and clear the initial values
		std::list<suffixInterval> suffIntervalBranchingSecond(suffIntervalBranching);
		suffIntervalBranching.clear();

		// Fill the second bracnhing list with values

		std::list<suffixInterval>::iterator it;
		for (it = suffIntervalBranchingSecond.begin(); it != suffIntervalBranchingSecond.end(); ++it) {

			//Since one nucleotide matches more letters for each nucleotide we will have more suffix intervals
			for (int j = 0; j < SUBSET_LENGTH; j++) {
				if (i != (patternLength - 1) && (*it).Lp > (*it).Rp)
					continue;

				//printf("C[%c] = %d + O(%c,%d) = %d + 1\n", subSet[j], C[artificalMapper[subSet[j]]], subSet[j],
				//	suffIntervalSubSet[j].Lp - 1,
				//	rankBit(root, subSet[j], (*it).Lp - 1, alphabet, alphabetLength, decompressedInputLength));
				suffIntervalSubSet[j].Lp = C[artificalMapper[subSet[j]]]
						+ rankBit(root, subSet[j], (*it).Lp - 1, alphabet, alphabetLength, decompressedInputLength) + 1
						- 1;

				//printf("C[%c] = %d + O(%c,%d) = %d\n", subSet[j], C[artificalMapper[subSet[j]]], subSet[j],
				//	suffIntervalSubSet[j].Rp,
				//rankBit(root, subSet[j], (*it).Rp, alphabet, alphabetLength, decompressedInputLength));
				suffIntervalSubSet[j].Rp = C[artificalMapper[subSet[j]]]
						+ rankBit(root, subSet[j], (*it).Rp, alphabet, alphabetLength, decompressedInputLength) - 1;

				// If the suffix interval is valid save it for next iteration
				if (suffIntervalSubSet[j].Rp >= suffIntervalSubSet[j].Lp && i != 0) {
					suffIntervalBranching.push_back(suffIntervalSubSet[j]);
					//printf("%d,%d\n\n", suffIntervalSubSet[j].Lp, suffIntervalSubSet[j].Rp);
				}
				// In the final iteration put all the suffix intervals in the final list for occurance counting!
				if (suffIntervalSubSet[j].Rp >= suffIntervalSubSet[j].Lp && i == 0) {
					finalSuffixIntervals.push_back(suffIntervalSubSet[j]);
					//printf("%d,%d\n\n", suffIntervalSubSet[j].Lp, suffIntervalSubSet[j].Rp);
				}
			}
			//printf("--------------NEW ITERATION---------\n\n");
		}

	}
	int patternOcc { 0 };
	std::list<suffixInterval>::iterator it;
	for (it = finalSuffixIntervals.begin(); it != finalSuffixIntervals.end(); ++it) {
		if ((*it).Rp >= (*it).Lp) {
			//printf("%d\n",suffIntervalSet[i].Rp - suffIntervalSet[i].Lp + 1);
			patternOcc = patternOcc + (*it).Rp - (*it).Lp + 1;
		}
	}

	return patternOcc;
}

/**
 * Write the suffix array into a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the suffix array.
 *
 * The row under has all the suffix array values, each separated with a space.
 */
int serializeSA(int SA[], int decompressedTextLength) {
	FILE *serializeSA;

	// Creating a file to write the suffix array
	serializeSA = fopen("../savedStructures/serializeSA", "w");
	if (serializeSA == NULL) {
		printf("\nThe OS didn't manage to create serializeSA file!");
		return 0;
	}

	// Save the length as a header file
	fprintf(serializeSA, "#%d\n", decompressedTextLength);

	// Save the stracture
	for (int i = 0; i < decompressedTextLength; i++) {
		fprintf(serializeSA, "%d ", SA[i]);
	}

	fclose(serializeSA);
	return 1;
}

/**
 * Write the Burrows-Wheeler transform into a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the BWT.
 *
 * The row under has all the BWT values.
 */
int serializeAlphabet(char alphabet[], int alphabetLength) {
	FILE *serializeAlphabet;

	// Creating a file to write the suffix array
	serializeAlphabet = fopen("../savedStructures/serializeAlphabet", "w");
	if (serializeAlphabet == NULL) {
		printf("\nThe OS didn't manage to create serializeAlphabet file!");
		return 0;
	}

	// Save the length as a header file
	fprintf(serializeAlphabet, "#%d\n", alphabetLength);

	// Save the structure
	for (int i = 0; i < alphabetLength; i++) {
		fprintf(serializeAlphabet, "%c", alphabet[i]);
	}

	fclose(serializeAlphabet);
	return 1;
}

/**
 * Write the Alphabet into a file.
 * The alphabet is SORTED.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the Alphabet.
 *
 * The row under has all the alphabet values.
 */
int serializeBWT(char BWT[], int decompressedTextLength) {
	FILE *serializeBWT;

	// Creating a file to write the suffix array
	serializeBWT = fopen("../savedStructures/serializeBWT", "w");
	if (serializeSA == NULL) {
		printf("\nThe OS didn't manage to create serializeBWT file!");
		return 0;
	}

	// Save the length as a header file
	fprintf(serializeBWT, "#%d\n", decompressedTextLength);

	// Save the structure
	for (int i = 0; i < decompressedTextLength; i++) {
		fprintf(serializeBWT, "%c", BWT[i]);
	}

	fclose(serializeBWT);
	return 1;
}

/**
 * Write the prefix sum table - C into a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the prefix sum table.
 *
 * The row under has all the prefix sum table values, each separated with a space.
 */
int serializeC(int C[], int alphabetLength) {
	FILE *serializeC;

	// Creating a file to write the suffix array
	serializeC = fopen("../savedStructures/serializeC", "w");
	if (serializeSA == NULL) {
		printf("\nThe OS didn't manage to create serializeC file!");
		return 0;
	}

	// Save the length as a header file
	fprintf(serializeC, "#%d\n", alphabetLength);

	// Save the structure
	for (int i = 0; i < alphabetLength; i++) {
		fprintf(serializeC, "%d ", C[i]);
	}

	fclose(serializeC);
	return 1;
}

/**
 * Recursively (preorder traverals) write the tree to a file.
 */
void writeWaveletToFile(node *root, FILE *fileToWrite) {
	if (!root) {
		fprintf(fileToWrite, "# ");
	} else {
		fprintf(fileToWrite, "%s ", root->val);
		writeWaveletToFile(root->left, fileToWrite);
		writeWaveletToFile(root->right, fileToWrite);
	}
}

/**
 * Write the wavelet tree into a file.
 *
 * We use the preorder traversal, writing a special sentinel on every null node.
 * The special sentinel is #.
 *
 * For more information check:
 * http://articles.leetcode.com/2010/09/serializationdeserialization-of-binary.html
 */
int serializeWavletTree(node *root) {
	FILE *serializeC;

	// Creating a file to write the suffix array
	serializeC = fopen("../savedStructures/serializeWaveletTree", "w");
	if (serializeSA == NULL) {
		printf("\nThe OS didn't manage to create serializeWaveletTree file!");
		return 0;
	}

	// Save the length as a header file
	writeWaveletToFile(root, serializeC);

	fclose(serializeC);
	return 1;
}

int main(int argc, char **argv) {

	int a = 2;
	// Menu
	printf("Welcome to the reference genome index.\n\n");
	printf("To load data from the load file type 'l'.\n");
	printf("To create an index from the output file type 'c'.\n");

	char userChoice;
	scanf("%c", &userChoice);

	// Create the index
	if (userChoice == 'c') {

		// Parse the input folder (bitVector has the input folder in bits)
		int inputLengthCompressed { 0 };
		char *S = NULL;
		S = parseInput(&inputLengthCompressed);

		// Transform S into something that SA can work with (make an artificial lexicographical ordering), i.e. artificalCode
		int deCompressedTextLenght = (inputLengthCompressed * 2) + 1;
		unsigned char *artificialCode = (unsigned char *) malloc(sizeof(unsigned char) * deCompressedTextLenght);

		// Decompress and transform 1 codeword (4 bits) to a char equivalent (1 byte) 0000 = A, 0001 = B, 0011 = C (gray code)
		artificialLexicographMapper(inputLengthCompressed, S, artificialCode);
		artificialCode[deCompressedTextLenght + -1] = '$';

		// Print artificial code
		printf("Artificial code: %s of length %d\n", artificialCode, deCompressedTextLenght);

		// Test the suffix array library
		int *SA = (int *) malloc(deCompressedTextLenght * sizeof(int));
		divsufsort(artificialCode, SA, deCompressedTextLenght);

		// Test the suffix array created by the artificial code
		printf("Suffix array: ");
		for (int i = 0; i < deCompressedTextLenght; i++)
			printf("%d ", SA[i]);

		// Generate the prefix sum table i.e. C
		int *C = (int *) malloc((IUPAC_SIZE + 1) * sizeof(int));
		createPrefixSumTable(deCompressedTextLenght, C, artificialCode);

		// Test the prefix sum table created by the artificial code
		printf("\nPrefix sum table: ");
		for (int i = 0; i < IUPAC_SIZE + 1; i++)
			printf("\nC[%c] = %d ", artificialAlphabet[i], C[i]);

		// Create the Burrows-Wheeler transform (BWT)
		// Initialise the hash map for the code mapping
		std::map<char, char> artificalMapper;
		artificalMapper['A'] = '#';	// 0000
		artificalMapper['B'] = 'T';	// 0001
		artificalMapper['C'] = 'K';	// 0011
		artificalMapper['D'] = 'G';	// 0010
		artificalMapper['E'] = 'S';	// 0110
		artificalMapper['F'] = 'B';	// 0111
		artificalMapper['G'] = 'Y';	// 0101
		artificalMapper['H'] = 'C';	// 0100

		artificalMapper['I'] = 'M';	// 1100
		artificalMapper['J'] = 'H';	// 1101
		artificalMapper['K'] = 'N';	// 1111
		artificalMapper['L'] = 'V';	// 1110
		artificalMapper['M'] = 'R';	// 1010
		artificalMapper['N'] = 'D';	// 1011
		artificalMapper['O'] = 'W';	// 1001
		artificalMapper['P'] = 'A';	// 1000

		char *BWT = (char *) malloc(deCompressedTextLenght * sizeof(char));
		char *alphabet;
		for (int i = 0; i < deCompressedTextLenght; i++) {
			if (SA[i] != 0) {
				// Transfer the BWT from artifical code to IUPAC for easier pattern searching
				BWT[i] = artificalMapper[artificialCode[SA[i] - 1]];

			} else
				BWT[i] = '$';

		}
		printf("\nBWT: ");
		puts(BWT);

		int alphabetLength;
		alphabet = stvoriAbecedu(BWT, deCompressedTextLenght, &alphabetLength, SA);
		alphabet[alphabetLength - 1] = '\0';
		alphabetLength--;

		printf("Alphabet: ");
		puts(alphabet);
		printf("\n");

		// Create the wavelet tree
		node *root = NULL;
		insertBit(&root, deCompressedTextLenght, alphabetLength, alphabet, BWT);

		// Let the user save the index if he wants to
		char userInputSerialization;
		fflush(stdout);
		printf(
				"Index successfully created, type 's' to save it to the load file. (later you can load it to save time)\n");
		scanf(" %c", &userInputSerialization);
		if (userInputSerialization == 's') {
			printf("\n");

			// SA serialization
			if (serializeSA(SA, deCompressedTextLenght) == 1)
				printf("Suffix array saved.\n");
			else
				printf("Problem while saving the suffix array.\n");

			// BWT serialization
			if (serializeBWT(BWT, deCompressedTextLenght) == 1)
				printf("Burrows-Wheeler transform saved.\n");
			else
				printf("Problem while saving the Burrows-Wheeler transform.\n");

			// Alphabet serialization
			if (serializeAlphabet(alphabet, alphabetLength) == 1)
				printf("Alphabet saved.\n");
			else
				printf("Problem while saving the alphabet.\n");

			// C serialization - prefix sum table
			if (serializeC(C, IUPAC_SIZE + 1) == 1)
				printf("Prefix sum table - C saved.\n");
			else
				printf("Problem while saving the prefix sum table (C).\n");

			// WaveletTree serialization - OCC
			if (serializeWavletTree(root) == 1)
				printf("Wavelet tree - OCC saved.\n");
			else
				printf("Problem while saving the wavelet tree (OCC).\n");
		}

		char userInput;
		printf("\n");
		do {
			printf("Enter a pattern you want to search: ");
			char pattern[IUPAC_SIZE + 1];
			scanf("%s", pattern);
			int occ = patternSearch(pattern, root, C, deCompressedTextLenght, alphabet, alphabetLength);

			// Ask the user if he wants another serach
			printf("Your pattern was repeated %d times in the original reference genome.\n\n", occ);
			printf("Type 'r' if you want another search or 'e' to exit.\n");
			fflush(stdin);
			scanf(" %c", &userInput);

		} while (userInput == 'r');

		// Free all the taken heap memory
		free(S);
		free(artificialCode);
		free(SA);
		free(C);
		free(BWT);

		// Erase the wavelet tree
		deallocateTree(&root);
	}

	printf("\nThank you for using the reference genome index.");

	return 0;
}


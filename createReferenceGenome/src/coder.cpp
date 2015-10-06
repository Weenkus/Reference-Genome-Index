/*
 * main.cpp
 *
 *  Created on: Mar 26, 2015
 *      Author: weenkus
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <unordered_set>
#include <map>
#include <cassert>
#include <time.h>

// Includes all functions need for easy bit usage, such as printBitVector
#include "../includes/bitUtils.h"

// Include all the constants
#include "../includes/def.h"

// Include all macros
#include "../includes/macros.h"

int codeTableIndex(char codeTable[5], char curChar1);
void initialiseTheHashCodeTable(char codeTable[5],
		std::map<char, int>& hashCodeTable);

int main() {
	DIR *makePool;
	FILE *referenceGenome;
	clock_t begin, end;
	double timeSpent;

	begin = clock();

	// Creating a file to write the encoded multi-genome
	referenceGenome = fopen("../output/referenceGenome", "w+");
	if (referenceGenome == NULL) {
		printf("\nThe OS didn't manage to create/open the referenceGenome!");
		return 1;
	}

	// Create a folder stream for all the genomes fastq files
	if (NULL == (makePool = opendir("../makePool"))) {
		printf("\nThe directory path from pointer folderStream is wrong!");
		return 1;
	}

	// Generate the hash table
	std::map<char, int> hashCodeTable;
	char codeTable[5];
	printf("Enter the code table (order the nucleotides, i.e. ACGT): ");
	scanf("%s", codeTable);
	initialiseTheHashCodeTable(codeTable, hashCodeTable);
	// Write the code table into the encoded multi-genome folder
	fprintf(referenceGenome, "#%s => CODE TABLE\n", codeTable);

	// Loop through all the files in the directory and encode them
	struct dirent* fileList;
	int numberOfFilesThatAreEncoded { 0 };
	while ((fileList = readdir(makePool))) {
		// Skip the current and top folder
		if (!strcmp(fileList->d_name, "."))
			continue;
		if (!strcmp(fileList->d_name, ".."))
			continue;

		// Open the file for reading
		char fileName[MAX_FILENAME_LENGTH + 1] = "../makePool/";
		strcat(fileName, fileList->d_name);
		FILE *currentFile = fopen(fileName, "r");
		if (currentFile == NULL) {
			printf("\nError : Failed to open entry file!\n");
			return 1;
		}

		// Start coding the read to the genome
		char encChar, curChar1, curChar2;

		// Write from the begining of the enconded file every time
		fseek(referenceGenome, 0, SEEK_SET);

		// Since the first line in the multi-genome has code table info, skip it!
		do {
			fscanf(referenceGenome, "%c", &encChar);
		} while (encChar != '\n');

		printf("\nFile: %s\n", fileList->d_name);
		// Encode the current read with the reference genome
		bool secondCharIsEOF = false;

		while ((fscanf(currentFile, "%c", &curChar1)) != EOF) {
			if (fscanf(currentFile, "%c", &curChar2) == EOF)
				secondCharIsEOF = true;
			char bitVector = 0x00;

			// Encoding genome is empty since it is the first read, therefore, just copy/paste the char from the read
			if (numberOfFilesThatAreEncoded == 0) {

				bitVector = setBit(hashCodeTable[curChar1], 1);
				//bitVector = setBitOne(hashCodeTable[curChar1]);
				//printf("%c ", curChar1);
				if (secondCharIsEOF == false) {
					bitVector = setBit(
							hashCodeTable[curChar2] + NUCLEOTIDE_NUMBER, 1);

					//bitVector = setBitOne(
					//		hashCodeTable[curChar2] + NUCLEOTIDE_NUMBER);
					//printf("%c ", curChar2);
				}
				printBitVector(bitVector);
				fwrite(&bitVector, 1, sizeof(bitVector), referenceGenome); // writing one byte

			} else {

				// Multi-genome has text we need to combine it with the reads and recode it
				if (fscanf(referenceGenome, "%c", &encChar) == EOF)
					encChar = 0x00;
				fseek(referenceGenome, -1 * sizeof(char), SEEK_CUR);// Ughh xD

				bitVector = setBit(hashCodeTable[curChar1], 1);
				//printf("%c ", curChar1);
				if (secondCharIsEOF == false) {
					bitVector = setBit(
							hashCodeTable[curChar2] + NUCLEOTIDE_NUMBER, 1);
					//printf("%c ", curChar2);
				}
				printBitVector(bitVector);

				bitVector |= encChar;
				fwrite(&bitVector, 1, sizeof(bitVector), referenceGenome);
			}

		}
		++numberOfFilesThatAreEncoded;
		// Close the current file/read
		fclose(currentFile);
	}

	end = clock();
	timeSpent = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("Time needed to code the input is %f.\n", timeSpent);

	fclose(referenceGenome);
	closedir(makePool);
	return 0;
}

/**
 * Returns the code for a char, using hashmap is a better solutionF
 */
int codeTableIndex(char codeTable[5], char curChar1) {
	for (int i = 0; i < NUCLEOTIDE_NUMBER; i++) {
		if (codeTable[i] == curChar1) {
			return i;
		}
	}
	// This will never run
	printf("The is an error, within the code table!\n");
	return 0;
}

/**
 * The fastest code table algorithm, using the hash map.
 * The code table is implemented so that the nucleotide is the key and
 * the returning integer is the position of the one in gray code.
 *
 * As a example, if we have a code table ACGT, and the user send G,
 * the function will return 2 meaning that at index 2 we need to write 1,
 * i.e. we would code G as 0010.
 */
void initialiseTheHashCodeTable(char codeTable[5],
		std::map<char, int>& hashCodeTable) {
	codeTable[NUCLEOTIDE_NUMBER] = '\0';
	for (int i = 0; i < NUCLEOTIDE_NUMBER; i++) {
		hashCodeTable[codeTable[i]] = i;
	}
}

